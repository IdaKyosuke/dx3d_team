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

	static constexpr int RoopAnimNum = static_cast<int>(Anim::Length);	// �A�j���[�V�����̐�
	static constexpr float MoveSpeed = 2.0f;	// �ړ����x
	static const char* AnimList[RoopAnimNum];
	int m_model;	// �G�̃��f��
	int m_animIndex;	// �Đ����̃A�j���[�V����
	Vector3 m_enemyPos;
	Vector3 m_enemyPastPos;

	// �A�j���[�V�����؂�ւ��p
	Anim m_nowAnim;
	Anim m_nextAnim;

	// ���f���̉�]�p
	Vector3 m_moveDirection;

	//static const char* AnimList[AnimNum];	// �A�j���[�V�������w�肷��p
	NavMesh* m_navMesh;	// �o�H�T���p
	LoadPlayer* m_player;	// �v���C���[

	bool m_isSet;

protected:
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnCollision(const Actor3D* other) override;

public:
	Enemy(NavMesh* navMesh, const Vector3& pos, LoadPlayer* loadPlayer);

	// �A�j���[�V������؂�ւ���(Lerp)
	void ChangeAnimLerp();

	// �A�j���[�V������؂�ւ���(����)
	void ChangeAnimQuick(const Anim nextAnim);

	// �A�j���[�V�������Đ�����
	void PlayAnim();

	// ���f���֌W���폜
	void Finalize();

	// �G�̈ړ�
	void EnemyMove();

	// �G�̈ʒu��Ԃ�
	Vector3 EnemyPos()
	{
		return m_enemyPos;
	}
};
