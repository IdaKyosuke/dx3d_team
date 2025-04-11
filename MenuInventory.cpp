#include "MenuInventory.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "MenuItemIcon.h"
#include "LoadPlayer.h"
#include "Input.h"
#include "Chest.h"

MenuInventory::MenuInventory(Chest* chest) :
	m_haveItemCount(0),
	m_itemNum(0),
	m_destroyTakeItem(0),
	m_takeItem(0),
	m_destroyItemIcon(false),
	m_isIventory(true),
	m_chest(chest)
{
	m_transform.position = Screen::BottomCenter + Vector2(0, -70);
	m_menuInventoryUi.Register("inventoryUi.png");
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

	//アイテムを収納できるか
	if (m_haveItemCount >= MaxHaveItem)
	{

	}
	else
	{

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

			GetParent()->AddChild(new MenuItemIcon(m_itemNum, i,this));
		}
	}

	if (m_isIventory)
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
		//インベントリからチェストへ変更
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_C))
		{
			m_isIventory = false;
			m_chest->ChageTakeChest();
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
	}
	if (m_takeItem > countGetItem - 1)
	{
		m_takeItem = 0;
	}
	
	m_takeItemTransform.position = TakeItemUiPos + Vector2(90 * m_takeItem,0);


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

void MenuInventory::Draw()
{
	m_menuInventoryUi.Draw(m_transform);
	m_takeItemUi.Draw(m_takeItemTransform);
}