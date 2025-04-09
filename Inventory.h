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
	static constexpr int MaxHaveItem = 5;	//アイテムを持てる最大量
	int m_haveItemCount;	//アイテム持てるかどうか

	int m_takeItem;		//今何のアイテムを持っているか

	bool m_canGetItem;
	bool m_gettingItem;

	int m_itemNum;

	//アイテム格納用
	std::vector<Item> m_itemList;
	Sprite m_inventoryUi;
	Transform m_transform;	// 姿勢

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

	void GettingItem()
	{
		m_gettingItem = true;
	}

	void SetItemList(Item* item)
	{
		m_itemList.push_back(*item);
	}
};