#pragma once
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class Map : public Node
{
	static constexpr Vector2 MapPos = Vector2(640, 400);

	Sprite m_drawMap;
	Transform m_transform;
	bool m_isOpenMap;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Map();
};