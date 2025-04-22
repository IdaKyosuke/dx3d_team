#include"EscapePoint.h"
#include"BoxCollider3D.h"
#include "ModelLoader.h"
#include"Quaternion.h"
#include"Math.h"

EscapePoint::EscapePoint() :
	Actor3D("Exit"),
	m_isEscape(false)
{
	// 脱出地点の座標を設定
	int index = rand() % PointNum;
	m_transform.position = pos[index];

	m_collider = new BoxCollider3D(ColSize, ColOffset);

	m_model = ModelLoader::LoadModel(m_stairPath);

	// モデルの回転
	MV1SetRotationXYZ(m_model, Vector3(0, Math::DegtoRad(Rotate[index]), 0));

	// モデルの座標指定
	MV1SetPosition(m_model, m_transform.position);

	// モデルの描画
	MV1DrawModel(m_model);
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