#pragma once
#include "ItemDate.h"
#include "Actor.h" 
#include "Vector3.h"
#include <list>
#include <cmath>
#include "Actor3D.h" 

class LoadPlayer;
class Inventory;

class Item : public Actor3D
{
private:
	int m_itemNumber;	//�A�C�e�������ʂ��邽�߂̔ԍ��i�[�p
	int m_model;		//�A�C�e���̃��f��
	const char* m_itemName;
	int m_sellMoney;	//���������̋��z




	static constexpr Vector3 CanGetRange = Vector3(100, 100, 100);	//�E����͈�

	bool m_canGetItem;

	Vector3 m_itemPos;

	enum class ItemNameList
	{
		GoldBar,
		Diamond,
		Potion,
		Bag,
		Watch,
		Coin,

		Length,
	};

	//�A�C�e���̏��
	 struct ItemDate itemDate[static_cast<int>(ItemNameList::Length)] =
	{
		ItemDate("Resource/Item/gold.mv1","Iron",100),
		ItemDate("Resource/Item/diamond.mv1","Gem",200),
		ItemDate("Resource/Item/potion.mv1","Potion",10),
		ItemDate("Resource/Item/bag.mv1","Bag",20),
		ItemDate("Resource/Item/watch.mv1","Watch",150),
		ItemDate("Resource/Item/coin.mv1","Coin",50),
	};

	 LoadPlayer* m_player;
	 Inventory* m_inventory;

	 float m_playerToDistance;	//�v���C���[�Ƃ̋���

protected:
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Item(int itemNumber,Vector3 spownPos,LoadPlayer* player,Inventory* inventory);

	float Distance(Vector3 a, Vector3 b)
	{
		float num = a.x - b.x;
		float num2 = a.y - b.y;
		float num3 = a.z - b.z;
		return std::sqrt(num * num + num2 * num2 + num3 * num3);
	}

	int GetItemNum()
	{
		return m_itemNumber;
	}

	virtual void OnCollision(const Actor3D* other);
};