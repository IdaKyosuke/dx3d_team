#pragma once
<<<<<<< HEAD
#include"Item.h"
#include "Inventory.h"
=======
#include "Item.h"
>>>>>>> origin/item
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class ItemIcon : public Node
{
private:
<<<<<<< HEAD
	static constexpr Vector2 SetPos[5] = { Vector2(60,890),Vector2(150,890),Vector2(240,890),Vector2(330,890),Vector2(420,890) };

	const char* m_iconName[3] = { "iron_icon.png","gem_icon.png","potion_icon.png"};
=======
	static constexpr Vector2 SetPos[5] = {Vector2(100,200),Vector2(400,500)};

	const char* m_iconName[3] = {"goldBar_icon.png","gem_icon.png"};

>>>>>>> origin/item

	int m_itemNum;
	int m_countGetItem;

	Sprite m_itemIconUi;
<<<<<<< HEAD
	Transform m_transform;
=======
	Transform m_transform;	// Žp¨
>>>>>>> origin/item

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
<<<<<<< HEAD
	ItemIcon(int itemNum, int countGetItem);
=======
	ItemIcon(int itemNum,int countGetItem);
>>>>>>> origin/item
};