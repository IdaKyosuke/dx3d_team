#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <list>
#include <vector>

class Inventory : public Node
{
private:
	static constexpr float FirstMaxHaveWeight = 50;	//最初に持てるアイテムの重さの最大量
	static constexpr Vector2 TakeItemUiPos = Vector2(60,890);
	static constexpr Vector2 InventoryUiPos = Vector2(60, 890);


	static constexpr int SquareSize = 90;

	int m_maxHaveItem;
	float m_maxHaveWeight;

	int m_haveItemCount;	//アイテムを持ってる数

	int m_takeItem;			//今何のアイテムを持っているか
	int m_dropItemNum;			//捨てたアイテムの番号

	float m_canHaveWeight;	//どれぐらいの重さまで持てるか

	bool m_canGetItem;			//アイテムを拾えるか
	bool m_gettingItem;			//アイテムを拾ったか
	bool m_dropItem;			//アイテムを落としたか
	bool m_dropItemHoge;

	int m_itemNum;		//アイテムの番号格納用

	//アイテム格納用
	std::vector<Item> m_itemList;
	std::vector<Item> m_advanceItemList;
	Sprite m_inventoryUi;
	Sprite m_takeItemUi;
	Transform m_transform;	// 姿勢
	Transform m_takeItemTransform;	// 姿勢


protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Inventory(int maxHaveItem);

	bool CanGetItem()
	{
		return m_canGetItem;
	}
	
	void SetItemList(int itemNum)
	{
		Item item = Item(itemNum);

		if (m_maxHaveItem >= m_haveItemCount)
		{
			m_advanceItemList.push_back(item);
		}
	}

	void ItemListSet();

	void TakeItem(int itemNum);

	bool GetItemNow()
	{
		return m_gettingItem;
	}

	void GettingItem()
	{
		m_gettingItem = true;
	}

	bool GetDropItem()
	{
		return m_dropItem;
	}

	int GetDropItemNum()
	{
		return m_dropItemNum;
	}

	void GetDropItemHoge()
	{
		m_dropItemHoge = true;
	}

	std::vector<Item> GetItemList()
	{
		return m_itemList;
	}

	int GetTakeItem()
	{
		return m_takeItem;
	}

	void EnhanceInventory()
	{
		m_maxHaveItem++;
	}

	int GetMaxHaveItem()
	{
		return m_maxHaveItem;
	}

	int GetHaveItemCount()
	{
		return m_haveItemCount;
	}

	void AddItemCount()
	{
		m_haveItemCount++;
	}
};