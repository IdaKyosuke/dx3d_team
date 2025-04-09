#pragma once
#include"Item.h"
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"
#include <list>

//class Item;

class Inventory : public Node
{
private:
	static constexpr int MaxHaveItem = 2;	//�A�C�e�������Ă�ő��
	int m_haveItemCount;	//�A�C�e�����Ă邩�ǂ���

	bool m_canGetItem;

	std::list<Item> m_itemList;		//�A�C�e���i�[�p
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
		m_haveItemCount--;
	}

	void SetItemList(Item* item)
	{
		m_itemList.push_front(*item);
	}
};