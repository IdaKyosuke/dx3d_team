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
const char* LoadPlayer::AnimList[AnimNum] =
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
	m_nowAnim(Anim::Idle),
	m_nextAnim(Anim::Idle),
	m_moveDirection(Vector3(0, 0, 0)),
	m_camNode(nullptr),
	m_collisionStage(collisionStage),
	m_elapsedTime(0),
	m_fallTime(0),
	m_isJump(false),
	m_isJumping(false),
	m_isFloating(false),
	m_duration(0),
	m_isFall(false),
	m_fallStartY(0),
	m_hit(false)
{
	//-----�A�j���[�V�����̍쐬-----
	// �A�j���[�V�����N���X�����X�g������
	for (int i = 0; i < AnimNum; i++)
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
	m_attachAnimList[static_cast<int>(Anim::Idle)]->FadeIn();

	m_collider = new BoxCollider3D(ColSize, ColOffset);

	// �̗͂̏����l��ݒ�
	m_hp = 100;
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
void LoadPlayer::ChangeAnimQuick(const Anim nextAnim)
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
	Quaternion::RotateAxisY(m_model, m_transform.angle.y, m_transform.position);

	// ���f���̕`��
	MV1DrawModel(m_model);
	
#ifdef _DEBUG
	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"PlayerPos Vector3(%.0f, %.0f, %.0f)",
		m_transform.position.x, m_transform.position.y, m_transform.position.z
	);

	DrawFormatString(0, 150, GetColor(255, 255, 255), "PlayerHP %.0f", m_hp);
#endif // _DEBUG
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
	// �� -> ���X�|�[�� or press "r" => ���X�^�[�g
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_R) || m_hp <= 0)
	{
		m_transform.position = SpawnPos;
		m_hp = 100;
	}

	// ���G����
	if (m_hit)
	{
		InvincibleTime();
	}

	// �v���C���[�̈ړ�
	NormalMove();

	// �A�j���[�V�����̐؂�ւ�
	ChangeAnimLerp();

	// 1�t���[���O�̈ʒu���X�V
	m_playerPastPos = m_transform.position;
}

// �W�����v����
void LoadPlayer::Jumping()
{
	m_elapsedTime += Time::GetInstance()->GetDeltaTime();

	m_transform.position.y += JumpPower - Gravity * m_elapsedTime;

	// ���������ɐG�ꂽ��
	if (m_isJumping && m_collisionStage->GetHeight(m_transform.position).HitFlag)
	{
		m_isJump = false;
		m_isJumping = false;
		m_nextAnim = Anim::Landing;
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

	// �ړ�����������i�d�͂ɉ����č��E�ړ��̌������ς��j
	m_moveDirection = m_camNode->CamFrontPlaneVec() * inputZ + m_camNode->CamRight() * inputX;

	// �d�͂����Z�i�W�����v���łȂ� && ���n���Ă��Ȃ��Ƃ��̂݁j
	if (!m_isJump && m_collisionStage->GetHeight(m_transform.position).HitFlag == 0)
	{
		m_fallTime += Time::GetInstance()->GetDeltaTime();
		// �ʏ펞
		m_transform.position.y -= Gravity * m_fallTime;

		if (!m_isFall)
		{
			// �����J�n���̍�����ێ�
			m_fallStartY = m_transform.position.y;
			m_isFall = true;
		}
	}
	else if (m_collisionStage->GetHeight(m_transform.position).HitFlag == 1)
	{
		// �v���C���[�̍����𑫏�̍����ɍ��킹��
		m_fallTime = 0;
		// �ʏ펞
		m_transform.position.y = m_collisionStage->GetHeight(m_transform.position).HitPosition.y;

		if (m_isFall)
		{
			CountFallHeight();
			m_isFall = false;
		}
	}

	// �ړ�
	m_transform.position += Math::Normalized(m_moveDirection) * (Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT) ? RunSpeed : WalkSpeed);
	
	// �i�ޗ\���ɑ��ꂪ���邩
	if (!m_collisionStage->CheckStage(this->GetPosition()))
	{
		// �Ȃ��Ƃ�
		m_transform.position.x = m_playerPastPos.x;
		m_transform.position.z = m_playerPastPos.z;
	}

	// �W�����v
	if (!m_isJump && m_collisionStage->GetHeight(m_transform.position).HitFlag != 0 && Input::GetInstance()->IsKeyDown(KEY_INPUT_SPACE))
	{
		m_nextAnim = Anim::JumpUp;
		m_isJump = true;
		m_elapsedTime = 0;
		// �W�����v�̃X�^�[�g�n�_���L�^
		m_fallStartY = m_transform.position.y;
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
	if (m_collisionStage->GetHeight(m_transform.position).HitFlag == 0)
	{
		m_nextAnim = Anim::JumpIdle;
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
					m_nextAnim = Anim::Walk;
					m_moving = true;
				}
				else
				{
					// run
					m_nextAnim = Anim::Run;
					m_moving = true;
				}
			}
			else
			{
				// idle
				m_nextAnim = Anim::Idle;
				m_moving = false;
			}
		}
	}
}

void LoadPlayer::Draw()
{
	// �A�j���[�V�����Đ�
	PlayAnim();
}

void LoadPlayer::OnCollision(const Actor3D* other)
{
	if (!m_hit && other->GetName() == "Enemy")
	{
		m_hit = true;
		DecreaseHP(10);
	}
}

// ���G����
void LoadPlayer::InvincibleTime()
{
	m_duration += Time::GetInstance()->GetDeltaTime();

	if (m_duration >= m_time)
	{
		// ���G���Ԃ̏I��
		m_duration = 0;
		m_hit = false;
	}
}

// ���������������v�Z����
void LoadPlayer::CountFallHeight()
{
	if ((abs(m_transform.position.y) - abs(m_fallStartY)) >= 1300.0f)
	{
		// �u�]���鍂�����痎�������Ƃ�
		m_hp = 0;
	}
	m_fallStartY = 0;
}

