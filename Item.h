#pragma once
#include "Actor3D.h" 
#include "Vector3.h"

class LoadPlayer;
class Inventory;

struct ItemData
{
	char m_itemName[256];
	char m_modelName[256];
	char m_iconName[256];
	char m_flavorText[256];
	int m_sellMoney;
	int m_weight;
};

class Item : public Actor3D
{
public:
	enum class ItemNameList
	{
		GoldBar,
		Diamond,
		Potion,
		Bag,
		Coin,
		Watch,
		Powder,
		Scroll,
		Burger,
		KituneMusk,
		GoldCup,
		ChiliPepper,
		Rock,
		Gem,

		Length,
	};

private:
	int m_itemNumber;	//アイテムを識別するための番号格納用
	int m_model;		//アイテムのモデル
	const char* m_itemName;
	const char* m_flavorText;
	int m_sellMoney;	//売った時の金額
	int m_itemWeight;	//アイテムの重さ

	static constexpr Vector3 CanGetRange = Vector3(100, 100, 100);	//拾える範囲

	const char* m_iconName;

	bool m_canGetItem;

	Vector3 m_itemPos; 

	ItemData m_itemData;

	Inventory* m_inventory;
	LoadPlayer* m_player;

	float m_playerToDistance;	//プレイヤーとの距離

protected:
	virtual void Release();
	virtual void Update();
	virtual void Draw();

public:
	Item(
		int itemNum,
		ItemData* item = nullptr,
		Vector3 spownPos = Vector3(0, 0, 0), 
		Inventory* inventory = nullptr
	);

	int GetItemNum() const
	{
		return m_itemNumber;
	}

	int GetItemWeight() const
	{
		return m_itemData.m_weight;
	}

	int GetSellMoney() const
	{
		return m_itemData.m_sellMoney;
	}

	const char* GetItemName()
	{
		return m_itemData.m_itemName;
	}

	const char* GetItemIconName()
	{
		return m_itemData.m_iconName;
	}

	const char* GetFlavorText()
	{
		return m_itemData.m_flavorText;
	}

	ItemData* GetItemData()
	{
		return &m_itemData;
	}

	virtual void OnCollision(const Actor3D* other);

	static int GetItemTypeNum()
	{
		return static_cast<int>(ItemNameList::Length);
	}

	Item GetItem()
	{
		return *this;
	}

	// 自分を削除
	void DestroyMine();
};