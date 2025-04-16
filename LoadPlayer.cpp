#include"LoadPlayer.h"
#include"Time.h"
#include"Math.h"
#include"Camera.h"
#include"Animation3D.h"
#include"Input.h"
#include"Lerp.h"
#include"Quaternion.h"
#include"CollisionStage.h"
#include"BoxCollider3D.h"
#include<math.h>

// �A�j���[�V�������X�g
const char* LoadPlayer::AnimList[RoopAnimNum] =
{
	"Man/Idle_stand.mv1",
	"Man/Walking.mv1",
	"Man/Sprint.mv1",
	"Man/Jumping_trimed.mv1",
	"Man/jumpUp.mv1",
	"Man/fall.mv1",
	"Man/landing.mv1",
	"Man/Floating.mv1",
};

LoadPlayer::LoadPlayer(CollisionStage* collisionStage) :
	Actor3D("Player", SpawnPos),
	m_model(MV1LoadModel("Resource/Man/Man.mv1")),
	m_animIndex(0),
	m_moving(false),
	m_nowAnim(RoopAnim::Idle),
	m_nextAnim(RoopAnim::Idle),
	m_moveMode(MoveMode::Normal),
	m_moveDirection(Vector3(0, 0, 0)),
	m_camNode(nullptr),
	m_collisionStage(collisionStage),
	m_elapsedTime(0),
	m_fallTime(0),
	m_isJump(false),
	m_isJumping(false),
	m_isFall(false),
	m_isFloating(false),
	m_isTerning(false),
	m_hit(false)
{
	//-----�A�j���[�V�����̍쐬-----
	// �A�j���[�V�����N���X�����X�g������
	for (int i = 0; i < RoopAnimNum; i++)
	{
		m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i]));
		AddChild(m_attachAnimList[i]);
	}

	// �A�j���[�V�����p�̈ʒu
	m_playerPastPos = m_transform.position;

	// �v���C���[�̉�](x, y, z)
	m_playerRotate = Vector3(0, 0, 0);
	m_transform.angle = Vector3(0, 180, 0);

	// �J�����̐���
	m_camNode = new Camera(this);
	AddChild(m_camNode);

	// �f�B���N�V���i�����C�g���쐬
	m_lightHandle = CreateDirLightHandle(Vector3(0, 100.0f, 0));

	// �ŏ��̃A�j���[�V�������w��
	m_attachAnimList[static_cast<int>(RoopAnim::Idle)]->FadeIn();

	m_collider = new BoxCollider3D(ColSize, ColOffset);
}

// �A�j���[�V������؂�ւ���(Lerp)
void LoadPlayer::ChangeAnimLerp()
{
	if (m_nowAnim == m_nextAnim) return;

	m_attachAnimList[static_cast<int>(m_nowAnim)]->FadeOut();
	m_attachAnimList[static_cast<int>(m_nextAnim)]->FadeIn();

	m_nowAnim = m_nextAnim;
}

// �A�j���[�V������؂�ւ���(����)
void LoadPlayer::ChangeAnimQuick(const RoopAnim nextAnim)
{
	m_attachAnimList[static_cast<int>(m_nowAnim)]->ChangeOut();
	m_attachAnimList[static_cast<int>(nextAnim)]->ChangeIn();

	m_nowAnim = nextAnim;
	m_nextAnim = nextAnim;
}

// �A�j���[�V�������Đ�����
void LoadPlayer::PlayAnim()
{
	// �J�����̈ʒu�ƌ���
	m_camNode->SetCamPosAndTag();

	// ���f���̉�]
	if (!m_isTerning)
	{
		Quaternion::RotateAxisY(m_model, m_transform.angle.y, m_transform.position);
	}
	else
	{
		Quaternion::RotateAxisYZ(m_model, m_transform.angle.y, m_transform.position);
	}

	// ���f���̕`��
	MV1DrawModel(m_model);
	
	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"PlayerPos Vector3(%.0f, %.0f, %.0f)",
		m_transform.position.x, m_transform.position.y, m_transform.position.z
	);
	
}

// ���f���֌W���폜
void LoadPlayer::Finalize()
{
	// ���C�g�n���h�����폜
	DeleteLightHandle(m_lightHandle);
	// �v���C���[�̃��f�����폜
	MV1DeleteModel(m_model);
}

void LoadPlayer::Update()
{
	// ������, �V�䓞�B -> ���X�|�[�� or press "r" => ���X�^�[�g
	if (m_transform.position.y <= -RestartHeight ||
		m_transform.position.y >= RestartHeight ||
		Input::GetInstance()->IsKeyDown(KEY_INPUT_R)
		)
	{
		m_transform.position = SpawnPos;
		m_isTerning = false;
	}
	/*

	// �ړ���Ԃ̐؂�ւ�
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_Q))
	{
		ChangeMoveMode();
	}*/

	// �v���C���[�̈ړ�
	switch (m_moveMode)
	{
	case MoveMode::Normal:
		NormalMove();
		break;

	case MoveMode::ZeroGravity:
		ZeroGravityMove();
		break;
	}

	// �A�j���[�V�����̐؂�ւ�
	ChangeAnimLerp();

	// 1�t���[���O�̈ʒu���X�V
	m_playerPastPos = m_transform.position;
}

// �W�����v����
void LoadPlayer::Jumping()
{
	m_elapsedTime += Time::GetInstance()->GetDeltaTime();
	if (!m_isTerning)
	{
		m_transform.position.y += JumpPower - Gravity * m_elapsedTime;
	}
	else
	{
		m_transform.position.y += -JumpPower + Gravity * m_elapsedTime;
	}

	// ���������ɐG�ꂽ��
	if (m_isJumping && m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag)
	{
		m_isJump = false;
		m_isFall = false;
		m_isJumping = false;
		m_nextAnim = RoopAnim::Landing;
	}
	else
	{
		m_isJumping = true;
	}
}

// �v���C���[�̒ʏ�ړ�
void LoadPlayer::NormalMove()
{
	// ----�v���C���[�̈ړ�----
	float inputX = 0;
	float inputZ = 0;
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_S)) inputZ = -1;
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_W)) inputZ = 1;
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_A)) inputX = -1;
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_D)) inputX = 1;

	// �d�͂̌������m�F
	float dir = m_isTerning ? -1.0f : 1.0f;
	// �ړ�����������i�d�͂ɉ����č��E�ړ��̌������ς��j
	m_moveDirection = m_camNode->CamFrontPlaneVec() * inputZ + m_camNode->CamRight() * inputX;

	// �d�͂����Z�i�W�����v���łȂ� && ���n���Ă��Ȃ��Ƃ��̂݁j
	if (!m_isJump && m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag == 0 && !m_isJump)
	{
		m_fallTime += Time::GetInstance()->GetDeltaTime();
		if (!m_isTerning)
		{
			// �ʏ펞
			m_transform.position.y -= Gravity * m_fallTime;
		}
		else
		{
			m_transform.position.y += Gravity * m_fallTime;
		}
	}
	else if (m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag == 1)
	{
		// �v���C���[�̍����𑫏�̍����ɍ��킹��
		m_fallTime = 0;
		if (!m_isTerning)
		{
			// �ʏ펞
			m_transform.position.y = m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitPosition.y;
		}
		else
		{
			// �d�͔��]��
			m_transform.position.y = m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitPosition.y;
		}
	}

	// �ړ�
	m_transform.position += Math::Normalized(m_moveDirection) * (Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT) ? RunSpeed : WalkSpeed);

	if (m_collisionStage->CapsuleCollider(m_transform.position, m_isTerning) != 0)
	{
		// �i�ޗ\���ɕǂ���������
		m_transform.position.x = m_playerPastPos.x;
		m_transform.position.z = m_playerPastPos.z;
	}

	// �W�����v
	if (!m_isJump && m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag != 0 && Input::GetInstance()->IsKeyDown(KEY_INPUT_SPACE))
	{
		m_nextAnim = RoopAnim::JumpUp;
		m_isJump = true;
		m_elapsedTime = 0;
	}
	else if (m_isJump)
	{
		Jumping();
	}

	// �v���C���[�̉�]�Ō��݂̌����Ɖ�]�\��̌����̕������Ⴄ�Ƃ��ɕ��������킹��i180 �` -180�ł������Ȃ����߁j
	if (!m_moveDirection.IsZero())
	{
		float afterAngle = 0;

		Math::MatchAngleSign(afterAngle, m_moveDirection, m_transform.angle);

		m_transform.angle.y = Lerp::Exec(m_transform.angle.y, afterAngle, 0.2f);
	}

	// ---- �ړ��A�j���[�V���� ----
	if (m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag == 0)
	{
		m_nextAnim = RoopAnim::JumpIdle;
	}
	else
	{
		if (!m_isJump)
		{
			if (m_playerPastPos.x != m_transform.position.x ||
				m_playerPastPos.z != m_transform.position.z)
			{
				if (!Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT))
				{
					// walk
					m_nextAnim = RoopAnim::Walk;
					m_moving = true;
				}
				else
				{
					// run
					m_nextAnim = RoopAnim::Run;
					m_moving = true;
				}
			}
			else
			{
				// idle
				m_nextAnim = RoopAnim::Idle;
				m_moving = false;
			}
		}
	}
}

// ���d�͏�Ԃ̈ړ�
void LoadPlayer::ZeroGravityMove()
{
	if (!m_isFloating)
	{
		if (!m_isTerning) m_transform.position.y += FloatHeight;
		else m_transform.position.y -= FloatHeight;
		m_isFloating = true;
	}

	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_SPACE))
	{
		// �d�͂̔��]
		m_isTerning = !m_isTerning;
		MV1SetRotationXYZ(m_model, Vector3(0, Math::DegtoRad(180), 0));
		m_moveMode = MoveMode::Normal;
		m_isFloating = false;
	}
}

// �ړ����[�h�̐؂�ւ�
void LoadPlayer::ChangeMoveMode()
{
	switch (m_moveMode)
	{
	case MoveMode::Normal:
		// �n�ʂɑ������Ă���Ƃ������ڍs�ł���
		if (m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag != 0)
		{
			// �ʏ�ړ� -> ���d�͈ړ�
			m_nextAnim = RoopAnim::Floating;
			m_moveMode = MoveMode::ZeroGravity;
			// �d�͂����Z�b�g
			m_fallTime = 0;
		}
		break;

	case MoveMode::ZeroGravity:
		// ���d�͈ړ� -> �ʏ�ړ�
		m_nextAnim = RoopAnim::Idle;
		m_moveMode = MoveMode::Normal;
		m_isFloating = false;
		// �d�͂����Z�b�g
		m_fallTime = 0;
		break;
	}
}

void LoadPlayer::Draw()
{
	// �A�j���[�V�����Đ�
	PlayAnim();
}

void LoadPlayer::OnCollision(const Actor3D* other)
{
	if (other->GetName() == "wall")
	{
		m_hit = true;
	}
}

