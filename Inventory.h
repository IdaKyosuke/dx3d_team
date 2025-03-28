#pragma once
#include"Item.h"
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"
#include <list>

//class Item;

class Inventory : public Node
{
private:
	static constexpr int MaxHaveItem = 2;	//アイテムを持てる最大量
	int m_haveItemCount;	//アイテム持てるかどうか

	bool m_canGetItem;

	std::list<Item> m_itemList;		//アイテム格納用
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
		m_haveItemCount--;
	}

	void SetItemList(Item* item)
	{
		m_itemList.push_front(*item);
	}
};