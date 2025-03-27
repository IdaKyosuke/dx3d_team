#include"Camera.h"
#include"Math.h"
#include"LoadPlayer.h"
#include"Input.h"
#include"Screen.h"

Camera::Camera(LoadPlayer* player) :
	Actor("PlayerCam"),
	m_camPos(StartPos),
	m_loadPlayerNode(player),
	m_pastTerning(false),
	m_diffY(DiffY),
	m_sightMode(SightMode::First)
{
	Vector3 playerPos = Vector3(m_loadPlayerNode->PlayerPos());
	switch(m_sightMode)
	{
	case SightMode::First:
		// ���g�̐��ʂ������_�ɂȂ�
		m_camTarget = Vector3(m_camPos.x * 2, m_camPos.y, m_camPos.z * 2);
		break;

	case SightMode::Third:
		// �v���C���[�������_�ɂȂ�
		m_camTarget = Vector3(playerPos.x, m_camTarget.y, playerPos.z);
		break;
	}
	// �v���C���[�̈ړ���
	m_pastPlayerPos = player->PlayerPos();
};

// �J�����̏ꏊ�Əœ_��ݒ�
void Camera::SetCamPosAndTag()
{
	switch (m_sightMode)
	{
	case SightMode::First:
		SetCameraPositionAndTargetAndUpVec(m_camPos, m_camTarget, Vector3(0, 1, 0));
		break;

	case SightMode::Third:
		if (Input::GetInstance()->IsKeyPress(KEY_INPUT_UP))
		{
			// �J�����̈ړ��i���j
			float y = m_camTarget.y + DiffTagY;
			SetCameraPositionAndTargetAndUpVec(m_camPos, Vector3(m_camTarget.x, y, m_camTarget.z), Vector3(0, 1, 0));
		}
		else if (Input::GetInstance()->IsKeyPress(KEY_INPUT_DOWN))
		{
			// �J�����̈ړ��i���j
			float y = m_camTarget.y - DiffTagY;
			SetCameraPositionAndTargetAndUpVec(m_camPos, Vector3(m_camTarget.x, y, m_camTarget.z), Vector3(0, 1, 0));
		}
		else
		{
			// �ʏ�J����
			SetCameraPositionAndTargetAndUpVec(m_camPos, m_camTarget, Vector3(0, 1, 0));
		}
		break;
	}
	
}

// �J�����̐��ʃx�N�g�����擾����(XYZ)
Vector3 Camera::CamFrontVec()
{
	Vector3 rot = Math::Normalized((m_camTarget - m_camPos));
	return rot;
}

// �J�����̐��ʃx�N�g�����擾(XZ����)
Vector3 Camera::CamFrontPlaneVec()
{
	Vector3 camRight = CamRight();
	Vector3 camUp = Vector3(0, -1.0f, 0);
	return Math::Normalized(camUp.CrossP(camRight));
}

// �J�����̉E�����x�N�g�����擾
Vector3 Camera::CamRight()
{
	// �J�����̐��ʂ��擾
	Vector3 camFront = CamFrontVec();
	// �J�����̏�x�N�g�����擾
	Vector3 camUp = Vector3(0, -1.0f, 0);
	// 2�̊O�ς����߂�
	Vector3 camRight = camFront.CrossP(camUp);
	// ���K�����ĕԂ�
	return Math::Normalized(camRight);
}

// �v���C���[��ǂ�������
void Camera::ChasePlayer(const Vector3& playerPos)
{
	switch (m_sightMode)
	{
	case SightMode::First:
		// 1�l�̎��_
		FirstPerson(playerPos);
		break;

	case SightMode::Third:
		// 3�l�̎��_
		ThirdPerson(playerPos);
		break;
	}
	
	// �J�����̍����͏�Ƀv���C���[�̈ړ��ʂƓ����ɂ���
	if (m_pastPlayerPos != playerPos)
	{
		m_camPos.y = playerPos.y + DiffY;
	}
	m_pastPlayerPos = playerPos;

	// �J�����̃X�N���[�����W�𒆐S�ɖ߂�
	Input::GetInstance()->SetMousePoint();
}


// 3�l�̎��_
void Camera::ThirdPerson(const Vector3& playerPos)
{
	// 3�l�̎��_
	if (Input::GetInstance()->GetMousePoint().x != Screen::Center.x)
	{
		// �}�E�X�𓮂�����
		float diffX = (Input::GetInstance()->GetMousePoint().x - Screen::Center.x) / DecMouseDiff;
		m_camPos = Math::PointRotate(m_camPos, m_loadPlayerNode->PlayerPos(), DX_PI_F / CamRot * diffX);
		// �v���C���[�ƈ�苗����ۂ�
		Vector3 pos = Vector3(m_camTarget - CamFrontPlaneVec() * CamDiff);
		m_camPos = Vector3(pos.x, playerPos.y + DiffY, pos.z);
	}
	else if (m_pastPlayerPos != playerPos)
	{
		// �}�E�X�̈ړ��ʂ��Ȃ��Ƃ��̓v���C���[�Ɠ�������������
		m_camPos += playerPos - m_pastPlayerPos;
	}
	m_camTarget = Vector3(playerPos.x, m_camPos.y, playerPos.z);
}

// 1�l�̎��_
void Camera::FirstPerson(const Vector3& playerPos)
{
	if (Input::GetInstance()->GetMousePoint().x != Screen::Center.x)
	{
		// �}�E�X�𓮂����ƃJ�����̒����_������
		float diffX = (Input::GetInstance()->GetMousePoint().x - Screen::Center.x) / DecMouseDiff;
		m_camTarget = Math::PointRotate(m_camTarget, playerPos, DX_PI_F / CamRot * diffX);
		// �J�����ƒ����_����苗���ɕۂ�
		Vector3 pos = Vector3(playerPos + CamFrontPlaneVec() * CamDiff);
		m_camTarget = Vector3(pos.x, playerPos.y + DiffY, pos.z);
	}
	else
	{
		// �}�E�X�̈ړ��ʂ��Ȃ��Ƃ��̓v���C���[�Ɠ�������������
		m_camTarget += playerPos - m_pastPlayerPos;
	}
	// �J�������v���C���[�ƈꏏ�Ɉړ�����
	m_camPos = Vector3(playerPos.x, playerPos.y + DiffY, playerPos.z);
}

// ���d�͗p�J����
void Camera::AntiGravity(const Vector3& playerPos)
{
	// �d�͂̌������ς������p�x��ς��Ă���
	if (m_loadPlayerNode->IsTerning() != m_pastTerning)
	{
		// ��������p�x�����Z�b�g
		m_diffY = 0;
	}

	if (m_diffY < DiffY)
	{
		m_diffY += AddAngleY;
		if (m_diffY >= DiffY)
		{
			// �l�ɏ����݂���
			m_diffY = DiffY;
		}
	}

	// �d�͂̌����ɂ���Č��グ�邩�����낷����ς���
	if (!m_loadPlayerNode->IsTerning())
	{
		m_camPos.y = playerPos.y + m_diffY;
	}
	else
	{
		m_camPos.y = playerPos.y - m_diffY;
	}

	// 1�t���[���O�̏d�͂̏�Ԃ�ۑ�
	m_pastTerning = m_loadPlayerNode->IsTerning();
}

void Camera::Update()
{	
	// �v���C���[�̕�������
	ChasePlayer(m_loadPlayerNode->PlayerPos());
}

void Camera::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 0, GetColor(255, 255, 255), 
		"CamTarget Vector3(%.0f, %.0f, %.0f)",
		m_camTarget.x, m_camTarget.y, m_camTarget.z);

	DrawFormatString(0, 20, GetColor(255, 255, 255),
		"CamPos Vector3(%.0f, %.0f, %.0f)",
		m_camPos.x, m_camPos.y, m_camPos.z);

	DrawFormatString(0, 120, GetColor(255, 255, 255),
		"CamAngle :%.0f",
		Math::ACos(CamFrontVec(), CamFrontPlaneVec()));

	DrawFormatString(0, 140, GetColor(255, 255, 255),
		"CamFrontVec Vector3(%.0f, %.0f, %.0f)",
		CamFrontVec().x, CamFrontVec().y, CamFrontVec().z);
#endif // _DEBUG

}