#pragma once
<<<<<<< HEAD
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

=======
#include"Actor3D.h"
#include"Vector3.h"
#include"DxLib.h"
#include<vector>

class Animation3D;
class NavMesh;
class LoadPlayer;

class Enemy : public Actor3D
{
public:
	enum class Anim
	{
		Idle,
		Run,
		Attack,
>>>>>>> navMesh
		Length,
	};

private:
<<<<<<< HEAD
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

=======
	//std::vector<Animation3D*> m_attacjAnimList;	// アニメーションリスト
	//static constexpr int AnimNum = static_cast<int>(Anim::Length);	// アニメーションの数

	static constexpr float MoveSpeed = 5.0f;	// 移動速度
	//static const char* AnimList[AnimNum];	// アニメーションを指定する用

	int m_model;
	int m_animIndex;	// 再生中のアニメーション
	NavMesh* m_navMesh;	// 経路探索用
	LoadPlayer* m_player;	// プレイヤー

	bool m_isSet;
>>>>>>> navMesh

protected:
	virtual void Update() override;
	virtual void Draw() override;
<<<<<<< HEAD

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
=======
	virtual void OnCollision(const Actor3D* other) override;

public:
	Enemy(NavMesh* navMesh, const Vector3& pos, LoadPlayer* loadPlayer);
};
>>>>>>> navMesh
