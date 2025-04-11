#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <vector>

class MenuInventory;

class Chest : public Node
{
private:
	static constexpr int MaxHaveItem = 30;

	static constexpr Vector2 TakeItemUiPos = Vector2(235, 560);

	int m_haveItemCount;	//アイテムを持ってる数
	int m_takeItem;			//今何番目のアイテムを持っているか

	int m_destroyTakeItem;		//捨てたときどこのアイテムを持っていたか

	bool m_canStorageItem;		//アイテムを収納できるか

	bool m_isChest;		//チェストを触っているとき

	int m_itemNum;				//アイテムの番号格納用
	bool m_destroyItemIcon;		//アイコンを消すか

	std::vector<Item> m_itemList;

	Sprite m_chestUi;
	Sprite m_takeItemUi;
	Transform m_transform;
	Transform m_takeItemTransform;	// 姿勢

	MenuInventory* m_menuInventory;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Chest(MenuInventory* menuInvenrtory);

	bool DestoryItemIcon()
	{
		return m_destroyItemIcon;
	}

	int DestroyTakeItem()
	{
		return m_destroyTakeItem;
	}

	void ChageTakeChest()
	{
		m_isChest = true;
	}
};
