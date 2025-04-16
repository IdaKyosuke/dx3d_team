#pragma once
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
		Length,
	};

private:
	std::vector<Animation3D*> m_attachAnimList;

	static constexpr int RoopAnimNum = static_cast<int>(Anim::Length);	// アニメーションの数
	static constexpr float MoveSpeed = 2.0f;	// 移動速度
	static const char* AnimList[RoopAnimNum];
	int m_model;	// 敵のモデル
	int m_animIndex;	// 再生中のアニメーション
	Vector3 m_enemyPos;
	Vector3 m_enemyPastPos;

	// アニメーション切り替え用
	Anim m_nowAnim;
	Anim m_nextAnim;

	// モデルの回転用
	Vector3 m_moveDirection;

	//static const char* AnimList[AnimNum];	// アニメーションを指定する用
	NavMesh* m_navMesh;	// 経路探索用
	LoadPlayer* m_player;	// プレイヤー

	bool m_isSet;

protected:
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnCollision(const Actor3D* other) override;

public:
	Enemy(NavMesh* navMesh, const Vector3& pos, LoadPlayer* loadPlayer);

	// アニメーションを切り替える(Lerp)
	void ChangeAnimLerp();

	// アニメーションを切り替える(即座)
	void ChangeAnimQuick(const Anim nextAnim);

	// アニメーションを再生する
	void PlayAnim();

	// モデル関係を削除
	void Finalize();

	// 敵の移動
	void EnemyMove();

	// 敵の位置を返す
	Vector3 EnemyPos()
	{
		return m_enemyPos;
	}
};
