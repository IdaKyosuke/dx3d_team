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
	m_takeItem(0),
	m_destroyItemIcon(false),
	m_canHaveWeight(0)
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
	auto countGetItem = std::distance(m_itemList.begin(), m_itemList.end());
	if (m_gettingItem)
	{
		decltype(m_itemList)::iterator getItem = std::next(m_itemList.begin(), countGetItem - 1);
		m_itemNum = getItem->GetItemNum();

		GetParent()->AddChild(new ItemIcon(m_itemNum, static_cast<int>(countGetItem - 1),this));

		m_gettingItem = false;

		m_canHaveWeight += getItem->GetItemWeight();
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

	m_takeItem = m_takeItem % MaxHaveItem;

	if (m_haveItemCount > 0)
	{
		//アイテムを捨てる
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_R))
		{
			m_haveItemCount--;

			Vector3 dropPos = m_player->GetPosition();

			//捨てたオブジェクトを生成
			decltype(m_itemList)::iterator takeItem = std::next(m_itemList.begin(), m_takeItem);
			GetParent()->AddChild(new Item(takeItem->GetItemNum(), dropPos, this));

			//捨てたアイテムが何番目のアイテムか
			m_destroyTakeItem = m_takeItem;

			m_destroyItemIcon = true;

			//vectorの中から捨てたアイテムのデータを消す
			m_itemList.erase(m_itemList.begin() + m_takeItem);
		}
	}

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
	m_takeItemTransform.position = TakeItemUiPos;

	m_takeItemTransform.position = TakeItemUiPos + Vector2(90 * m_takeItem, 0);
}

void Inventory::Draw()
{
	m_inventoryUi.Draw(m_transform);
	m_takeItemUi.Draw(m_takeItemTransform);
}