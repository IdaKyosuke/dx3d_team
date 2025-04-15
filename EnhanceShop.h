#pragma once
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"

class Inventory;

class EnhanceShop : public Node
{
private:
	Sprite m_enhanceShopUi;
	Transform m_enhanceShopTransform;


protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	EnhanceShop();
};
