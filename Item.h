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
	int m_sellMoney;	//売った時の金額

	static constexpr Vector3 CanGetRange = Vector3(100, 100, 100);	//拾える範囲

	bool m_canGetItem;

	Vector3 m_itemPos;

	enum class ItemNameList
	{
		GoldBar,
		Diamond,
		Potion,

		Length,
	};

	//アイテムの情報
	 struct ItemDate itemDate[static_cast<int>(ItemNameList::Length)] =
	{
		ItemDate("Resource/Item/iron.mv1","Iron",100),
		ItemDate("Resource/Item/gem.mv1","Gem",200),
		ItemDate("Resource/Item/potion.mv1","Potion",10),
	};

	 Inventory* m_inventory;

	 float m_playerToDistance;	//プレイヤーとの距離

protected:
	virtual void Release();
	virtual void Update();
	virtual void Draw();

public:
	Item(int itemNumber,Vector3 spownPos = Vector3(0,0,0), Inventory* inventory = nullptr);

	int GetItemNum() const
	{
		return m_itemNumber;
	}

	virtual void OnCollision(const Actor3D* other);
};