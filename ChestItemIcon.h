#pragma once
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class Chest;
class Item;

class ChestItemIcon : public Node
{
private:
	static constexpr Vector2 SetPos = Vector2(235, 560);

	int m_itemNum;
	int m_countGetItem;

	Sprite m_itemIconUi;
	Transform m_transform;

	Chest* m_chest;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	ChestItemIcon(const int countGetItem, Item* item, Chest* chest);
};
