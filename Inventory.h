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
	static constexpr Vector2 TakeItemUiPos = Vector2(60,890);
	static constexpr Vector2 InventoryUiPos = Vector2(60, 890);


	static constexpr int SquareSize = 90;

	int m_maxHaveItem;

	int m_haveItemCount;	//アイテムを持ってる数

	int m_takeItem;			//今何のアイテムを持っているか
	int m_dropItemNum;			//捨てたアイテムの番号

	int m_nowHaveWeight;	//今どれぐらいの重さ持っているか

	bool m_canGetItem;			//アイテムを拾えるか
	bool m_gettingItem;			//アイテムを拾ったか
	bool m_dropItem;			//アイテムを落としたか
	bool m_dropItemCompletion;

	int m_itemNum;		//アイテムの番号格納用

	int m_seInventory;	//サウンド

	//アイテム格納用
	std::vector<Item> m_itemList;
	std::vector<Item> m_advanceItemList;
	Sprite m_inventoryUi;
	Sprite m_takeItemUi;
	Transform m_transform;	// 姿勢
	Transform m_takeItemTransform;	// 姿勢
	std::vector<Item*> m_addItemList;

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

	void GetDropItemCompletion()
	{
		m_dropItemCompletion = true;
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

	int GetHaveWeight()
	{
		return m_nowHaveWeight;
	}

	void AddItemCount()
	{
		m_haveItemCount++;
	}

	void ItemClear()
	{
		m_itemList.clear();
	}

	// アイテムが自分をインベントリ追加候補リストに追加する
	void AddAdvanceItemList(Item* item)
	{
		m_addItemList.push_back(item);
	}

	// アイテムがインベントリに入るかを確認
	void CheckCanAddItem();
};