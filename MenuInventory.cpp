#include "MenuInventory.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "MenuItemIcon.h"
#include "LoadPlayer.h"
#include "Input.h"
#include "Chest.h"
#include "EnhanceType.h"

#include "Item.h"

MenuInventory::MenuInventory(Chest* chest,EnhanceType* enhanceType) :
	m_haveItemCount(0),
	m_itemNum(0),
	m_destroyTakeItem(0),
	m_takeItem(0),
	m_destroyItemIcon(false),
	m_isIventory(false),
	m_chest(chest),
	m_gettingItem(false),
	m_maxHaveItem(0),
	m_enhanceType(enhanceType)
{
	m_transform.position = Screen::BottomCenter + Vector2(0, -70);
	m_menuInventoryUi.Register("inventory_ui.png");
	m_takeItemUi.Register("take_item.png");	
}

void MenuInventory::Load()
{
	m_menuInventoryUi.Load();
	m_takeItemUi.Load();
}

void MenuInventory::Release()
{
	m_menuInventoryUi.Release();
	m_takeItemUi.Release();
}

void MenuInventory::Update()
{
	m_menuInventoryUi.Update();
	m_takeItemUi.Update();
		
	if (m_destroyItemIcon)
	{
		m_destroyItemIcon = false;
	}

	m_maxHaveItem = m_enhanceType->GetMaxHaveInventory();

	//�E�����A�C�e����F������
	//�F�����ăA�C�R���𐶐�
	m_haveItemCount = static_cast<int>(std::distance(m_itemList.begin(), m_itemList.end()));

	if (m_gettingItem)
	{
		GetParent()->AddChild(new MenuItemIcon(std::next(m_itemList.begin(), m_haveItemCount - 1)->GetItemNum(), 
			m_haveItemCount - 1, this));

		m_gettingItem = false;
	}
	//�A�C�R���̐���
	if(haveitem)
	{
		for (int i = 0; i <= m_haveItemCount - 1; i++)
		{
			GetParent()->AddChild(new MenuItemIcon(std::next(m_itemList.begin(),i)->GetItemNum(), i, this));
		}
		haveitem = false;
	}

	if (m_isIventory)
	{
		//�A�C�e���I��
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_A))
		{
			m_takeItem--;
		}
		else if (Input::GetInstance()->IsKeyDown(KEY_INPUT_D))
		{
			m_takeItem++;
		}

		//�C���x���g������`�F�X�g�֕ύX
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_C)&& !m_chest->GetTakeChest())
		{
			m_isIventory = false;

			m_chest->SetIsInventory(true);
		}

		//�A�C�e�����`�F�X�g�Ɋi�[����
		if (m_haveItemCount > 0 && m_chest->GetCanStorageItem())
		{
			if (Input::GetInstance()->IsKeyDown(KEY_INPUT_R))
			{
				//�i�[�����A�C�e�������Ԗڂ̃A�C�e����
				m_destroyTakeItem = m_takeItem;

				m_chest->Change(std::next(m_itemList.begin(), m_takeItem)->GetItemNum());

				m_destroyItemIcon = true;

				m_chest->StringingChest();

				//vector�̒�����̂Ă��A�C�e���̃f�[�^������
				m_itemList.erase(m_itemList.begin() + m_takeItem);
			}
		}
	}
	else
	{
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_C))
		{
			m_isIventory = true;

			m_chest->SetIsInventory(false);
		}
	}

	m_takeItem = m_takeItem % MaxHaveItem;
	
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
	
	m_takeItemTransform.position = TakeItemUiPos + Vector2(90 * m_takeItem,0);

	if (!m_isIventory && !m_chest->GetTakeChest())
	{
		m_isIventory = true;
	}
}

void MenuInventory::Draw()
{
	for (int i = 0; i <= m_maxHaveItem - 1; i++)
	{
		m_transform.position = InventoryUiPos + Vector2(SquareSize * i, 0);

		m_menuInventoryUi.Draw(m_transform);
	}
	m_takeItemUi.Draw(m_takeItemTransform);
}