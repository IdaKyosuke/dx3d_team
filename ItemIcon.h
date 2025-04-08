#pragma once
#include"Item.h"
#include "Inventory.h"
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class ItemIcon : public Node
{
private:
	const char* m_iconName[3] = { "goldBar_icon.png","gem.png","gem.png" };

	int m_itemNum;

	Sprite m_iconUi;
	Transform m_Icontransform;	// épê®

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	ItemIcon(int itemNum);
};