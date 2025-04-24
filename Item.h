#pragma once
#include "ItemDate.h"
#include "Actor3D.h" 
#include "Vector3.h"

class LoadPlayer;
class Inventory;

class Item : public Actor3D
{
private:
	int m_itemNumber;	//�A�C�e�������ʂ��邽�߂̔ԍ��i�[�p
	int m_model;		//�A�C�e���̃��f��
	const char* m_itemName;
	int m_sellMoney;	//���������̋��z
	int m_itemWeight;	//�A�C�e���̏d��

	static constexpr Vector3 CanGetRange = Vector3(100, 100, 100);	//�E����͈�

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

		Length,
	};

	//�A�C�e���̏��
	struct ItemDate itemDate[static_cast<int>(ItemNameList::Length)] =
	{
		ItemDate("Resource/Item/gold.mv1","Iron",100,30),
		ItemDate("Resource/Item/diamond.mv1","Diamond",200,30),
		ItemDate("Resource/Item/potion.mv1","Potion",10,20),
		ItemDate("Resource/Item/bag.mv1","Bag",40, 25),
		ItemDate("Resource/Item/coin.mv1","Coin",20,10),
		ItemDate("Resource/Item/watch.mv1","Watch",60,15),
	};

	const char* IconName[static_cast<int>(ItemNameList::Length)] = 
	{ "iron_icon.png","gem_icon.png","potion_icon.png","bag_icon.png","coin_icon.png","watch_icon.png" };

	Inventory* m_inventory;
	LoadPlayer* m_player;

	float m_playerToDistance;	//�v���C���[�Ƃ̋���

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