#include"Enemy.h"
#include"LoadPlayer.h"
#include"NavMesh.h"

Enemy::Enemy(NavMesh* navMesh, const Vector3& pos, LoadPlayer* loadPlayer) :
	Actor3D("Enemy", pos),
	m_navMesh(navMesh),
	m_model(MV1LoadModel("Resource/item.mv1")),
	m_player(loadPlayer)
{

}

void Enemy::Update()
{
	// ���g�ƃv���C���[�Ԃ̌o�H�T�����s��
	m_navMesh->SetPathPlan(this->GetPosition(), m_player->GetPosition());

	// �ړ�����
	m_navMesh->MoveInitialize();

	// �ړ�
	m_transform.position = m_navMesh->Move(this->GetPosition());
}

void Enemy::Draw()
{
	// ���f���̕`��
	MV1DrawModel(m_model);
}

void Enemy::OnCollision(const Actor3D* other)
{

}