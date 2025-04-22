#include"EscapePoint.h"
#include"BoxCollider3D.h"
#include "ModelLoader.h"

EscapePoint::EscapePoint() :
	Actor3D("Exit"),
	m_isEscape(false)
{
	// 脱出地点の座標を設定
	int index = rand() % PointNum;
	m_transform.position = pos[index];

	m_collider = new BoxCollider3D(ColSize, ColOffset);

	m_model = ModelLoader::LoadModel(m_stairPath);

	// モデルの描画
	MV1DrawModel(m_model);

	//ポジションの設定
	MV1SetPosition(m_model, m_transform.position);
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

void EscapePoint::Draw()
{
	// モデルの描画
	MV1DrawModel(m_model);
}