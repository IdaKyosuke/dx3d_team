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

	//アイテムを収納できるか
	if (m_haveItemCount >= MaxHaveItem)
	{
		m_canStorageItem = false;
	}
	else
	{
		m_canStorageItem = true;
	}

	//拾ったアイテムを認識する
	//認識してアイコンを生成
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
		//アイテム選択
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_Q))
		{
			m_takeItem--;
		}
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_E))
		{
			m_takeItem++;
		}
		//チェストからインベントリへ変更
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_C))
		{
			m_isChest = false;
			m_menuInventory->ChangeTakeInventory();
		}
	}

	m_takeItem = m_takeItem % MaxHaveItem;

	
	//持っていないアイテムとかを選択できないようにしたりなど
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

		//アイテムをチェストに格納する
		if (m_haveItemCount > 0)
			{
				//アイテムを捨てる			
			if (Input::GetInstance()->IsKeyDown(KEY_INPUT_R))
			{
				m_haveItemCount--;

				//捨てたアイテムが何番目のアイテムか
				m_destroyTakeItem = m_takeItem;

				m_destroyItemIcon = true;
										//vectorの中から捨てたアイテムのデータを消す
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