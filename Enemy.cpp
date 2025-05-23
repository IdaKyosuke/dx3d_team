#include "Enemy.h"
#include "Time.h"
#include "Math.h"
#include "Animation3D.h"
#include "Collision3D.h"
#include"ActorCollision3D.h"
#include"BoxCollider3D.h"
#include"LoadPlayer.h"
#include"CheckRoot.h"
#include"Quaternion.h"
#include"Audio3D.h"
#include"NavMesh.h"
#include "Input.h"
#include "Lerp.h"
#include<future>
#include <math.h>

// アニメーションリスト
const char* Enemy::AnimList[AnimNum] =
{
	"Zombie/ZombieIdle.mv1",
	"Zombie/ZombieWalk.mv1",
	"Zombie/ZombieAttack.mv1",
};

Enemy::Enemy(NavMesh* navMesh, const Vector3& pos, LoadPlayer* loadPlayer) :
	Actor3D("Enemy", pos),
	m_model(MV1LoadModel("Resource/Zombie/Zombie.mv1")),
	m_player(loadPlayer),
	m_animIndex(0),
	m_nowAnim(Anim::Idle),
	m_nextAnim(Anim::Idle),
	m_navMesh(navMesh),
	m_moveDirection(Vector3(0,0,0)),
	m_isSet(false),
	m_isFind(false),
	m_isAttack(false),
	m_countCoolTime(false),
	m_durationCoolTime(0),
	m_isMove(false),
	m_polyCount(0),
	m_isCheck(false),
	m_pastAttackFlg(false),
	m_stoped(false)
{
	// アニメーションクラスをリスト化する
	for (int i = 0; i < AnimNum; i++)
	{
		if (i < AnimNum - 1)
		{
			// ループするアニメーション
			m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i]));
			AddChild(m_attachAnimList[i]);
		}
		else
		{
			// ループしないアニメーション
			m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i], false));
			AddChild(m_attachAnimList[i]);
		}
	}

	// 敵の音
	AddChild(new Audio3D("Resource/sound/zombie_voice.mp3", this));

	// モデルを指定場所に描画
	MV1SetPosition(m_model, pos);

	// アニメーション用の位置
	m_enemyPastPos = m_enemyPos;

	// 最初のアニメーションを指定
	m_attachAnimList[static_cast<int>(Anim::Idle)]->FadeIn();

	m_collider = new BoxCollider3D(FindColSize, ColOffset);

	m_checkRoot = new CheckRoot(m_navMesh);
}

// アニメーションを切り替える(Lerp)
void Enemy::ChangeAnimLerp()
{
	if (m_nowAnim == m_nextAnim) return;

	m_attachAnimList[static_cast<int>(m_nowAnim)]->FadeOut();
	m_attachAnimList[static_cast<int>(m_nextAnim)]->FadeIn();

	m_nowAnim = m_nextAnim;
	
}

// アニメーションを切り替える(即座)
void Enemy::ChangeAnimQuick(const Anim nextAnim)
{
	m_attachAnimList[static_cast<int>(m_nowAnim)]->ChangeOut();
	m_attachAnimList[static_cast<int>(nextAnim)]->ChangeIn();

	m_nowAnim = nextAnim;
	m_nextAnim = nextAnim;
	
}

// アニメーションを再生する
void Enemy::PlayAnim()
{
	// モデルの描画
	MV1DrawModel(m_model);
#ifdef _DEBUG
	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"EnemyPos Vector3(%.0f, %.0f, %.0f)",
		m_enemyPos.x, m_enemyPos.y, m_enemyPos.z
	);
#endif // _DEBUG

}

void Enemy::Update()
{
	if (m_player->IsTheWorld())
	{
		if (!m_stoped)
		{
			m_stoped = m_player->IsTheWorld();
			m_attachAnimList[static_cast<int>(m_nowAnim)]->StopAnim();
		}
		return;
	}
	else
	{
		if (m_stoped != m_player->IsTheWorld())
		{
			m_stoped = m_player->IsTheWorld();
			m_attachAnimList[static_cast<int>(m_nowAnim)]->StartAnim();
		}	
	}
		
	if (m_isAttack)
	{
		Attack();
	}
	else
	{
		// 攻撃中でないとき
		if (m_isFind)
		{
			MoveCombat();
			// アニメーションは常に移動アニメーション
			m_nextAnim = Anim::Run;
		}
		else
		{
			MoveWanderAround();
		}
		// 現在の移動方向を取得
		m_moveDirection = m_transform.position - m_enemyPastPos;
	}
	
	// モデルの回転
	if (!m_moveDirection.IsZero())
	{
		float afterAngle = 0;

		Math::MatchAngleSign(afterAngle, m_moveDirection, m_transform.angle);

		m_transform.angle.y = Lerp::Exec(m_transform.angle.y, afterAngle, 0.2f);
	}

	// アニメーションの切り替え
	ChangeAnimLerp();

	m_enemyPastPos = this->GetPosition();

	m_pastAttackFlg = m_isAttack;
}

// 敵の移動（臨戦態勢）
void Enemy::MoveCombat()
{
	if (!m_isCheck)
	{
		if (!m_found)
		{
			m_thread = new std::thread(&CheckRoot::SetPathPlan, m_checkRoot, this->GetPosition(), m_player->GetPosition(), &m_polyCount, &m_found);
			m_thread->detach();
		}
		else
		{
			// 移動準備
			m_checkRoot->MoveInitialize(this->GetPosition());
			m_isCheck = true;
			m_found = false;

			// threadの破棄
			delete m_thread;
			m_thread = nullptr;
		}
	}
	else
	{
		m_transform.position = m_checkRoot->Move(this->GetPosition(), MoveSpeedCombat, Width, &m_polyCount);

		// ある程度移動ができたら || プレイヤーに攻撃したら || プレイヤーと同じポリゴンに乗ったら　再探索
		if (m_polyCount <= 0 || 
			m_pastAttackFlg != m_isAttack ||
			m_checkRoot->CheckPlayerPoly(this->GetPosition(), m_player->GetPosition())
			)
		{
			// 今回の探索情報を削除
			m_checkRoot->RemovePathPlan();
			m_isCheck = false;
		}
	}
}

// 敵の移動（徘徊）
void Enemy::MoveWanderAround()
{
	if (!m_isMove)
	{
		if (!m_found)
		{
			m_thread = new std::thread(&CheckRoot::SetPathPlan, m_checkRoot, this->GetPosition(), m_navMesh->GetPos(), &m_polyCount, &m_found);
			m_thread->detach();
		}
		else
		{
			// 移動準備
			m_checkRoot->MoveInitialize(this->GetPosition());
			m_isMove = true;
			m_found = false;

			// threadの破棄
			delete m_thread;
			m_thread = nullptr;
		}
	}
	else
	{
		if (m_transform.position != m_checkRoot->Move(this->GetPosition(), MoveSpeed, Width, &m_polyCount))
		{
			// 目的地に到着していないとき
			m_transform.position = m_checkRoot->Move(this->GetPosition(), MoveSpeed, Width, &m_polyCount);
		}
		else
		{
			// 目的に到着 => 今回の探索結果を破棄 && 再度目的地の設定と経路探索
			m_checkRoot->RemovePathPlan();
 			m_isMove = false;
		}

		// アニメーション
		if (this->GetPosition() != m_enemyPastPos)
		{
			m_nextAnim = Anim::Run;
		}
		else
		{
			m_nextAnim = Anim::Idle;
		}
	}
	
}

void Enemy::Draw()
{
	if (!m_player->IsTheWorld())
	{
		// アニメーション再生
		PlayAnim();

		Quaternion::RotateAxisY(m_model, m_transform.angle.y, m_transform.position);
	}

	// モデルの描画
	MV1DrawModel(m_model);

#ifdef _DEBUG
	DrawFormatString(0, 40, GetColor(255, 255, 255),
		"EnemyPos Vector3(%.0f, %.0f, %.0f)",
		this->GetPosition().x, this->GetPosition().y, this->GetPosition().z
	);
#endif // DEBUG
}

void Enemy::OnCollision(const Actor3D* other)
{
	if (other->GetName() == "Player" && !m_player->IsTheWorld())
	{
		if (!m_isFind)
		{
			// プレイヤーを見つけていない => コライダーを小さくする
			m_collider->ChangeSize(AttackColSize);
			m_isFind = true;

			// プレイヤーを再探索
			delete m_thread;
			m_thread = nullptr;
			m_checkRoot->RemovePathPlan();
			m_found = false;
		}
		else
		{
			if (m_isAttack) return;
			// プレイヤーを発見済み => 攻撃
			m_nextAnim = Anim::Attack;
			m_player->DecreaseHP(Power);
			m_isAttack = true;
		}
	}
}

// 敵の攻撃
void Enemy::Attack()
{
	// 攻撃中は動かない
	if (m_attachAnimList[static_cast<int>(m_nowAnim)]->FinishAnim())
	{
		// 攻撃アニメーションの終了と同時にアニメーション変更
		ChangeAnimQuick(Anim::Idle);
		m_countCoolTime = true;
	}

	if (m_countCoolTime)
	{
		m_durationCoolTime += Time::GetInstance()->GetDeltaTime();

		// クールタイム終了
		if (m_durationCoolTime <= CoolTime)
		{
			m_countCoolTime = false;
			m_durationCoolTime = 0;
			m_isAttack = false;
		}
	}
}

// リソースの解放
void Enemy::Release()
{
	// モデルを削除
	MV1DeleteModel(m_model);

	// threadを破棄
	delete m_thread;
	m_thread = nullptr;

	// もともとのreleaseを実行
	Actor3D::Release();
}