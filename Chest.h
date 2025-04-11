#pragma once
#include "Item.h"
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <vector>

class MenuInventory;

class Chest : public Node
{
private:
	static constexpr int MaxHaveItem = 30;

	static constexpr Vector2 TakeItemUiPos = Vector2(235, 560);

	int m_haveItemCount;	//�A�C�e���������Ă鐔
	int m_takeItem;			//�����Ԗڂ̃A�C�e���������Ă��邩

	int m_destroyTakeItem;		//�̂Ă��Ƃ��ǂ��̃A�C�e���������Ă�����

	bool m_canStorageItem;		//�A�C�e�������[�ł��邩

	bool m_isChest;		//�`�F�X�g��G���Ă���Ƃ�

	int m_itemNum;				//�A�C�e���̔ԍ��i�[�p
	bool m_destroyItemIcon;		//�A�C�R����������

	std::vector<Item> m_itemList;

	Sprite m_chestUi;
	Sprite m_takeItemUi;
	Transform m_transform;
	Transform m_takeItemTransform;	// �p��

	MenuInventory* m_menuInventory;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Chest(MenuInventory* menuInvenrtory);

	bool DestoryItemIcon()
	{
		return m_destroyItemIcon;
	}

	int DestroyTakeItem()
	{
		return m_destroyTakeItem;
	}

	void ChageTakeChest()
	{
		m_isChest = true;
	}
};
