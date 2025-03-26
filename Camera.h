#pragma once
#include"Actor.h"
#include"Vector3.h"

class LoadPlayer;

class Camera : public Actor
{
private:
	static constexpr Vector3 StartPos = Vector3(1300, 200, 300);
	static constexpr float DiffY = 150;
	static constexpr float CamDiff = 400.0f;	// �v���C���[�ƃJ�����̋���
	static constexpr float DiffTagY = 200.0f;	// ����������̍���
	static constexpr float AddAngleY = 2.0f;	// y���̍��������Z���Ă������߂̒l
	static constexpr float DecMouseDiff = 10.0f;	// �}�E�X�̈ړ��ʂ���]�Ɏg�����߂ɁA�l�����炷�p
	static constexpr float DivAngle = 720.0f;	// �}�E�X�̈ړ��ʂɑ΂���J�����̉�]�ʂ����߂�

	Vector3 m_camPos;
	Vector3 m_camTarget;
	Vector3 m_pastPlayerPos;

	LoadPlayer* m_loadPlayerNode;
	bool m_pastTerning;		// 1�t���[���O�̏d�͂̌���
	float m_diffY;		// �J�����p�x���������ς���悤

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	Camera(LoadPlayer* player);

	// �J�����̏ꏊ�Əœ_��ݒ�
	void SetCamPosAndTag();

	// �J�����̐��ʃx�N�g�����擾����(XYZ)
	Vector3 CamFrontVec();

	// �J�����̐��ʃx�N�g�����擾(XZ����)
	Vector3 CamFrontPlaneVec();

	// �J�����̉E�����x�N�g�����擾
	Vector3 CamRight();

	// �v���C���[�̕�������
	void ChasePlayer(const Vector3& playerPos);

	// �v���C���[�𒆐S�ɉ�]����3�l�̎��_�J����
	void ThirdPersonCam(const Vector3& playerPos);

	// �v���C���[�ƈꏏ�Ɉړ�����1�l�̃J����
	void FirstPersonCam(const Vector3& playerPos);

	// ���d�͂���������ꍇ�̃J����
	void AntiGravityCam(const Vector3& playerPos);
};