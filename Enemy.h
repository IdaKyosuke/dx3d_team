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
	// ���[�v����A�j���[�V����
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

	static constexpr int RoopAnimNum = static_cast<int>(RoopAnim::Length);	// �A�j���[�V�����̐�

	static constexpr float WalkSpeed = 4.0f;	// �������x
	static constexpr Vector3 SpawnPos = Vector3(0, 20, 0);	// �X�e�[�W�ɃX�|�[������ꏊ

	static const char* AnimList[RoopAnimNum];

	int m_model;	// �G�̃��f��

	int m_animIndex;	// �Đ����̃A�j���[�V����

	int m_lightHandle;	// �������쐬����

	Collision3D* m_collision3D;

	Vector3 m_enemyPos;
	Vector3 m_enemyPastPos;

	// �A�j���[�V�����؂�ւ��p
	RoopAnim m_nowAnim;
	RoopAnim m_nextAnim;

=======
	//std::vector<Animation3D*> m_attacjAnimList;	// �A�j���[�V�������X�g
	//static constexpr int AnimNum = static_cast<int>(Anim::Length);	// �A�j���[�V�����̐�

	static constexpr float MoveSpeed = 5.0f;	// �ړ����x
	//static const char* AnimList[AnimNum];	// �A�j���[�V�������w�肷��p

	int m_model;
	int m_animIndex;	// �Đ����̃A�j���[�V����
	NavMesh* m_navMesh;	// �o�H�T���p
	LoadPlayer* m_player;	// �v���C���[

	bool m_isSet;
>>>>>>> navMesh

protected:
	virtual void Update() override;
	virtual void Draw() override;
<<<<<<< HEAD

public:
	Enemy(Collision3D* collision3D);

	// �A�j���[�V������؂�ւ���(Lerp)
	void ChangeAnimLerp();

	// �A�j���[�V������؂�ւ���(����)
	void ChangeAnimQuick(const RoopAnim nextAnim);

	// �A�j���[�V�������Đ�����
	void PlayAnim();

	// ���f���֌W���폜
	void Finalize();

	// �G�̈ʒu��Ԃ�
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
