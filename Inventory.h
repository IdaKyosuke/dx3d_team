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
	static constexpr int MaxHaveItem = 5;	//アイテムを持てる最大量
	static constexpr Vector2 TakeItemUiPos[MaxHaveItem] = {Vector2(60,890),Vector2(150,890), Vector2(240,890), Vector2(330,890), Vector2(420,890), };
	int m_haveItemCount;	//アイテム持てるかどうか

	int m_takeItem;		//今何のアイテムを持っているか
	int m_destroyTakeItem;	//捨てたときどこのアイテムを持っていたか

	bool m_canGetItem;			//アイテムを拾えるか
	bool m_gettingItem;			//アイテムを拾ったか
	bool m_destroyItemIcon;		//アイコンを消すか

	int m_itemNum;

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

	bool DestoryItemIcon()
	{
		return m_destroyItemIcon;
	}

	int DestroyTakeItem()
	{
		return m_destroyTakeItem;
	}
};