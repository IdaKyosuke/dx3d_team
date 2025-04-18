#include "Inventory.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "ItemIcon.h"
#include "LoadPlayer.h"
#include "Input.h"

Inventory::Inventory(LoadPlayer* player) :
	m_maxHaveItem(FirstMaxHaveItem),
	m_maxHaveWeight(FirstMaxHaveWeight),
	m_haveItemCount(0),
	m_canGetItem(false),
	m_itemNum(0),
	m_destroyTakeItem(0),
	m_gettingItem(false),
	m_player(player),
	m_takeItem(0),
	m_destroyItemIcon(false),
	m_canHaveWeight(0)
{
	
	m_inventoryUi.Register("inventory_ui.png");
	m_takeItemUi.Register("take_item.png");
}

void Inventory::Load()
{
	m_inventoryUi.Load();
	m_takeItemUi.Load();
}

void Inventory::Release()
{
	m_inventoryUi.Release();
	m_takeItemUi.Release();
}

void Inventory::Update()
{
	m_inventoryUi.Update();
	m_takeItemUi.Update();

	if (m_destroyItemIcon)
	{
		m_destroyItemIcon = false;
	}
	if (m_gettingItem)
	{
		m_gettingItem = false;
	}

	//�A�C�e�����E�����Ƃ��ł��邩
	if (m_haveItemCount >= m_maxHaveItem)
	{
		m_canGetItem = false;
	}
	else
	{
		m_canGetItem = true;
	}

	//�E�����A�C�e����F������
	//�F�����ăA�C�R���𐶐�
	m_haveItemCount = static_cast<int>(m_itemList.size());
	
	//�A�C�e���I��
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_Q))
	{
		m_takeItem--;
	}
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_E))
	{
		m_takeItem++;
	}

	m_takeItem = m_takeItem % m_maxHaveItem;

	if (m_haveItemCount > 0)
	{
		//�A�C�e�����̂Ă�
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_G))
		{
			m_haveItemCount--;

			//�̂Ă��I�u�W�F�N�g�𐶐�
			decltype(m_itemList)::iterator takeItem = std::next(m_itemList.begin(), m_takeItem);
			GetParent()->AddChild(new Item(takeItem->GetItemNum(), m_player->GetPosition(), this));

			//�̂Ă��A�C�e�������Ԗڂ̃A�C�e����
			m_destroyTakeItem = m_takeItem;

			m_destroyItemIcon = true;

			//vector�̒�����̂Ă��A�C�e���̃f�[�^������
			m_itemList.erase(m_itemList.begin() + m_takeItem);
		}
	}

	//�����Ă��Ȃ��A�C�e���Ƃ���I���ł��Ȃ��悤�ɂ�����Ȃ�
	if (m_takeItem < 0)
	{
		if (m_haveItemCount == 0)
		{
			m_takeItem = 0;
		}
		else
		{
			m_takeItem = static_cast<int>(m_haveItemCount - 1);
		}
	}
	if (m_takeItem > m_haveItemCount - 1)
	{
		m_takeItem = 0;
	}
	m_takeItemTransform.position = TakeItemUiPos;

	//Ui�̃|�W�V�����̐ݒ�
	m_takeItemTransform.position = TakeItemUiPos + Vector2(SquareSize * m_takeItem, 0);
}

void Inventory::Draw()
{
	for (int i = 0; i <= m_maxHaveItem -1; i++)
	{
		m_transform.position = InventoryUiPos + Vector2(SquareSize * i, 0);

		m_inventoryUi.Draw(m_transform);
	}
	m_takeItemUi.Draw(m_takeItemTransform);
}

void Inventory::TakeItem(int itemNum,float itemWeight)
{
	GetParent()->AddChild(new ItemIcon(itemNum, m_haveItemCount, this));

	m_canHaveWeight += itemWeight;
}