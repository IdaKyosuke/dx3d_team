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
	static constexpr int MaxHaveItem = 5;	//�A�C�e�������Ă�ő��
	int m_haveItemCount;	//�A�C�e�����Ă邩�ǂ���

	int m_takeItem;		//�����̃A�C�e���������Ă��邩

	bool m_canGetItem;
	bool m_gettingItem;

	int m_itemNum;

	//�A�C�e���i�[�p
	std::vector<Item> m_itemList;
	Sprite m_inventoryUi;
	Transform m_transform;	// �p��

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

	void GettingItem()
	{
		m_gettingItem = true;
	}

	void SetItemList(Item* item)
	{
		m_itemList.push_back(*item);
	}
};