#include "Chest.h"
#include "MenuInventory.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "ChestItemIcon.h"
#include "LoadPlayer.h"
#include "Input.h"

Chest::Chest(MenuInventory* menuInventory) :
	m_haveItemCount(0),
	m_itemNum(0),
	m_destroyTakeItem(0),
	m_takeItem(0),
	m_destroyItemIcon(false),
	m_canStorageItem(true),
	m_isChest(false),
	m_menuInventory(menuInventory)
{
	m_transform.position = Screen::Center + Vector2(0, 170);
	m_chestUi.Register("chest_Ui.png");
	m_takeItemUi.Register("take_item.png");
}

void Chest::Load()
{
	m_chestUi.Load();
	m_takeItemUi.Load();
}

void Chest::Release()
{
	m_chestUi.Release();
	m_takeItemUi.Release();
}

void Chest::Update()
{
	m_chestUi.Update();
	m_takeItemUi.Update();

	if (m_destroyItemIcon)
	{
		m_destroyItemIcon = false;
	}

	//�A�C�e�������[�ł��邩
	if (m_haveItemCount >= MaxHaveItem)
	{
		m_canStorageItem = false;
	}
	else
	{
		m_canStorageItem = true;
	}

	//�E�����A�C�e����F������
	//�F�����ăA�C�R���𐶐�
	auto countGetItem = std::distance(m_itemList.begin(), m_itemList.end());

	if (m_haveItemCount > 0)
	{

		for (int i = 0; i <= m_haveItemCount; i++)
		{
			decltype(m_itemList)::iterator getItem = std::next(m_itemList.begin(), i);
			m_itemNum = getItem->GetItemNum();

			GetParent()->AddChild(new ChestItemIcon(m_itemNum, i, this));
		}
	}

	if (m_isChest)
	{
		//�A�C�e���I��
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_Q))
		{
			m_takeItem--;
		}
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_E))
		{
			m_takeItem++;
		}
		//�`�F�X�g����C���x���g���֕ύX
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_C))
		{
			m_isChest = false;
			m_menuInventory->ChangeTakeInventory();
		}
	}

	m_takeItem = m_takeItem % MaxHaveItem;

	
	//�����Ă��Ȃ��A�C�e���Ƃ���I���ł��Ȃ��悤�ɂ�����Ȃ�
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
		if (m_takeItem > countGetItem - 1)
		{
			m_takeItem = 0;
		}

		//�A�C�e�����`�F�X�g�Ɋi�[����
		if (m_haveItemCount > 0)
			{
				//�A�C�e�����̂Ă�			
			if (Input::GetInstance()->IsKeyDown(KEY_INPUT_R))
			{
				m_haveItemCount--;

				//�̂Ă��A�C�e�������Ԗڂ̃A�C�e����
				m_destroyTakeItem = m_takeItem;

				m_destroyItemIcon = true;
										//vector�̒�����̂Ă��A�C�e���̃f�[�^������
				m_itemList.erase(m_itemList.begin() + m_takeItem);
			}
		}

	}

	int verticalCount = m_takeItem / 10;
	if (m_takeItem >= 10)
	{
		int takeItem = m_takeItem % 10;

		m_takeItemTransform.position = TakeItemUiPos + Vector2(90 * takeItem, 90 * verticalCount);
	}
	else
	{
		m_takeItemTransform.position = TakeItemUiPos + Vector2(90 * m_takeItem,0);
	}

		
}
	

void Chest::Draw()
{
	m_chestUi.Draw(m_transform);
	m_takeItemUi.Draw(m_takeItemTransform);
}