#pragma once
#pragma once
#include "Vector3.h"
#include "Transform3D.h"

class BoxCollider3D;

class Collider3D
{
public:
	// 中心座標からのズレ
	Vector3 m_offset;

	// コンストラクタ
	Collider3D(const Vector3& offset) :
		m_offset(offset) {}

	virtual void ChangeSize(const Vector3& size) = 0;

	// 衝突判定
	// ダブルディスパッチによる形状識別を行う
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const Collider3D* collider2) const = 0;
	virtual bool CheckCollision(const Transform3D& transform1, const Transform3D& transform2, const BoxCollider3D* collider2) const = 0;

#ifdef _DEBUG
	// 描画
	virtual void Draw(const Transform3D& transform) const = 0;
#endif
};
