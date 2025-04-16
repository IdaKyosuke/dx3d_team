#pragma once
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"

class Chest;
class EnhanceButton;
class EnhanceInventory;

class EnhanceShop : public Node
{
private:
	Sprite m_enhanceShopUi;
	Transform m_enhanceShopTransform;

	Chest* m_chest;
	EnhanceButton* m_enhanceButton;
	EnhanceInventory* m_enhanceInventory;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	EnhanceShop(Chest* chest,EnhanceButton* enhanceButton,EnhanceInventory* enhanceInventory);
};
