#pragma once
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class MenuInventory;
class Inventory;
class Item;

class MenuItemIcon : public Node
{
private:
	static constexpr Vector2 SetPos =  Vector2(250,870);

	int m_itemNum;
	int m_countGetItem;

	Sprite m_itemIconUi;
	Transform m_transform;

	MenuInventory* m_menuInventory;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	MenuItemIcon(const int countGetItem, Item* item, MenuInventory* menuInventory = nullptr);
};
