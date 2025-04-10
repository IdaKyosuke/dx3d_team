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
	static constexpr int MaxHaveItem = 5;	//�A�C�e�������Ă�ő��
	static constexpr Vector2 TakeItemUiPos[MaxHaveItem] = {Vector2(60,890),Vector2(150,890), Vector2(240,890), Vector2(330,890), Vector2(420,890), };
	int m_haveItemCount;	//�A�C�e�����Ă邩�ǂ���

	int m_takeItem;		//�����̃A�C�e���������Ă��邩
	int m_destroyTakeItem;	//�̂Ă��Ƃ��ǂ��̃A�C�e���������Ă�����

	bool m_canGetItem;			//�A�C�e�����E���邩
	bool m_gettingItem;			//�A�C�e�����E������
	bool m_destroyItemIcon;		//�A�C�R����������

	int m_itemNum;

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

	bool DestoryItemIcon()
	{
		return m_destroyItemIcon;
	}

	int DestroyTakeItem()
	{
		return m_destroyTakeItem;
	}
};