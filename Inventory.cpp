#include "Inventory.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "ItemIcon.h"
#include "LoadPlayer.h"
#include "Input.h"

Inventory::Inventory(int maxHaveItem) :
	m_maxHaveItem(maxHaveItem),
	m_haveItemCount(0),
	m_canGetItem(false),
	m_itemNum(0),
	m_gettingItem(false),
	m_takeItem(0),
	m_dropItem(false),
	m_nowHaveWeight(0),
	m_dropItemNum(0),
	m_dropItemCompletion(false),
	m_seInventory(0)
{
	m_inventoryUi.Register("inventory_ui.png");
	m_takeItemUi.Register("take_item.png");
}

void Inventory::Load()
{
	//seを設定
	m_seInventory = LoadSoundMem("Resource/sound/move_takeUi.mp3");
	ChangeVolumeSoundMem(128, m_seInventory);

	m_inventoryUi.Load();
	m_takeItemUi.Load();
}

void Inventory::Release()
{
	DeleteSoundMem(m_seInventory);

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

	if (m_haveItemCount > 0)
	{
		//アイテム選択
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_Q))
		{
			m_takeItem--;

			PlaySoundMem(m_seInventory, DX_PLAYTYPE_BACK);
		}
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_E))
		{
			m_takeItem++;

			PlaySoundMem(m_seInventory, DX_PLAYTYPE_BACK);
		}

		//アイテムを捨てる
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_G))
		{
			m_dropItem = true;

			m_dropItemNum = m_takeItem;
		}
		if (m_dropItemCompletion)
		{
			m_itemList.erase(m_itemList.begin() + m_dropItemNum);
			m_dropItemCompletion = false;

			m_haveItemCount--;
		}
	}

	m_takeItem = m_takeItem % m_maxHaveItem;

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

		m_nowHaveWeight += Item(itemNum).GetItemWeight();
	}
}

// アイテムがインベントリに入るかを確認
void Inventory::CheckCanAddItem()
{
	// いったんコピー
	std::vector<Item*> addItemList = m_addItemList;
	// リストをリセット
	m_addItemList.clear();

	for (Item* item : addItemList)
	{
		// 限界までアイテムを所持している場合はループを抜ける
		if (m_haveItemCount >= m_maxHaveItem) break;

		// アイテムを追加
		Item i = *item;
		m_itemList.push_back(i);

		// カウントアップ
		AddItemCount();

		// アイコンを追加
		TakeItem(item->GetItemNum());

		// 追加したアイテムは削除
		item->DestroyMine();
	}
}