#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <list>
#include <vector>

class EnhanceType;

class Inventory : public Node
{
private:
	static constexpr Vector2 TakeItemUiPos = Vector2(60,890);
	static constexpr Vector2 InventoryUiPos = Vector2(60, 890);
	static constexpr Vector2 SlashUiPos = Vector2(105, 823);
	static constexpr Vector2 FontSize = Vector2(20, 35);	// 数字1文字の幅・高さ
	static constexpr int FontMargin = 5;					// 数字と数字の間の余白

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

	int m_fontTextureId;	// 数字フォント
	Transform m_haveWeightTransform;	// 姿勢

	//アイテム格納用
	std::vector<Item> m_itemList;   
	std::vector<Item*> m_addItemList;
	Sprite m_inventoryUi;
	Sprite m_takeItemUi;
	Sprite m_slashUi;
	Transform m_inventoryTransform;	// 姿勢
	Transform m_takeItemTransform;	// 姿勢
	Transform m_slashTransform;	// 姿勢

	EnhanceType* m_enhanceType;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Inventory(EnhanceType* enhanceType);

	bool CanGetItem()
	{
		return m_canGetItem;
	}

	//アイテムを拾ったときのアイコンの生成、重さの追加
	void TakeItem(Item* item);

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

	std::vector<Item*> GetItemAddlessList()
	{
		std::vector<Item*> addList;
		for (Item item : m_itemList)
		{
			Item* i = &item;
			addList.push_back(i);
		}

		return addList;
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

	void AddAdvanceItemList(Item* item)
	{
		m_addItemList.push_back(item);
	}

	void CheckCanAddItem();
};