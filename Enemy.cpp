#include "Enemy.h"
#include "Time.h"
#include "Math.h"
#include "Animation3D.h"
#include "Collision3D.h"
#include "Input.h"
#include "Lerp.h"
#include "Quaternion.h"
#include <math.h>

// アニメーションリスト
const char* Enemy::AnimList[RoopAnimNum] =
{
	"Zombie/ZombieIdle.mv1",
	"Zombie/ZombieWalk.mv1",
	"Zombie/ZombieAttack.mv1",
};

Enemy::Enemy(Collision3D* collision3D) :
	Actor("Enemy"),
	m_model(MV1LoadModel("Resource/Zombie/Zombie.mv1")),
	m_animIndex(0),
	m_nowAnim(RoopAnim::Idle),
	m_nextAnim(RoopAnim::Idle),
	m_collision3D(collision3D)
{
	// アニメーションクラスをリスト化する
	for (int i = 0; i < RoopAnimNum; i++)
	{
		m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i]));
		AddChild(m_attachAnimList[i]);
	}

	// 敵の位置(x, y, z)
	m_enemyPos = SpawnPos;

	// モデルを指定場所に描画
	MV1SetPosition(m_model, SpawnPos);

	// アニメーション用の位置
	m_enemyPastPos = m_enemyPos;

	// ディレクショナルライトを作成
	m_lightHandle = CreateDirLightHandle(Vector3(0, 100.0f, 0));

	// 最初のアニメーションを指定
	m_attachAnimList[static_cast<int>(RoopAnim::Idle)]->FadeIn();
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
void Enemy::ChangeAnimQuick(const RoopAnim nextAnim)
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
	// ライトハンドルを削除
	DeleteLightHandle(m_lightHandle);
	// プレイヤーのモデルを削除
	MV1DeleteModel(m_model);
}

void Enemy::Update()
{
	// 1フレーム前の位置を更新
	m_enemyPastPos = m_enemyPos;

	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_1))
	{
		m_nextAnim = RoopAnim::Idle;
	}
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_2))
	{
		m_nextAnim = RoopAnim::Walk;
	}
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_3))
	{
		m_nextAnim = RoopAnim::Attack;
	}

	// アニメーションの切り替え
	ChangeAnimLerp();
}

void Enemy::Draw()
{
	// アニメーション再生
	PlayAnim();
}
