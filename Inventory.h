#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <list>
#include <vector>

class LoadPlayer;

class Inventory : public Node
{
private:
	static constexpr int FirstMaxHaveItem = 3;		//最初にアイテムを持てる数の最大量
	static constexpr float FirstMaxHaveWeight = 50;	//最初に持てるアイテムの重さの最大量
	static constexpr Vector2 TakeItemUiPos = Vector2(60,890);
	static constexpr Vector2 InventoryUiPos = Vector2(60, 890);


	static constexpr int SquareSize = 90;

	int m_maxHaveItem;
	int m_maxHaveWeight;

	int m_haveItemCount;	//アイテムを持ってる数

	int m_takeItem;			//今何のアイテムを持っているか
	int m_destroyTakeItem;	//捨てたときどこのアイテムを持っていたか

	float m_canHaveWeight;	//どれぐらいの重さまで持てるか

	bool m_canGetItem;			//アイテムを拾えるか
	bool m_gettingItem;			//アイテムを拾ったか
	bool m_destroyItemIcon;		//アイコンを消すか

	int m_itemNum;		//アイテムの番号格納用

	//アイテム格納用
	std::vector<Item> m_itemList;
	Sprite m_inventoryUi;
	Sprite m_takeItemUi;
	Transform m_transform;	// 姿勢
	Transform m_takeItemTransform;	// 姿勢

	LoadPlayer* m_player;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Inventory(LoadPlayer* player);

	bool CanGetItem()
	{
		return m_canGetItem;
	}

	//アイテムを同時に拾えるようにできたときに消す↓
	void GettingItem()
	{
		m_gettingItem = true;
	}
	//-----------------

	void SetItemList(Item* item)
	{
		m_itemList.push_back(*item);
	}

	bool DestoryItemIcon()
	{
		return m_destroyItemIcon;
	}

	int DestroyTakeItem()
	{
		return m_destroyTakeItem;
	}

	std::vector<Item> TakeItMenu()
	{
		return m_itemList;
	}

	void EnhanceInventory()
	{
		m_maxHaveItem++;
	}

	void SetMaxHaveItem(int maxHaveItem)
	{
		m_maxHaveItem = maxHaveItem;
	}

	int GetMaxHaveItem()
	{
		return m_maxHaveItem;
	}
};