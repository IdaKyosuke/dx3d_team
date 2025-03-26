#pragma once
#include "Collider.h"
#include "Collision.h"
#include "DxLib.h"

class CircleCollider2D : public Collider
{
public:
	int radius;	// 半径

	// コンストラクタ
	CircleCollider2D(int radius, const Vector2& offset = Vector2(0, 0)) :
		Collider(offset),
		radius(radius) {}

	// 衝突判定
	virtual bool CheckCollision(const Transform& transform1, const Transform& transform2, const Collider* collider2) const override
	{
		// ダブルディスパッチによる形状識別をする
		return collider2->CheckCollision(transform2, transform1, this);
	}
	virtual bool CheckCollision(const Transform& transform1, const Transform& transform2, const BoxCollider2D* collider2) const override
	{
		// 矩形×円
		return Collision::Check(transform2, collider2, transform1, this);
	}
	virtual bool CheckCollision(const Transform& transform1, const Transform& transform2, const CircleCollider2D* collider2) const override
	{
		// 円×円
		return Collision::Check(transform1, this, transform2, collider2);
	}

#ifdef _DEBUG
	// 描画
	virtual void Draw(const Transform& transform) const override
	{
		DrawCircle(
			static_cast<int>(transform.position.x + offset.x * transform.scale),
			static_cast<int>(transform.position.y + offset.y * transform.scale),
			static_cast<int>(radius * transform.scale),
			GetColor(0, 255, 0), false
		);
	}
#endif
};
