#include"EscapePoint.h"
#include"BoxCollider3D.h"
#include "ModelLoader.h"
#include"Quaternion.h"
#include"Math.h"

EscapePoint::EscapePoint(const Vector3& pos, const float rot) :
	Actor3D("Exit"),
	m_isEscape(false)
{
	m_transform.position = pos;

	m_collider = new BoxCollider3D(ColSize, ColOffset);

	m_model = ModelLoader::LoadModel(m_stairPath);

	// モデルの回転
	MV1SetRotationXYZ(m_model, Vector3(0, Math::DegtoRad(rot), 0));

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