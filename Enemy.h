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
	//std::vector<Animation3D*> m_attacjAnimList;	// アニメーションリスト
	//static constexpr int AnimNum = static_cast<int>(Anim::Length);	// アニメーションの数

	static constexpr float MoveSpeed = 5.0f;	// 移動速度
	//static const char* AnimList[AnimNum];	// アニメーションを指定する用

	int m_model;
	int m_animIndex;	// 再生中のアニメーション
	NavMesh* m_navMesh;	// 経路探索用
	LoadPlayer* m_player;	// プレイヤー

protected:
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnCollision(const Actor3D* other) override;

public:
	Enemy(NavMesh* navMesh, const Vector3& pos, LoadPlayer* loadPlayer);
};