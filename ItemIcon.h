#pragma once
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class Inventory;

class ItemIcon : public Node
{
private:
	static constexpr Vector2 SetPos[5] = { Vector2(60,890),Vector2(150,890),Vector2(240,890),Vector2(330,890),Vector2(420,890) };

	const char* m_iconName[6] = { "iron_icon.png","gem_icon.png","potion_icon.png","bag_icon.png","coin_icon.png","watch_icon.png" };

	int m_itemNum;
	int m_countGetItem;

	Sprite m_itemIconUi;
	Transform m_transform;

	Inventory* m_inventory;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	ItemIcon(int itemNum, int countGetItem,Inventory* inventory);
};
