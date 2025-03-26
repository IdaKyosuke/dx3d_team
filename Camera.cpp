#include"Camera.h"
#include"Math.h"
#include"LoadPlayer.h"
#include"Input.h"
#include"Screen.h"

Camera::Camera(LoadPlayer* player) :
	Actor("PlayerCam"),
	m_camPos(StartPos),
	m_camTarget(player->PlayerPos()),
	m_loadPlayerNode(player),
	m_pastTerning(false),
	m_diffY(DiffY)
{
	Vector3 playerPos = Vector3(m_loadPlayerNode->PlayerPos());
	m_camTarget = Vector3(playerPos.x, m_camTarget.y, playerPos.z);
	// �v���C���[�̈ړ���
	m_pastPlayerPos = player->PlayerPos();
};

// �J�����̏ꏊ�Əœ_��ݒ�
void Camera::SetCamPosAndTag()
{
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
	// 3�l�̎��_
	if (Input::GetInstance()->GetMousePoint().x != Screen::Center.x)
	{
		// �}�E�X�𓮂�����
		float diffX = (Input::GetInstance()->GetMousePoint().x - Screen::Center.x) / 10;
		m_camPos = Math::PointRotate(m_camPos, m_loadPlayerNode->PlayerPos(), DX_PI_F / CamRot * diffX);
		// �v���C���[�ƈ�苗����ۂ�
		m_camPos = m_camTarget - CamFrontPlaneVec() * CamDiff;
	}
	else if (m_pastPlayerPos != playerPos)
	{
		// �}�E�X�̈ړ��ʂ��Ȃ��Ƃ��̓v���C���[�Ɠ�������������
		m_camPos += playerPos - m_pastPlayerPos;
	}

	// 1�l�̎��_
	

	m_camTarget = Vector3(playerPos.x, m_camPos.y, playerPos.z);
	// �J�����̍����͏�Ƀv���C���[�̈ړ��ʂƓ����ɂ���
	if (m_pastPlayerPos != playerPos)
	{
		
	}
	m_camPos.y = m_camTarget.y - CamFrontPlaneVec().y * CamDiff;
	m_pastPlayerPos = playerPos;

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

	// �J�����̃X�N���[�����W�𒆐S�ɖ߂�
	Input::GetInstance()->SetMousePoint();
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
#endif // _DEBUG

}