#include "Enemy.h"
#include "Time.h"
#include "Math.h"
#include "Animation3D.h"
#include "Collision3D.h"
#include "Input.h"
#include "Lerp.h"
#include "Quaternion.h"
#include <math.h>

// �A�j���[�V�������X�g
const char* Enemy::AnimList[RoopAnimNum] =
{
	"Zombie/ZombieIdle.mv1",
	"Zombie/ZombieWalk.mv1",
	"Zombie/ZombieAttack.mv1",
};

Enemy::Enemy(Collision3D* collision3D) :
	Actor("Enemy"),
	m_model(MV1LoadModel("Resource/Zombie/Zombie.mv1")),
	m_animIndex(0),
	m_nowAnim(RoopAnim::Idle),
	m_nextAnim(RoopAnim::Idle),
	m_collision3D(collision3D)
{
	// �A�j���[�V�����N���X�����X�g������
	for (int i = 0; i < RoopAnimNum; i++)
	{
		m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i]));
		AddChild(m_attachAnimList[i]);
	}

	// �G�̈ʒu(x, y, z)
	m_enemyPos = SpawnPos;

	// ���f�����w��ꏊ�ɕ`��
	MV1SetPosition(m_model, SpawnPos);

	// �A�j���[�V�����p�̈ʒu
	m_enemyPastPos = m_enemyPos;

	// �f�B���N�V���i�����C�g���쐬
	m_lightHandle = CreateDirLightHandle(Vector3(0, 100.0f, 0));

	// �ŏ��̃A�j���[�V�������w��
	m_attachAnimList[static_cast<int>(RoopAnim::Idle)]->FadeIn();
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
void Enemy::ChangeAnimQuick(const RoopAnim nextAnim)
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
	// ���C�g�n���h�����폜
	DeleteLightHandle(m_lightHandle);
	// �v���C���[�̃��f�����폜
	MV1DeleteModel(m_model);
}

void Enemy::Update()
{
	// 1�t���[���O�̈ʒu���X�V
	m_enemyPastPos = m_enemyPos;

	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_1))
	{
		m_nextAnim = RoopAnim::Idle;
	}
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_2))
	{
		m_nextAnim = RoopAnim::Walk;
	}
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_3))
	{
		m_nextAnim = RoopAnim::Attack;
	}

	// �A�j���[�V�����̐؂�ւ�
	ChangeAnimLerp();
}

void Enemy::Draw()
{
	// �A�j���[�V�����Đ�
	PlayAnim();
}
