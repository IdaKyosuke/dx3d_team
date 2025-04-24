#include "Inventory.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "ItemIcon.h"
#include "LoadPlayer.h"
#include "Input.h"

Inventory::Inventory(int maxHaveItem) :
	m_maxHaveItem(maxHaveItem),
	m_maxHaveWeight(FirstMaxHaveWeight),
	m_haveItemCount(0),
	m_canGetItem(false),
	m_itemNum(0),
	m_gettingItem(false),
	m_takeItem(0),
	m_dropItem(false),
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

	if (m_dropItem)
	{
		m_dropItem = false;
	}
	if (m_gettingItem)
	{
		m_gettingItem = false;
	}

	//アイテムを拾うことができるか
	if (m_haveItemCount < m_maxHaveItem)
	{
		m_canGetItem = true;
	}
	else 
	{
		m_canGetItem = false;
	}


	//拾ったアイテムを認識する
	//認識してアイコンを生成
	
	//アイテム選択
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
		//アイテムを捨てる
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_G))
		{
			m_dropItem = true;
			m_haveItemCount--;

			m_dropItemNum = m_takeItem;			
		}
		if (m_dropItemHoge)
		{
			m_itemList.erase(m_itemList.begin() + m_takeItem);

			m_dropItemHoge = false;
		}
	}

	//持っていないアイテムとかを選択できないようにしたりなど
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

	//Uiのポジションの設定
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

void Inventory::TakeItem(int itemNum)
{
	if (m_haveItemCount <= m_maxHaveItem)
	{
		AddChild(new ItemIcon(itemNum, m_haveItemCount - 1, this));
	}
}

void Inventory::ItemListSet()
{
	if (m_advanceItemList.size() <= m_maxHaveItem)
	{
		m_itemList.push_back(std::next(m_advanceItemList.begin(), m_advanceItemList.size() - 1)->GetItem());

		m_advanceItemList.erase(std::next(m_advanceItemList.begin(), m_advanceItemList.size() - 1));
	}
}