#pragma once
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class Inventory;

class ItemIcon : public Node
{
private:
	static constexpr Vector2 SetPos = Vector2(60,890);

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
