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

	bool m_canGetItem;

	Vector3 m_itemPos;

	enum class ItemNameList
	{
		GoldBar,
		Diamond,
		Potion,

		Length,
	};

	//�A�C�e���̏��
	struct ItemDate itemDate[static_cast<int>(ItemNameList::Length)] =
	{
		ItemDate("Resource/Item/iron.mv1","Iron",100,30),
		ItemDate("Resource/Item/gem.mv1","Gem",200,30),
		ItemDate("Resource/Item/potion.mv1","Potion",10,20),
	};

	Inventory* m_inventory;

	float m_playerToDistance;	//�v���C���[�Ƃ̋���

protected:
	virtual void Release();
	virtual void Update();
	virtual void Draw();

public:
	Item(int itemNumber, Vector3 spownPos = Vector3(0, 0, 0), Inventory* inventory = nullptr);

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

	virtual void OnCollision(const Actor3D* other);

	static int GetItemTypeNum()
	{
		return static_cast<int>(ItemNameList::Length);
	}
};