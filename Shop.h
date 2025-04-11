#pragma once
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"

class Inventory;

class Shop : public Node
{
private:
	Sprite m_shopUi;
	Transform m_transform;

	Inventory* m_inventory;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Shop(Inventory* inventory);
};
