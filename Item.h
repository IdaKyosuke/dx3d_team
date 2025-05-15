#pragma once
#include "ItemDate.h"
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
private:
	int m_itemNumber;	//�A�C�e�������ʂ��邽�߂̔ԍ��i�[�p
	int m_model;		//�A�C�e���̃��f��
	const char* m_itemName;
	const char* m_flavorText;
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
		Powder,
		Scroll,

		Length,
	};

	ItemData m_itemData[static_cast<int>(ItemNameList::Length)];

	Inventory* m_inventory;
	LoadPlayer* m_player;

	float m_playerToDistance;	//�v���C���[�Ƃ̋���

protected:
	virtual void Release();
	virtual void Update();
	virtual void Draw();

public:
	Item(int itemNumber, Vector3 spownPos = Vector3(0, 0, 0), Inventory* inventory = nullptr,LoadPlayer* player = nullptr);

	void LoadItemData();

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

	// �������폜
	void DestroyMine();
};