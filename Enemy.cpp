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
	// 自身とプレイヤー間の経路探索を行う
	m_navMesh->SetPathPlan(this->GetPosition(), m_player->GetPosition());

	// 移動準備
	m_navMesh->MoveInitialize();

	// 移動
	m_transform.position = m_navMesh->Move(this->GetPosition());
}

void Enemy::Draw()
{
	// モデルの描画
	MV1DrawModel(m_model);
}

void Enemy::OnCollision(const Actor3D* other)
{

}