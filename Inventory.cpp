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
	m_takeItem(1000),
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

	//アイテムを拾うことができるか
	if (m_haveItemCount >= MaxHaveItem)
	{
		m_canGetItem = false;
	}
	else
	{
		m_canGetItem = true;
	}

	//拾ったアイテムを認識する
	//認識してアイコンを生成
	int countGetItem = std::distance(m_itemList.begin(), m_itemList.end());
	if (m_gettingItem)
	{
		decltype(m_itemList)::iterator getItem = std::next(m_itemList.begin(), countGetItem - 1);
		m_itemNum = getItem->GetItemNum();

		GetParent()->AddChild(new ItemIcon(m_itemNum, countGetItem - 1,this));

		m_gettingItem = false;
	}

	//アイテム選択
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_Q))
	{
		m_takeItem--;
	}
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_E))
	{
		m_takeItem++;
	}

	if (m_takeItem < 0)
	{
		m_takeItem = 0;
	}
	m_takeItemTransform.position = TakeItemUiPos[m_takeItem % MaxHaveItem];

	if (m_haveItemCount > 0)
	{
		//アイテムを捨てる
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_R))
		{
			m_haveItemCount--;

			Vector3 dropPos = m_player->PlayerPos();

			

			//捨てたオブジェクトを生成
			decltype(m_itemList)::iterator takeItem = std::next(m_itemList.begin(), (m_takeItem % MaxHaveItem));
			GetParent()->AddChild(new Item(takeItem->GetItemNum(), dropPos, m_player, this));

			//捨てたアイテムが何番目のアイテムか
			m_destroyTakeItem = m_takeItem % MaxHaveItem;

			m_destroyItemIcon = true;
			
			//vectorの中から捨てたアイテムのデータを消す
			m_itemList.erase(m_itemList.begin() + m_takeItem % MaxHaveItem);
		}
	}
}

void Inventory::Draw()
{
	m_inventoryUi.Draw(m_transform);
	m_takeItemUi.Draw(m_takeItemTransform);
}