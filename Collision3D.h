#pragma once
#include"Vector3.h"

class Transform3D;
class BoxCollider3D;

class Collision3D
{
public:
	// 箱と箱
	static bool Check(
		const Transform3D& transform1, const BoxCollider3D* box1,
		const Transform3D& transform2, const BoxCollider3D* box2
	);

	// 分離軸に投影された軸成分から投影線分の長さを出す(sep:分離軸, e1,2,3:判定相手の各方向ベクトル)
	static float GetLengthSeparateAxis(
		const Vector3& sep,
		const Vector3& e1,
		const Vector3& e2,
		const Vector3& e3 = Vector3(0, 0, 0)
	);
};