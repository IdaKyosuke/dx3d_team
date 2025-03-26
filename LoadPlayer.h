#pragma once
#include"Actor.h"
#include"Vector3.h"
#include"DxLib.h"
#include<vector>

class Camera;
class Animation3D;
class Collision3D;

class LoadPlayer : public Actor
{
public:
	// ���[�v����A�j���[�V����
	enum class RoopAnim
	{
		Idle,
		Walk,
		Run,
		Jump,
		JumpUp,
		JumpIdle,
		Landing,
		Floating,

		Length,
	};

	// ���[�v���Ȃ��A�j���[�V����
	enum class NoRoopAnim
	{

	};

	// ���݂̈ړ����[�h
	enum MoveMode
	{
		Normal,
		ZeroGravity,
	};

private:
	std::vector<Animation3D*> m_attachAnimList;
	
	static constexpr int RoopAnimNum = static_cast<int>(RoopAnim::Length);	// �A�j���[�V�����̐�

	static constexpr float WalkSpeed = 4.0f;	// �������x
	static constexpr float RunSpeed = 9.0f;	// ���鑬�x
	static constexpr float RotateSpeed = 10.0f;	// ��]���x
	static constexpr float DurationTime = 1.0f;
	static constexpr float JumpPower = 4.0f;	// �����x
	static constexpr float Gravity = 9.8f;		// �d�͉����x
	static constexpr Vector3 SpawnPos = Vector3(950, 40, 90);	// �X�e�[�W�ɃX�|�[������ꏊ
	static constexpr float RestartHeight = 3000.0f;	// ���X�|�[�����鍂���̐�Βl
	static constexpr float FloatHeight = 50.0f;		// ���d�͏�Ԃ̍���

	Vector3 AxisY = Vector3(0.0f, 1.0f, 0.0f);	// ��]��(Y���ŏ����)


	static const char* AnimList[RoopAnimNum];

	int m_model;	// �v���C���[�̃��f��(mv1)

	int m_animIndex;	// �Đ����̃A�j���[�V����

	int m_lightHandle;	// �������쐬����
	float m_angle;
	bool m_isJump;
	bool m_isJumping;
	bool m_isFall;
	bool m_isFloating;
	bool m_isTerning;	// �d�͂����]���Ă��邩�ǂ���

	Camera* m_camNode;
	Collision3D* m_collision3D;

	Vector3 m_playerPos;
	Vector3 m_playerPastPos;
	Vector3 m_playerRotate;
	Vector3 m_moveDirection;


	// �A�j���[�V�����؂�ւ��p
	RoopAnim m_nowAnim;
	RoopAnim m_nextAnim;


	// �ړ����[�h�؂�ւ��p
	MoveMode m_moveMode;

	bool m_moving;	// �ړ��A�j���[�V�������Đ�����
	float m_elapsedTime;	// �o�ߎ��ԕێ�
	float m_fallTime;		// ���R����������������p
	// �v���C���[�̃W�����v����
	void Jumping();		// �����ŃW�����v���鏈��

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	LoadPlayer(Collision3D* collision3D);

	// �A�j���[�V������؂�ւ���(Lerp)
	void ChangeAnimLerp();

	// �A�j���[�V������؂�ւ���(����)
	void ChangeAnimQuick(const RoopAnim nextAnim);

	// �A�j���[�V�������Đ�����
	void PlayAnim();

	// ���f���֌W���폜
	void Finalize();

	// �v���C���[�̒ʏ�ړ�
	void NormalMove();
	// ���d�͏�Ԃ̈ړ�
	void ZeroGravityMove();

	// �ړ����[�h�̐؂�ւ�
	void ChangeMoveMode();

	// �v���C���[�̈ʒu��Ԃ�
	Vector3 PlayerPos()
	{
		return m_playerPos;
	}

	bool IsJump()
	{
		return m_isJump;
	}

	bool IsTerning()
	{
		return m_isTerning;
	}
};