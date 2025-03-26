#pragma once
#include "Vector2.h"
#include "Transform.h"

class BoxCollider2D;
class CircleCollider2D;

class Collider
{
public:
	// 中心座標からのズレ
	Vector2 offset;

	// コンストラクタ
	Collider(const Vector2& offset) :
		offset(offset) {}

	// 衝突判定
	// ダブルディスパッチによる形状識別を行う
	virtual bool CheckCollision(const Transform& transform1, const Transform& transform2, const Collider* collider2) const = 0;
	virtual bool CheckCollision(const Transform& transform1, const Transform& transform2, const BoxCollider2D* collider2) const = 0;
	virtual bool CheckCollision(const Transform& transform1, const Transform& transform2, const CircleCollider2D* collider2) const = 0;

#ifdef _DEBUG
	// 描画
	virtual void Draw(const Transform& transform) const = 0;
#endif
};
