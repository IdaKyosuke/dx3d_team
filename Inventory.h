#pragma once
#include"Item.h"
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"
#include <list>
#include <vector>

//class Item;

class Inventory : public Node
{
private:
	static constexpr int MaxHaveItem = 2;	//�A�C�e�������Ă�ő��
	int m_haveItemCount;	//�A�C�e�����Ă邩�ǂ���

	bool m_canGetItem;

	const char* m_iconName[3] = {"goldBar_icon.png","gem.png","gem.png"};

	int m_itemNum;

	//std::list<Item> m_itemList;		//�A�C�e���i�[�p
	std::vector<Item> m_itemList;
	Sprite m_inventoryUi;
	Sprite m_iconUi;
	Transform m_transform;	// �p��
	Transform m_Icontransform;	// �p��

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Inventory();

	bool CanGetItem()
	{
		return m_canGetItem;
	}

	void OnInventory()
	{
		m_haveItemCount++;
	}

	void SetItemList(Item* item)
	{
		m_itemList.push_back(*item);
	}
};