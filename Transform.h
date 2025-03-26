#pragma once
#include "Vector2.h"

class Transform
{
public:
	Vector2 position;	// 座標
	float angle;		// 角度（ラジアン）
	float scale;		// 拡縮

	Transform() :
		angle(0),
		scale(1) {}
};
