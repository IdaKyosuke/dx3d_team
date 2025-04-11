#include "Inventory.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "ItemIcon.h"
#include "LoadPlayer.h"
#include "Input.h"

Inventory::Inventory(LoadPlayer* player) :
	m_haveItemCount(0),
	m_canGetItem(false),
	m_itemNum(0),
	m_destroyTakeItem(0),
	m_gettingItem(false),
	m_player(player),
<<<<<<< HEAD
	m_takeItem(1000),
=======
	m_takeItem(0),
>>>>>>> collision
	m_destroyItemIcon(false)
{
	m_transform.position = Screen::BottomCenter + Vector2(-400, -70);
	m_inventoryUi.Register("inventoryUi.png");
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

	//�A�C�e�����E�����Ƃ��ł��邩
	if (m_haveItemCount >= MaxHaveItem)
	{
		m_canGetItem = false;
	}
	else
	{
		m_canGetItem = true;
	}

	//�E�����A�C�e����F������
	//�F�����ăA�C�R���𐶐�
<<<<<<< HEAD
	int countGetItem = std::distance(m_itemList.begin(), m_itemList.end());
=======
	auto countGetItem = std::distance(m_itemList.begin(), m_itemList.end());
>>>>>>> collision
	if (m_gettingItem)
	{
		decltype(m_itemList)::iterator getItem = std::next(m_itemList.begin(), countGetItem - 1);
		m_itemNum = getItem->GetItemNum();

<<<<<<< HEAD
		GetParent()->AddChild(new ItemIcon(m_itemNum, countGetItem - 1,this));
=======
		GetParent()->AddChild(new ItemIcon(m_itemNum, static_cast<int>(countGetItem - 1),this));
>>>>>>> collision

		m_gettingItem = false;
	}

	//�A�C�e���I��
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_Q))
	{
		m_takeItem--;
	}
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_E))
	{
		m_takeItem++;
	}

<<<<<<< HEAD
	if (m_takeItem < 0)
	{
		m_takeItem = 0;
	}
	m_takeItemTransform.position = TakeItemUiPos[m_takeItem % MaxHaveItem];

	if (m_haveItemCount > 0)
	{
=======
	m_takeItem = m_takeItem % MaxHaveItem;

	if (m_takeItem < 0)
	{
		if (countGetItem == 0)
		{
			m_takeItem = 0;
		}
		else
		{
			m_takeItem = static_cast<int>(countGetItem - 1);
		}
	}
	if (m_takeItem > countGetItem - 1)
	{
		m_takeItem = 0;
	}
	m_takeItemTransform.position = TakeItemUiPos[m_takeItem];

	if (m_haveItemCount > 0)
	{
		

>>>>>>> collision
		//�A�C�e�����̂Ă�
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_R))
		{
			m_haveItemCount--;

<<<<<<< HEAD
			Vector3 dropPos = m_player->PlayerPos();

			

			//�̂Ă��I�u�W�F�N�g�𐶐�
			decltype(m_itemList)::iterator takeItem = std::next(m_itemList.begin(), (m_takeItem % MaxHaveItem));
			GetParent()->AddChild(new Item(takeItem->GetItemNum(), dropPos, m_player, this));

			//�̂Ă��A�C�e�������Ԗڂ̃A�C�e����
			m_destroyTakeItem = m_takeItem % MaxHaveItem;
=======
			Vector3 dropPos = m_player->GetPosition();

			//�̂Ă��I�u�W�F�N�g�𐶐�
			decltype(m_itemList)::iterator takeItem = std::next(m_itemList.begin(), (m_takeItem));
			GetParent()->AddChild(new Item(takeItem->GetItemNum(), dropPos, m_player, this));

			//�̂Ă��A�C�e�������Ԗڂ̃A�C�e����
			m_destroyTakeItem = m_takeItem;
>>>>>>> collision

			m_destroyItemIcon = true;
			
			//vector�̒�����̂Ă��A�C�e���̃f�[�^������
<<<<<<< HEAD
			m_itemList.erase(m_itemList.begin() + m_takeItem % MaxHaveItem);
=======
			m_itemList.erase(m_itemList.begin() + m_takeItem);
>>>>>>> collision
		}
	}
}

void Inventory::Draw()
{
	m_inventoryUi.Draw(m_transform);
	m_takeItemUi.Draw(m_takeItemTransform);
}