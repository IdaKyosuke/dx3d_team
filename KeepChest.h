#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <list>
#include <vector>

class LoadPlayer;

class KeepChest : public Node
{
private:
	//アイテム格納用
	std::vector<Item> m_itemList;

public:
	KeepChest() {};

	void SetItemList(int itemNum)
	{
		Item m_item = Item(itemNum);

		m_itemList.push_back(m_item);
	}

	std::vector<Item> TakeItMenu()
	{
		return m_itemList;
	}
};