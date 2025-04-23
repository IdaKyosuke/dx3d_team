#include"Camera.h"
#include"Math.h"
#include"LoadPlayer.h"
#include"Input.h"
#include"Screen.h"

Camera::Camera(LoadPlayer* player) :
	Actor3D("PlayerCam"),
	m_loadPlayerNode(player),
	m_pastTerning(false),
	m_diffY(DiffY),
	m_sightMode(SightMode::First),
	m_isLocked(true)
{
	Vector3 playerPos = Vector3(m_loadPlayerNode->GetPosition());

	switch(m_sightMode)
	{
	case SightMode::First:
		// �J�����̈ʒu���v���C���[�Ɠ����ɂ���
		m_transform.position = playerPos;
		// ���g�̐��ʂ������_�ɂȂ�
		m_camTarget = Vector3(m_transform.position.x * 2, m_transform.position.y + DiffY, m_transform.position.z * 2);
		break;

	case SightMode::Third:
		m_transform.position = StartPos;
		// �v���C���[�������_�ɂȂ�
		m_camTarget = Vector3(playerPos.x, m_camTarget.y, playerPos.z);
		break;
	}
	// �v���C���[�̈ړ���
	m_pastPlayerPos = player->GetPosition();
};

// �J�����̏ꏊ�Əœ_��ݒ�
void Camera::SetCamPosAndTag()
{
	switch (m_sightMode)
	{
	case SightMode::First:
		SetCameraPositionAndTargetAndUpVec(m_transform.position, m_camTarget, Vector3(0, 1, 0));
		break;

	case SightMode::Third:
		if (Input::GetInstance()->IsKeyPress(KEY_INPUT_UP))
		{
			// �J�����̈ړ��i���j
			float y = m_camTarget.y + DiffTagY;
			SetCameraPositionAndTargetAndUpVec(m_transform.position, Vector3(m_camTarget.x, y, m_camTarget.z), Vector3(0, 1, 0));
		}
		else if (Input::GetInstance()->IsKeyPress(KEY_INPUT_DOWN))
		{
			// �J�����̈ړ��i���j
			float y = m_camTarget.y - DiffTagY;
			SetCameraPositionAndTargetAndUpVec(m_transform.position, Vector3(m_camTarget.x, y, m_camTarget.z), Vector3(0, 1, 0));
		}
		else
		{
			// �ʏ�J����
			SetCameraPositionAndTargetAndUpVec(m_transform.position, m_camTarget, Vector3(0, 1, 0));
		}
		break;
	}

}

// �J�����̐��ʃx�N�g�����擾����(XYZ)
Vector3 Camera::CamFrontVec()
{
	Vector3 rot = Math::Normalized((m_camTarget - m_transform.position));
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

// �J�����𓮂���
void Camera::MoveCam(const Vector3& playerPos)
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
		m_transform.position.y = playerPos.y + DiffY;
	}
	m_pastPlayerPos = playerPos;

	// �J�����̃X�N���[�����W�𒆐S�ɖ߂�
	if (m_isLocked)
	{
		Input::GetInstance()->SetMousePoint();
	}
}


// 3�l�̎��_
void Camera::ThirdPerson(const Vector3& playerPos)
{
	// 3�l�̎��_
	if (Input::GetInstance()->GetMousePoint().x != Screen::Center.x)
	{
		// �}�E�X�𓮂�����
		float diffX = (Input::GetInstance()->GetMousePoint().x - Screen::Center.x) / DecMouseDiff;
		m_transform.position = Math::PointRotate(m_transform.position, m_loadPlayerNode->GetPosition(), DX_PI_F / CamRot * diffX);
		// �v���C���[�ƈ�苗����ۂ�
		Vector3 pos = Vector3(m_camTarget - CamFrontPlaneVec() * CamDiff);
		m_transform.position = Vector3(pos.x, playerPos.y + DiffY, pos.z);
	}
	else if (m_pastPlayerPos != playerPos)
	{
		// �}�E�X�̈ړ��ʂ��Ȃ��Ƃ��̓v���C���[�Ɠ�������������
		m_transform.position += playerPos - m_pastPlayerPos;
	}
	m_camTarget = Vector3(playerPos.x, m_transform.position.y, playerPos.z);
}

// 1�l�̎��_
void Camera::FirstPerson(const Vector3& playerPos)
{
	if (Input::GetInstance()->GetMousePoint().x != Screen::Center.x)
	{
		// �}�E�X�𓮂����ƃJ�����̒����_������(���E)
		float diffX = (Input::GetInstance()->GetMousePoint().x - Screen::Center.x) / DecMouseDiff;
		Vector3 rotPos = Math::PointRotate(m_camTarget, playerPos, DX_PI_F / CamRot * diffX);
		m_camTarget = Vector3(rotPos.x, m_camTarget.y, rotPos.z);
		// �J�����ƒ����_����苗���ɕۂ�
		Vector3 pos = Vector3(playerPos + CamFrontPlaneVec() * CamDiff);
		// �v���C���[�̏㉺�ړ������������͂��̕������Z����
		if (playerPos.y != m_pastPlayerPos.y)
		{
			m_camTarget.y += playerPos.y - m_pastPlayerPos.y;
		}
		m_camTarget = Vector3(pos.x, m_camTarget.y, pos.z);
	}
	else
	{
		// �}�E�X�̈ړ��ʂ��Ȃ��Ƃ��̓v���C���[�Ɠ�������������
		m_camTarget += playerPos - m_pastPlayerPos;
	}

	// ���_�̏㉺�ړ�
	if (Input::GetInstance()->GetMousePoint().y != Screen::Center.y)
	{
		// �}�E�X��y���W�̈ړ��ʂ��擾
		float diffY = (Input::GetInstance()->GetMousePoint().y - Screen::Center.y) / DecMouseDiff;
		m_camTarget.y += 5 * -diffY;
		// �}�E�X�̎��_�̏������������
		if (m_camTarget.y >= playerPos.y + MaxCamHeight)
		{
			// ���
			m_camTarget.y = playerPos.y + MaxCamHeight;
		}
		else if (m_camTarget.y <= playerPos.y + MinCamHeight)
		{
			// ����
			m_camTarget.y = playerPos.y + MinCamHeight;
		}
	}

	// �J�������v���C���[�ƈꏏ�Ɉړ�����
	m_transform.position = Vector3(playerPos.x, playerPos.y + DiffY, playerPos.z);
}

void Camera::Update()
{
	// �J�����𓮂���
	MoveCam(m_loadPlayerNode->GetPosition());

	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_TAB))
	{
		m_isLocked = !m_isLocked;
	}
}

void Camera::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 0, GetColor(255, 255, 255),
		"CamTarget Vector3(%.0f, %.0f, %.0f)",
		m_camTarget.x, m_camTarget.y, m_camTarget.z);

	DrawFormatString(0, 20, GetColor(255, 255, 255),
		"CamPos Vector3(%.0f, %.0f, %.0f)",
		m_transform.position.x, m_transform.position.y, m_transform.position.z);
#endif // _DEBUG

}