#pragma once
#include "Actor.h"
#include "Vector3.h"
#include "DxLib.h"
#include <vector>

class Animation3D;
class Collision3D;

class Enemy : public Actor
{
public:
	// ループするアニメーション
	enum class RoopAnim
	{
		Idle,
		Walk,
		Attack,

		Length,
	};

private:
	std::vector<Animation3D*> m_attachAnimList;

	static constexpr int RoopAnimNum = static_cast<int>(RoopAnim::Length);	// アニメーションの数

	static constexpr float WalkSpeed = 4.0f;	// 歩く速度
	static constexpr Vector3 SpawnPos = Vector3(0, 20, 0);	// ステージにスポーンする場所

	static const char* AnimList[RoopAnimNum];

	int m_model;	// 敵のモデル

	int m_animIndex;	// 再生中のアニメーション

	int m_lightHandle;	// 光源を作成する

	Collision3D* m_collision3D;

	Vector3 m_enemyPos;
	Vector3 m_enemyPastPos;

	// アニメーション切り替え用
	RoopAnim m_nowAnim;
	RoopAnim m_nextAnim;


protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	Enemy(Collision3D* collision3D);

	// アニメーションを切り替える(Lerp)
	void ChangeAnimLerp();

	// アニメーションを切り替える(即座)
	void ChangeAnimQuick(const RoopAnim nextAnim);

	// アニメーションを再生する
	void PlayAnim();

	// モデル関係を削除
	void Finalize();

	// 敵の位置を返す
	Vector3 EnemyPos()
	{
		return m_enemyPos;
	}
};
