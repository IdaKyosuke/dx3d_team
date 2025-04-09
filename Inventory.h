#pragma once
#include "Item.h"
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"
#include <list>
#include <vector>

//class Item;
class ItemIcon;

class Inventory : public Node
{
private:
	static constexpr int MaxHaveItem = 5;	//�A�C�e�������Ă�ő��
	int m_haveItemCount;	//�A�C�e�����Ă邩�ǂ���

<<<<<<< HEAD
	int m_takeItem;		//�����̃A�C�e���������Ă��邩

	bool m_canGetItem;
	bool m_gettingItem;

	int m_itemNum;

	//�A�C�e���i�[�p
	std::vector<Item> m_itemList;
=======
	const char* m_iconName[MaxHaveItem];

	bool m_canGetItem;		//�A�C�e�����E���邩
	bool m_gettingItem;

	std::list<Item> m_itemList;		//�A�C�e���i�[�p

	//Item m_itemlist[MaxHaveItem];
>>>>>>> origin/item
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
		m_itemList.push_back(*item);
	}
};