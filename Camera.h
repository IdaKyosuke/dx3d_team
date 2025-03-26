#pragma once
#include"Actor.h"
#include"Vector3.h"

class LoadPlayer;

class Camera : public Actor
{
private:
	static constexpr Vector3 StartPos = Vector3(1300, 100, 300);
	static constexpr float DiffY = 150;
	static constexpr float CamDiff = 400.0f;	// �v���C���[�ƃJ�����̋���
	static constexpr float DiffTagY = 200.0f;	// ����������̍���
	static constexpr float AddAngleY = 2.0f;	// y���̍��������Z���Ă������߂̒l
	static constexpr float CamRot = 720.0f;	// �}�E�X�̈ړ��ʂɑ΂���J�����̉�]�̕���

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

};