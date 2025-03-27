#pragma once
#include "ItemDate.h"
#include "Actor.h" 
#include "Vector3.h"
#include <list>

#include <cmath>

class LoadPlayer;

class Item : public Actor
{
private:
	int m_itemNumber;	//�A�C�e�������ʂ��邽�߂̔ԍ��i�[�p
	int m_model;		//�A�C�e���̃��f��
	int m_sellMoney;	//���������̋��z

	//static constexpr Vector3 SpownPos =  Vector3(930, 40, 90);

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
		ItemDate("Resource/Item/GoldBar.mv1","GoldBar",100),
		ItemDate("Resource/Item/Diamond.mv1","Diamond",200),
		ItemDate("Resource/Item/Potion.mv1","Potion",10),
	};

	 LoadPlayer* m_player;

	 float m_playerToDistance;

protected:
	virtual void Load();
	virtual void Release();
	virtual void Update();
	virtual void Draw();

public:
	Item(int itemNumber,Vector3 spownPos,LoadPlayer* player);

	float Distance(Vector3 a, Vector3 b)
	{
		float num = a.x - b.x;
		float num2 = a.y - b.y;
		float num3 = a.z - b.z;
		return std::sqrt(num * num + num2 * num2 + num3 * num3);
	}
};