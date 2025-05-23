#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <vector>

class Chest;
class EnhanceType;

class MenuInventory : public Node
{
private:
	static constexpr int MaxHaveItem = 5;
	static constexpr Vector2 TakeItemUiPos = Vector2(250,870);
	static constexpr Vector2 InventoryUiPos = Vector2(250, 870);
	static constexpr Vector2 SlashUiPos = Vector2(105, 870);
	static constexpr Vector2 FontSize = Vector2(20, 35);	// 数字1文字の幅・高さ
	static constexpr int FontMargin = 5;					// 数字と数字の間の余白

	static constexpr int SquareSize = 90;

	int m_maxHaveItem;

	int m_haveItemCount;	//アイテムを持ってる数
	int m_takeItem;			//今何番目のアイテムを持っているか

	int m_destroyTakeItem;	//捨てたときどこのアイテムを持っていたか
	bool m_gettingItem;		//アイテムをチェストからインベントリを持ってきたか

	bool m_isIventory;		//インベントリを触ってるとき

	int m_itemNum;		//アイテムの番号格納用
	bool m_destroyItemIcon;		//アイコンを消すか
	int m_nowHaveWeight;

	int m_seInventory;	//サウンド

	std::vector<Item> m_itemList;
	std::vector<Item*> m_addItemList;

	Sprite m_menuInventoryUi;
	Sprite m_takeItemUi;
	Sprite m_slashUi;
	Transform m_transform;
	Transform m_takeItemTransform;	// 姿勢
	Transform m_slashTransform;	// 姿勢

	Chest* m_chest;
	EnhanceType* m_enhanceType;

	int m_fontTextureId;	// 数字フォント
	Transform m_haveWeightTransform;	// 姿勢

	bool haveitem = m_itemList.empty();

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	MenuInventory(Chest* chest,EnhanceType* enhancetype);

	bool DestoryItemIcon() 
	{
		return m_destroyItemIcon;
	}

	int DestroyTakeItem()
	{
		return m_destroyTakeItem;
	}

	bool GetTakeInventory()
	{
		return m_isIventory;
	}

	void SetIsInventory(bool nowTake)
	{
		m_isIventory = nowTake;
	}

	void GettingItem() 
	{
		m_gettingItem = true;
	}
	
	std::vector<Item> GetItemList()
	{
		return m_itemList;
	}

	void Change(Item item)
	{
		m_itemList.push_back(item);
	}

	void AddAdvanceItemList(Item* item)
	{
		m_addItemList.push_back(item);
	}

	void AddItemCount()
	{
		m_haveItemCount++;
	}

	//アイテムを拾ったときのアイコンの生成、重さの追加
	void TakeItem(Item* item);

	void CheckCanAddItem();
};