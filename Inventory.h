#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <list>
#include <vector>

class LoadPlayer;

class Inventory : public Node
{
private:
	static constexpr int FirstMaxHaveItem = 3;		//�ŏ��ɃA�C�e�������Ă鐔�̍ő��
	static constexpr float FirstMaxHaveWeight = 50;	//�ŏ��Ɏ��Ă�A�C�e���̏d���̍ő��
	static constexpr Vector2 TakeItemUiPos = Vector2(60,890);
	static constexpr Vector2 InventoryUiPos = Vector2(60, 890);


	static constexpr int SquareSize = 90;

	int m_maxHaveItem;
	int m_maxHaveWeight;

	int m_haveItemCount;	//�A�C�e���������Ă鐔

	int m_takeItem;			//�����̃A�C�e���������Ă��邩
	int m_destroyTakeItem;	//�̂Ă��Ƃ��ǂ��̃A�C�e���������Ă�����

	float m_canHaveWeight;	//�ǂꂮ�炢�̏d���܂Ŏ��Ă邩

	bool m_canGetItem;			//�A�C�e�����E���邩
	bool m_gettingItem;			//�A�C�e�����E������
	bool m_destroyItemIcon;		//�A�C�R����������

	int m_itemNum;		//�A�C�e���̔ԍ��i�[�p

	//�A�C�e���i�[�p
	std::vector<Item> m_itemList;
	Sprite m_inventoryUi;
	Sprite m_takeItemUi;
	Transform m_transform;	// �p��
	Transform m_takeItemTransform;	// �p��

	LoadPlayer* m_player;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Inventory(LoadPlayer* player);

	bool CanGetItem()
	{
		return m_canGetItem;
	}

	//�A�C�e���𓯎��ɏE����悤�ɂł����Ƃ��ɏ�����
	void GettingItem()
	{
		m_gettingItem = true;
	}
	//-----------------

	void SetItemList(Item* item)
	{
		m_itemList.push_back(*item);
	}

	bool DestoryItemIcon()
	{
		return m_destroyItemIcon;
	}

	int DestroyTakeItem()
	{
		return m_destroyTakeItem;
	}

	std::vector<Item> TakeItMenu()
	{
		return m_itemList;
	}

	void EnhanceInventory()
	{
		m_maxHaveItem++;
	}

	void SetMaxHaveItem(int maxHaveItem)
	{
		m_maxHaveItem = maxHaveItem;
	}

	int GetMaxHaveItem()
	{
		return m_maxHaveItem;
	}
};