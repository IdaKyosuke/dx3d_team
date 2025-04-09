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
	//std::vector<Animation3D*> m_attacjAnimList;	// �A�j���[�V�������X�g
	//static constexpr int AnimNum = static_cast<int>(Anim::Length);	// �A�j���[�V�����̐�

	static constexpr float MoveSpeed = 5.0f;	// �ړ����x
	//static const char* AnimList[AnimNum];	// �A�j���[�V�������w�肷��p

	int m_model;
	int m_animIndex;	// �Đ����̃A�j���[�V����
	NavMesh* m_navMesh;	// �o�H�T���p
	LoadPlayer* m_player;	// �v���C���[

protected:
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnCollision(const Actor3D* other) override;

public:
	Enemy(NavMesh* navMesh, const Vector3& pos, LoadPlayer* loadPlayer);
};