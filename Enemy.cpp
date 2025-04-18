#include "Enemy.h"
#include "Time.h"
#include "Math.h"
#include "Animation3D.h"
#include "Collision3D.h"
#include"ActorCollision3D.h"
#include"BoxCollider3D.h"
#include"LoadPlayer.h"
#include"Quaternion.h"
#include"NavMesh.h"
#include "Input.h"
#include "Lerp.h"
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
	m_animIndex(0),
	m_nowAnim(Anim::Idle),
	m_nextAnim(Anim::Idle),
	m_navMesh(navMesh),
	m_player(loadPlayer),
	m_moveDirection(Vector3(0,0,0)),
	m_isSet(false),
	m_isFind(false),
	m_isAttack(false),
	m_countCoolTime(false),
	m_durationCoolTime(0)
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

	// モデルを指定場所に描画
	MV1SetPosition(m_model, pos);

	// アニメーション用の位置
	m_enemyPastPos = m_enemyPos;

	// 最初のアニメーションを指定
	m_attachAnimList[static_cast<int>(Anim::Idle)]->FadeIn();

	m_collider = new BoxCollider3D(FindColSize, ColOffset);
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

	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"EnemyPos Vector3(%.0f, %.0f, %.0f)",
		m_enemyPos.x, m_enemyPos.y, m_enemyPos.z
	);
}

// モデル関係を削除
void Enemy::Finalize()
{
	// モデルを削除
	MV1DeleteModel(m_model);
}

void Enemy::Update()
{
	if (m_isAttack)
	{
		Attack();
	}
	else
	{
		// 攻撃中でないとき
		EnemyMove();
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
}

// 敵の移動
void Enemy::EnemyMove()
{
	// 自身とプレイヤー間の経路探索を行う
	m_navMesh->SetPathPlan(this->GetPosition(), m_player->GetPosition());

	// 移動準備
	m_navMesh->MoveInitialize(this->GetPosition());

	m_transform.position = m_navMesh->Move(this->GetPosition(), MoveSpeed, 20.0f);

	// 今回の探索情報を削除
	m_navMesh->RemovePathPlan();

	m_moveDirection = m_transform.position - m_enemyPastPos;

	// 移動アニメーション
	if (this->GetPosition() != m_enemyPastPos)
	{
		m_nextAnim = Anim::Run;
	}
	else
	{
		m_nextAnim = Anim::Idle;
	}
}

void Enemy::Draw()
{
	// アニメーション再生
	PlayAnim();

	Quaternion::RotateAxisY(m_model, m_transform.angle.y, m_transform.position);

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
	if (other->GetName() == "Player")
	{
		if (!m_isFind)
		{
			// プレイヤーを見つけていない => コライダーを小さくする
			// いったん削除
			m_collider->ChangeSize(AttackColSize);
			m_isFind = true;

		}
		else
		{
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
