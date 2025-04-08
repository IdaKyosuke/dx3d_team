#pragma once
#include"Item.h"
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"
#include <list>
#include <vector>

//class Item;

class Inventory : public Node
{
private:
	static constexpr int MaxHaveItem = 2;	//アイテムを持てる最大量
	int m_haveItemCount;	//アイテム持てるかどうか

	bool m_canGetItem;

	const char* m_iconName[3] = {"goldBar_icon.png","gem.png","gem.png"};

	int m_itemNum;

	//std::list<Item> m_itemList;		//アイテム格納用
	std::vector<Item> m_itemList;
	Sprite m_inventoryUi;
	Sprite m_iconUi;
	Transform m_transform;	// 姿勢
	Transform m_Icontransform;	// 姿勢

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Inventory();

	bool CanGetItem()
	{
		return m_canGetItem;
	}

	void OnInventory()
	{
		m_haveItemCount++;
	}

	void SetItemList(Item* item)
	{
		m_itemList.push_back(*item);
	}
};