#pragma once
#include "Item.h"
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"
#include <list>

//class Item;
class ItemIcon;

class Inventory : public Node
{
private:
	static constexpr int MaxHaveItem = 2;	//�A�C�e�������Ă�ő��
	int m_haveItemCount;	//�A�C�e�����Ă邩�ǂ���

	const char* m_iconName[MaxHaveItem];

	bool m_canGetItem;		//�A�C�e�����E���邩
	bool m_gettingItem;

	std::list<Item> m_itemList;		//�A�C�e���i�[�p

	//Item m_itemlist[MaxHaveItem];
	Sprite m_inventoryUi;
	Transform m_transform;	// �p��

	ItemIcon* m_itemIcon;

	//Item m_item[];

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
		m_itemList.push_front(*item);
	}
};