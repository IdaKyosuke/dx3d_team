#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <vector>

class Chest : public Node
{
private:
	static constexpr int MaxHaveItem = 30;
	static constexpr Vector2 TakeItemUiPos = Vector2(235, 560);

	int m_haveItemCount;	//アイテムを持ってる数
	int m_takeItem;			//今何番目のアイテムを持っているか

	int m_destroyTakeItem;		//捨てたときどこのアイテムを持っていたか
	int m_destroyItemIconCount;	//何個アイテムのアイコンを消したか

	bool m_canStorageItem;		//アイテムを収納できるか
	bool m_storagingItem;		//アイテムを収納した時

	bool m_isChest;		//チェストを触っているとき

	int m_itemNum;				//アイテムの番号格納用
	bool m_destroyItemIcon;		//アイコンを消すか


	int m_seChest;	//サウンド

	std::vector<Item> m_itemList;
	std::vector<Item*> m_addItemList;
	std::vector<Item> m_lostItemList;

	Sprite m_chestUi;
	Sprite m_takeItemUi;
	Transform m_transform;
	Transform m_takeItemTransform;	// 姿勢

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Chest();

	bool DestroyItemIcon()
	{
		return m_destroyItemIcon;
	}

	void AddDestroyItemIconCount()
	{
		m_destroyItemIconCount++;
	}

	int DestroyTakeItem()
	{
		return m_destroyTakeItem;
	}

	bool GetTakeChest()
	{
		return	m_isChest;
	}

	bool GetCanStorageItem()
	{
		return m_canStorageItem;
	}

	void SetIsInventory(bool nowTake)
	{
		m_isChest = nowTake;
	}

	void SetItemList(int itemNum)
	{
		Item m_item = Item(itemNum);

		m_itemList.push_back(m_item);
	}

	void  StringingChest()
	{
		m_storagingItem = true;
	}

	std::vector<Item> GetItemList()
	{
		return m_itemList;
	}

	std::vector<Item> GetLostItemList()
	{
		return m_lostItemList;
	}

	int GetTakeItem() const
	{
		return m_takeItem;
	}

	void CreateIcon(Item* item);

	void AddItemCount()
	{
		m_haveItemCount++;
	}

	void AddAdvanceItemList(int itemNum, ItemData* itemData)
	{
		Item* item = new Item(itemNum, itemData);

		m_addItemList.push_back(item);
	}

	void CheckCanAddItem();

	void LostItem(int loatItemNum)
	{
		m_destroyTakeItem = loatItemNum;
		m_destroyItemIcon = true;

		//一番最後のアイテム消したらエラーが起こったから起こらないようにしてる
		if (m_takeItem == m_itemList.size()-1)
		{
			m_takeItem = m_takeItem -1;
		}

		m_haveItemCount--;

		m_itemList.erase(m_itemList.begin() + loatItemNum);
	}
};