#include"Enemy.h"
#include"LoadPlayer.h"
#include"BoxCollider3D.h"
#include"Quaternion.h"
#include"NavMesh.h"

Enemy::Enemy(NavMesh* navMesh, const Vector3& pos, LoadPlayer* loadPlayer) :
	Actor3D("Enemy", pos),
	m_navMesh(navMesh),
	m_model(MV1LoadModel("Resource/item.mv1")),
	m_player(loadPlayer),
	m_isSet(false)
{
	m_collider = new BoxCollider3D(Vector3(200, 200, 200));
}

void Enemy::Update()
{
	// Ž©g‚ÆƒvƒŒƒCƒ„[ŠÔ‚ÌŒo˜H’Tõ‚ðs‚¤
	m_navMesh->SetPathPlan(this->GetPosition(), m_player->GetPosition());

	// ˆÚ“®€”õ
	m_navMesh->MoveInitialize(this->GetPosition());

	m_transform.position = m_navMesh->Move(this->GetPosition(), MoveSpeed, 20.0f);

	// ¡‰ñ‚Ì’Tõî•ñ‚ðíœ
	m_navMesh->RemovePathPlan();
}

void Enemy::Draw()
{
	Quaternion::RotateAxisY(m_model, m_transform.angle.y, m_transform.position);

	// ƒ‚ƒfƒ‹‚Ì•`‰æ
	MV1DrawModel(m_model);

	DrawFormatString(0, 40, GetColor(255, 255, 255),
		"EnemyPos Vector3(%.0f, %.0f, %.0f)",
		this->GetPosition().x, this->GetPosition().y, this->GetPosition().z
	);
}

void Enemy::OnCollision(const Actor3D* other)
{

}