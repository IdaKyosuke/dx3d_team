#pragma once
#include"Vector3.h"

class Transform3D
{
public:
	Vector3 position;
	Vector3 angle;
	Vector3 scale;

	Transform3D() :
		angle(0,0,0),
		scale(1,1,1) {}
};