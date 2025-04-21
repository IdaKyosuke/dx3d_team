#include "Chest.h"
#include "MenuInventory.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "ChestItemIcon.h"
#include "LoadPlayer.h"
#include "Input.h"

Chest::Chest() :
	m_haveItemCount(0),
	m_itemNum(0),
	m_destroyTakeItem(0),
	m_takeItem(0),
	m_destroyItemIcon(false),
	m_canStorageItem(true),
	m_isChest(false),
	m_storagingItem(false)
{
	m_transform.position = Screen::Center + Vector2(0, 170);
	m_chestUi.Register("chest_ui.png");
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

	m_haveItemCount = static_cast<int>(m_itemList.size());

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
	if (m_storagingItem)
	{
		GetParent()->AddChild(new ChestItemIcon(std::next(m_itemList.begin(), m_haveItemCount - 1)->GetItemNum(),
			m_haveItemCount - 1, this));

		m_storagingItem = false;
	}

	if (m_isChest)
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
		else if (Input::GetInstance()->IsKeyDown(KEY_INPUT_S))
		{
			m_takeItem += 10;
		}
		else if (Input::GetInstance()->IsKeyDown(KEY_INPUT_W))
		{
			m_takeItem -= 10;
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
			m_takeItem = m_haveItemCount - 1;
		}
	}
	if (m_takeItem > m_haveItemCount - 1)
	{
		m_takeItem = 0;
	}

	//����
	int verticalCount = m_takeItem / 10;
	if (m_takeItem >= 10)
	{
		int takeItem = m_takeItem % 10;

		m_takeItemTransform.position = TakeItemUiPos + Vector2(90 * takeItem, 90 * verticalCount);
	}
	else
	{
		m_takeItemTransform.position = TakeItemUiPos + Vector2(90 * m_takeItem, 0);
	}
}

void Chest::Draw()
{
	m_chestUi.Draw(m_transform);
	m_takeItemUi.Draw(m_takeItemTransform);
}

void Chest::CreateIcon(int ItemNum)
{
	GetParent()->AddChild(new ChestItemIcon(std::next(m_itemList.begin(), ItemNum)->GetItemNum(),
		ItemNum, this));
}