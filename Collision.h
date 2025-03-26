#pragma once

class Transform;
class BoxCollider2D;
class CircleCollider2D;

// è’ìÀîªíË
class Collision
{
public:
	static bool Check(
		const Transform& transform1, const BoxCollider2D* box1,
		const Transform& transform2, const BoxCollider2D* box2
	);

	static bool Check(
		const Transform& transform1, const BoxCollider2D* box,
		const Transform& transform2, const CircleCollider2D* circle
	);

	static bool Check(
		const Transform& transform1, const CircleCollider2D* circle,
		const Transform& transform2, const BoxCollider2D* box
	)
	{
		Check(transform2, box, transform1, circle);
	}

	static bool Check(
		const Transform& transform1, const CircleCollider2D* circle1,
		const Transform& transform2, const CircleCollider2D* circle2
	);
};
