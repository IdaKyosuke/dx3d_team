#pragma once
#include "ItemDate.h"
#include "Actor3D.h" 
#include "Vector3.h"

class LoadPlayer;
class Inventory;

class Item : public Actor3D
{
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

		Length,
	};

	//アイテムの情報
	struct ItemDate itemDate[static_cast<int>(ItemNameList::Length)] =
	{
		ItemDate("Resource/Item/gold.mv1","Gold",100,30,
			"ただの金の延べ棒。ずっしりと重い"),
		ItemDate("Resource/Item/diamond.mv1","Diamond",200,30,
			"キラキラと光るダイヤモンド。"),
		ItemDate("Resource/Item/potion.mv1","Potion",10,20,
			"謎のポーション。匂いはここに来る\n前に飲んだエナジードリンクの匂い\nがする。"),
		ItemDate("Resource/Item/bag.mv1","Bag",40, 25,
			"布で出来た巾着。\n中には何も入っていない。"),
		ItemDate("Resource/Item/coin.mv1","Coin",20,10,
			"どこの国の硬貨なのかわからない\nコイン。"),
		ItemDate("Resource/Item/watch.mv1","Watch",60,15,
			"もう動いていない時計。\n随分と前のものだ。"),
		ItemDate("Resource/Item/powder.mv1","Powder",20,5,
			"白い粉。これに限る。"),
		ItemDate("Resource/Item/scroll.mv1","Scroll",50,5,
			"ただの巻物。\n（この巻を解く者よ。知識は力なれ\nど、代償を伴う。覚悟なき者、ここ\nより先へ進むべからず。ｇじゃいおｊｆｄｈそご")
	};

	const char* IconName[static_cast<int>(ItemNameList::Length)] = 
	{ "goldbar_icon.png","diamond_icon.png","potion_icon.png","bag_icon.png","coin_icon.png","watch_icon.png","powder_icon.png","scroll_icon.png"};

	Inventory* m_inventory;
	LoadPlayer* m_player;

	float m_playerToDistance;	//プレイヤーとの距離

protected:
	virtual void Release();
	virtual void Update();
	virtual void Draw();

public:
	Item(int itemNumber, Vector3 spownPos = Vector3(0, 0, 0), Inventory* inventory = nullptr,LoadPlayer* player = nullptr);

	int GetItemNum() const
	{
		return m_itemNumber;
	}

	int GetItemWeight() const
	{
		return m_itemWeight;
	}

	int GetSellMoney() const
	{
		return m_sellMoney;
	}

	const char* GetItemName()
	{
		return m_itemName;
	}

	const char* GetItemIconName()
	{
		return m_iconName;
	}

	const char* GetFlavorText()
	{
		return m_flavorText;
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
};