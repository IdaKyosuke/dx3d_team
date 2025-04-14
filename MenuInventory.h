#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <vector>

class Chest;

class MenuInventory : public Node
{
private:
	static constexpr int MaxHaveItem = 5;
	static constexpr Vector2 TakeItemUiPos = Vector2(460,890);


	int m_haveItemCount;	//�A�C�e���������Ă鐔
	int m_takeItem;			//�����Ԗڂ̃A�C�e���������Ă��邩

	int m_destroyTakeItem;	//�̂Ă��Ƃ��ǂ��̃A�C�e���������Ă�����

	bool m_isIventory;		//�C���x���g����G���Ă�Ƃ�

	int m_itemNum;		//�A�C�e���̔ԍ��i�[�p
	bool m_destroyItemIcon;		//�A�C�R����������

	int m_inum[MaxHaveItem];

	std::vector<Item> m_itemList;

	Sprite m_menuInventoryUi;
	Sprite m_takeItemUi;
	Transform m_transform;
	Transform m_takeItemTransform;	// �p��

	Chest* m_chest;

	bool haveitem = m_itemList.empty();

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	MenuInventory(Chest* chest);

	bool DestoryItemIcon() 
	{
		return m_destroyItemIcon;
	}

	int DestroyTakeItem()
	{
		return m_destroyTakeItem;
	}

	void ChangeTakeInventory()
	{
		m_isIventory = true;
	}

	/*
	void BroughtItMenu(Item* item)
	{
		m_itemList.push_back(*item);
	}

	*/
	void BroughtItMenu(int num,int hoge)
	{
		m_inum[hoge] = num;
	}

	void Change(int itemNum)
	{
		Item m_item = Item(itemNum);

		m_itemList.push_back(m_item);
	}
};