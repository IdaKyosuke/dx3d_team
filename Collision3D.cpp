#include"Collision3D.h"
#include"Vector3.h"
#include"Transform3D.h"
#include"BoxCollider3D.h"
#include<math.h>

// 箱と箱
bool Collision3D::Check(
	const Transform3D& transform1, const BoxCollider3D* box1,
	const Transform3D& transform2, const BoxCollider3D* box2
)
{
	// 各箱の中心座標とサイズを用意、オブジェクトの回転も計算する(回転は未実装)
	Vector3 center1 = transform1.position + box1->m_offset.ChangeScale(transform1.scale);
	Vector3 vecX1 = (box1->m_normVec[0] * box1->m_size.x) / 2;
	Vector3 vecY1 = (box1->m_normVec[1] * box1->m_size.y) / 2;
	Vector3 vecZ1 = (box1->m_normVec[2] * box1->m_size.z) / 2;

	Vector3 center2 = transform2.position + box2->m_offset.ChangeScale(transform2.scale);
	Vector3 vecX2 = (box2->m_normVec[0] * box2->m_size.x) / 2;
	Vector3 vecY2 = (box2->m_normVec[1] * box2->m_size.y) / 2;
	Vector3 vecZ2 = (box2->m_normVec[2] * box2->m_size.z) / 2;

	// box1の中心->box2の中心のベクトル
	Vector3 distance = center1 - center2;

	// 分離平面が存在しているかを判定 （いずれかの条件に当てはまった時、衝突はしていない）
	// ---- 各標準化方向ベクトル ----
	// 分離軸 box1X
	float r1 = vecX1.Magnitude();
	float r2 = GetLengthSeparateAxis(box1->m_normVec[0], vecX2, vecY2, vecZ2);
	float length = static_cast<float>(fabs(distance.DotP(box1->m_normVec[0])));
	if (length > r1 + r2) return false;	

	// 分離軸 box1Y
	r1 = vecY1.Magnitude();
	r2 = GetLengthSeparateAxis(box1->m_normVec[1], vecX2, vecY2, vecZ2);
	length = static_cast<float>(fabs(distance.DotP(box1->m_normVec[1])));
	if (length > r1 + r2) return false;

	// 分離軸 box1Z
	r1 = vecZ1.Magnitude();
	r2 = GetLengthSeparateAxis(box1->m_normVec[2], vecX2, vecY2, vecZ2);
	length = static_cast<float>(fabs(distance.DotP(box1->m_normVec[2])));
	if (length > r1 + r2) return false;

	// 分離軸 box2X
	r1 = vecX2.Magnitude();
	r2 = GetLengthSeparateAxis(box2->m_normVec[0], vecX1, vecY1, vecZ1);
	length = static_cast<float>(fabs(distance.DotP(box2->m_normVec[0])));
	if (length > r1 + r2) return false;

	// 分離軸 box1Y
	r1 = vecY2.Magnitude();
	r2 = GetLengthSeparateAxis(box2->m_normVec[1], vecX1, vecY1, vecZ1);
	length = static_cast<float>(fabs(distance.DotP(box2->m_normVec[1])));
	if (length > r1 + r2) return false;

	// 分離軸 box1Z
	r1 = vecZ2.Magnitude();
	r2 = GetLengthSeparateAxis(box2->m_normVec[2], vecX1, vecY1, vecZ1);
	length = static_cast<float>(fabs(distance.DotP(box2->m_normVec[2])));
	if (length > r1 + r2) return false;

	// ---- 双方の方向ベクトルに垂直な分離軸 ----
	// 分離軸 box1X - box2X
	Vector3 cross = box1->m_normVec[0].CrossP(box2->m_normVec[0]);
	r1 = GetLengthSeparateAxis(cross, vecY1, vecZ1);
	r2 = GetLengthSeparateAxis(cross, vecY2, vecZ2);
	length = static_cast<float>(fabs(distance.DotP(cross)));
	if (length > r1 + r2) return false;
	// 分離軸 box1X - box2Y
	cross = box1->m_normVec[0].CrossP(box2->m_normVec[1]);
	r1 = GetLengthSeparateAxis(cross, vecY1, vecZ1);
	r2 = GetLengthSeparateAxis(cross, vecX2, vecZ2);
	length = static_cast<float>(fabs(distance.DotP(cross)));
	if (length > r1 + r2) return false;
	// 分離軸 box1X - box2Z
	cross = box1->m_normVec[0].CrossP(box2->m_normVec[2]);
	r1 = GetLengthSeparateAxis(cross, vecY1, vecZ1);
	r2 = GetLengthSeparateAxis(cross, vecX2, vecY2);
	length = static_cast<float>(fabs(distance.DotP(cross)));
	if (length > r1 + r2) return false;


	// 分離軸 box1Y - box2X
	cross = box1->m_normVec[1].CrossP(box2->m_normVec[0]);
	r1 = GetLengthSeparateAxis(cross, vecX1, vecZ1);
	r2 = GetLengthSeparateAxis(cross, vecY2, vecZ2);
	length = static_cast<float>(fabs(distance.DotP(cross)));
	if (length > r1 + r2) return false;
	// 分離軸 box1Y - box2Y
	cross = box1->m_normVec[1].CrossP(box2->m_normVec[1]);
	r1 = GetLengthSeparateAxis(cross, vecX1, vecZ1);
	r2 = GetLengthSeparateAxis(cross, vecX2, vecZ2);
	length = static_cast<float>(fabs(distance.DotP(cross)));
	if (length > r1 + r2) return false;
	// 分離軸 box1Y - box2Z
	cross = box1->m_normVec[1].CrossP(box2->m_normVec[2]);
	r1 = GetLengthSeparateAxis(cross, vecX1, vecZ1);
	r2 = GetLengthSeparateAxis(cross, vecX2, vecY2);
	length = static_cast<float>(fabs(distance.DotP(cross)));
	if (length > r1 + r2) return false;


	// 分離軸 box1Z - box2X
	cross = box1->m_normVec[2].CrossP(box2->m_normVec[0]);
	r1 = GetLengthSeparateAxis(cross, vecX1, vecY1);
	r2 = GetLengthSeparateAxis(cross, vecY2, vecZ2);
	length = static_cast<float>(fabs(distance.DotP(cross)));
	if (length > r1 + r2) return false;
	// 分離軸 box1Z - box2Y
	cross = box1->m_normVec[2].CrossP(box2->m_normVec[1]);
	r1 = GetLengthSeparateAxis(cross, vecX1, vecY1);
	r2 = GetLengthSeparateAxis(cross, vecX2, vecZ2);
	length = static_cast<float>(fabs(distance.DotP(cross)));
	if (length > r1 + r2) return false;
	// 分離軸 box1Z - box2Z
	cross = box1->m_normVec[2].CrossP(box2->m_normVec[2]);
	r1 = GetLengthSeparateAxis(cross, vecX1, vecY1);
	r2 = GetLengthSeparateAxis(cross, vecX2, vecY2);
	length = static_cast<float>(fabs(distance.DotP(cross)));
	if (length > r1 + r2) return false;

	// 分離平面が存在していないので衝突している
	return true;
}

// 分離軸に投影された軸成分から投影線分の長さを出す(sep:標準化された分離軸, e1～3:判定相手の各方向ベクトル)
float Collision3D::GetLengthSeparateAxis(
	const Vector3& sep,
	const Vector3& e1,
	const Vector3& e2,
	const Vector3& e3
)
{
	// 分離軸とそれぞれの軸成分の内積の和(fabs:浮動小数点型の絶対値を求める, abs:整数型の絶対値を求める)
	float r1 = static_cast<float>(fabs(sep.DotP(e1)));
	float r2 = static_cast<float>(fabs(sep.DotP(e2)));
	float r3 = e3 != Vector3(0, 0, 0) ? static_cast<float>(fabs(sep.DotP(e3))) : 0;
	return r1 + r2 + r3;
}