#pragma once
#include "ItemDate.h"
#include "Actor.h" 
#include "Vector3.h"
#include <list>

#include <cmath>

class LoadPlayer;
class Inventory;

class Item : public Actor
{
private:
	int m_itemNumber;	//�A�C�e�������ʂ��邽�߂̔ԍ��i�[�p
	int m_model;		//�A�C�e���̃��f��
	const char* m_itemName;
	const char* m_iconName;
	int m_sellMoney;	//���������̋��z


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
		ItemDate("Resource/Item/goldBar.mv1","GoldBar","ItemIcon/goldBar_icon.png",100),
		ItemDate("Resource/Item/diamond.mv1","Diamond","ItemIcon/diamond_icon.png",200),
		ItemDate("Resource/Item/potion.mv1","Potion","ItemIcon/potion_icon.png",10),
	};

	 LoadPlayer* m_player;
	 Inventory* m_inventory;

	 float m_playerToDistance;	//�v���C���[�Ƃ̋���

protected:
	virtual void Release();
	virtual void Update();
	virtual void Draw();

public:
	Item(int itemNumber,Vector3 spownPos,LoadPlayer* player,Inventory* inventory);

	float Distance(Vector3 a, Vector3 b)
	{
		float num = a.x - b.x;
		float num2 = a.y - b.y;
		float num3 = a.z - b.z;
		return std::sqrt(num * num + num2 * num2 + num3 * num3);
	}
};