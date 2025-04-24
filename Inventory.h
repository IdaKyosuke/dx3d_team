#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <list>
#include <vector>

class Inventory : public Node
{
private:
	static constexpr float FirstMaxHaveWeight = 50;	//�ŏ��Ɏ��Ă�A�C�e���̏d���̍ő��
	static constexpr Vector2 TakeItemUiPos = Vector2(60,890);
	static constexpr Vector2 InventoryUiPos = Vector2(60, 890);


	static constexpr int SquareSize = 90;

	int m_maxHaveItem;
	float m_maxHaveWeight;

	int m_haveItemCount;	//�A�C�e���������Ă鐔

	int m_takeItem;			//�����̃A�C�e���������Ă��邩
	int m_dropItemNum;			//�̂Ă��A�C�e���̔ԍ�

	float m_canHaveWeight;	//�ǂꂮ�炢�̏d���܂Ŏ��Ă邩

	bool m_canGetItem;			//�A�C�e�����E���邩
	bool m_gettingItem;			//�A�C�e�����E������
	bool m_dropItem;			//�A�C�e���𗎂Ƃ�����
	bool m_dropItemHoge;

	int m_itemNum;		//�A�C�e���̔ԍ��i�[�p

	//�A�C�e���i�[�p
	std::vector<Item> m_itemList;
	std::vector<Item> m_advanceItemList;
	Sprite m_inventoryUi;
	Sprite m_takeItemUi;
	Transform m_transform;	// �p��
	Transform m_takeItemTransform;	// �p��


protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Inventory(int maxHaveItem);

	bool CanGetItem()
	{
		return m_canGetItem;
	}
	
	void SetItemList(int itemNum)
	{
		Item item = Item(itemNum);

		if (m_maxHaveItem >= m_haveItemCount)
		{
			m_advanceItemList.push_back(item);
		}
	}

	void ItemListSet();

	void TakeItem(int itemNum);

	bool GetItemNow()
	{
		return m_gettingItem;
	}

	void GettingItem()
	{
		m_gettingItem = true;
	}

	bool GetDropItem()
	{
		return m_dropItem;
	}

	int GetDropItemNum()
	{
		return m_dropItemNum;
	}

	void GetDropItemHoge()
	{
		m_dropItemHoge = true;
	}

	std::vector<Item> GetItemList()
	{
		return m_itemList;
	}

	int GetTakeItem()
	{
		return m_takeItem;
	}

	void EnhanceInventory()
	{
		m_maxHaveItem++;
	}

	int GetMaxHaveItem()
	{
		return m_maxHaveItem;
	}

	int GetHaveItemCount()
	{
		return m_haveItemCount;
	}

	void AddItemCount()
	{
		m_haveItemCount++;
	}
};