#pragma once
#include "Collider.h"
#include "Collision.h"
#include "DxLib.h"

class BoxCollider2D : public Collider
{
public:
	Vector2 size;	// 幅・高さ

	// コンストラクタ
	BoxCollider2D(const Vector2& size, const Vector2& offset = Vector2(0, 0)) :
		Collider(offset),
		size(size) {}

	// 衝突判定
	virtual bool CheckCollision(const Transform& transform1, const Transform& transform2, const Collider* collider2) const override
	{
		// ダブルディスパッチによる形状識別をする
		return collider2->CheckCollision(transform2, transform1, this);
	}
	virtual bool CheckCollision(const Transform& transform1, const Transform& transform2, const BoxCollider2D* collider2) const override
	{
		// 矩形×矩形
		return Collision::Check(transform1, this, transform2, collider2);
	}
	virtual bool CheckCollision(const Transform& transform1, const Transform& transform2, const CircleCollider2D* collider2) const override
	{
		// 矩形×円
		return Collision::Check(transform1, this, transform2, collider2);
	}

#ifdef _DEBUG
	// 描画
	virtual void Draw(const Transform& transform) const override
	{
		DrawBox(
			static_cast<int>(transform.position.x + offset.x * transform.scale - size.x * transform.scale / 2),
			static_cast<int>(transform.position.y + offset.y * transform.scale - size.y * transform.scale / 2),
			static_cast<int>(transform.position.x + offset.x * transform.scale + size.x * transform.scale / 2),
			static_cast<int>(transform.position.y + offset.y * transform.scale + size.y * transform.scale / 2),
			GetColor(0, 255, 0), false
		);
	}
#endif
};
