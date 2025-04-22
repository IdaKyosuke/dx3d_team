#include"EscapePoint.h"
#include"BoxCollider3D.h"

EscapePoint::EscapePoint() :
	Actor3D("Exit"),
	m_isEscape(false)
{
	// 脱出地点の座標を設定
	int index = rand() % PointNum;
	m_transform.position = pos[index];

	m_collider = new BoxCollider3D(ColSize);
}

void EscapePoint::OnCollision(const Actor3D* other)
{
	// 脱出地点にプレイヤーが到着したら
	if (other->GetName() == "Player")
	{
		// 脱出できたことにする
		m_isEscape = true;
	}
}