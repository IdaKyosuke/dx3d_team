#pragma once
#include "Actor.h"
#include "BoxCollider2D.h"

class HitBox : public Actor
{
public:
	HitBox(const char* name, const Vector2& position, const Vector2& size) :
		Actor(name, nullptr, position)
	{
		m_collider = new BoxCollider2D(size);
	}
};
