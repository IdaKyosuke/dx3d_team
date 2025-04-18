#include "Enemy.h"
#include "Time.h"
#include "Math.h"
#include "Animation3D.h"
#include "Collision3D.h"
#include"ActorCollision3D.h"
#include"BoxCollider3D.h"
#include"LoadPlayer.h"
#include"Quaternion.h"
#include"NavMesh.h"
#include "Input.h"
#include "Lerp.h"
#include <math.h>


// �A�j���[�V�������X�g
const char* Enemy::AnimList[AnimNum] =
{
	"Zombie/ZombieIdle.mv1",
	"Zombie/ZombieWalk.mv1",
	"Zombie/ZombieAttack.mv1",
};

Enemy::Enemy(NavMesh* navMesh, const Vector3& pos, LoadPlayer* loadPlayer) :
	Actor3D("Enemy", pos),
	m_model(MV1LoadModel("Resource/Zombie/Zombie.mv1")),
	m_animIndex(0),
	m_nowAnim(Anim::Idle),
	m_nextAnim(Anim::Idle),
	m_navMesh(navMesh),
	m_player(loadPlayer),
	m_moveDirection(Vector3(0,0,0)),
	m_isSet(false),
	m_isFind(false),
	m_isAttack(false),
	m_countCoolTime(false),
	m_durationCoolTime(0)
{
	// �A�j���[�V�����N���X�����X�g������
	for (int i = 0; i < AnimNum; i++)
	{
		if (i < AnimNum - 1)
		{
			// ���[�v����A�j���[�V����
			m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i]));
			AddChild(m_attachAnimList[i]);
		}
		else
		{
			// ���[�v���Ȃ��A�j���[�V����
			m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i], false));
			AddChild(m_attachAnimList[i]);
		}
	}

	// ���f�����w��ꏊ�ɕ`��
	MV1SetPosition(m_model, pos);

	// �A�j���[�V�����p�̈ʒu
	m_enemyPastPos = m_enemyPos;

	// �ŏ��̃A�j���[�V�������w��
	m_attachAnimList[static_cast<int>(Anim::Idle)]->FadeIn();

	m_collider = new BoxCollider3D(FindColSize, ColOffset);
}

// �A�j���[�V������؂�ւ���(Lerp)
void Enemy::ChangeAnimLerp()
{
	if (m_nowAnim == m_nextAnim) return;

	m_attachAnimList[static_cast<int>(m_nowAnim)]->FadeOut();
	m_attachAnimList[static_cast<int>(m_nextAnim)]->FadeIn();

	m_nowAnim = m_nextAnim;
}

// �A�j���[�V������؂�ւ���(����)
void Enemy::ChangeAnimQuick(const Anim nextAnim)
{
	m_attachAnimList[static_cast<int>(m_nowAnim)]->ChangeOut();
	m_attachAnimList[static_cast<int>(nextAnim)]->ChangeIn();

	m_nowAnim = nextAnim;
	m_nextAnim = nextAnim;
}

// �A�j���[�V�������Đ�����
void Enemy::PlayAnim()
{
	// ���f���̕`��
	MV1DrawModel(m_model);

	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"EnemyPos Vector3(%.0f, %.0f, %.0f)",
		m_enemyPos.x, m_enemyPos.y, m_enemyPos.z
	);
}

// ���f���֌W���폜
void Enemy::Finalize()
{
	// ���f�����폜
	MV1DeleteModel(m_model);
}

void Enemy::Update()
{
	if (m_isAttack)
	{
		Attack();
	}
	else
	{
		// �U�����łȂ��Ƃ�
		EnemyMove();
	}

	// ���f���̉�]
	if (!m_moveDirection.IsZero())
	{
		float afterAngle = 0;

		Math::MatchAngleSign(afterAngle, m_moveDirection, m_transform.angle);

		m_transform.angle.y = Lerp::Exec(m_transform.angle.y, afterAngle, 0.2f);
	}

	// �A�j���[�V�����̐؂�ւ�
	ChangeAnimLerp();

	m_enemyPastPos = this->GetPosition();
}

// �G�̈ړ�
void Enemy::EnemyMove()
{
	// ���g�ƃv���C���[�Ԃ̌o�H�T�����s��
	m_navMesh->SetPathPlan(this->GetPosition(), m_player->GetPosition());

	// �ړ�����
	m_navMesh->MoveInitialize(this->GetPosition());

	m_transform.position = m_navMesh->Move(this->GetPosition(), MoveSpeed, 20.0f);

	// ����̒T�������폜
	m_navMesh->RemovePathPlan();

	m_moveDirection = m_transform.position - m_enemyPastPos;

	// �ړ��A�j���[�V����
	if (this->GetPosition() != m_enemyPastPos)
	{
		m_nextAnim = Anim::Run;
	}
	else
	{
		m_nextAnim = Anim::Idle;
	}
}

void Enemy::Draw()
{
	// �A�j���[�V�����Đ�
	PlayAnim();

	Quaternion::RotateAxisY(m_model, m_transform.angle.y, m_transform.position);

	// ���f���̕`��
	MV1DrawModel(m_model);

#ifdef _DEBUG
	DrawFormatString(0, 40, GetColor(255, 255, 255),
		"EnemyPos Vector3(%.0f, %.0f, %.0f)",
		this->GetPosition().x, this->GetPosition().y, this->GetPosition().z
	);
#endif // DEBUG
}

void Enemy::OnCollision(const Actor3D* other)
{
	if (other->GetName() == "Player")
	{
		if (!m_isFind)
		{
			// �v���C���[�������Ă��Ȃ� => �R���C�_�[������������
			// ��������폜
			m_collider->ChangeSize(AttackColSize);
			m_isFind = true;

		}
		else
		{
			// �v���C���[�𔭌��ς� => �U��
			m_nextAnim = Anim::Attack;
			m_player->DecreaseHP(Power);
			m_isAttack = true;
		}
	}
}

// �G�̍U��
void Enemy::Attack()
{
	// �U�����͓����Ȃ�
	if (m_attachAnimList[static_cast<int>(m_nowAnim)]->FinishAnim())
	{
		// �U���A�j���[�V�����̏I���Ɠ����ɃA�j���[�V�����ύX
		ChangeAnimQuick(Anim::Idle);
		m_countCoolTime = true;
	}

	if (m_countCoolTime)
	{
		m_durationCoolTime += Time::GetInstance()->GetDeltaTime();

		// �N�[���^�C���I��
		if (m_durationCoolTime <= CoolTime)
		{
			m_countCoolTime = false;
			m_durationCoolTime = 0;
			m_isAttack = false;
		}
	}
}
