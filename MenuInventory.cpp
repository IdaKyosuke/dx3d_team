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
	m_enhanceType(enhanceType), 
	m_seInventory(0),
	m_fontTextureId(0),
	m_nowHaveWeight(0)
{
	m_transform.position = Screen::BottomCenter + Vector2(0, -70);
	m_menuInventoryUi.Register("inventory_ui1.png");
	m_takeItemUi.Register("take_item.png");	
	m_slashUi.Register("slash.png");

	m_slashTransform.position = SlashUiPos;
}

void MenuInventory::Load()
{
	// seを設定
	m_seInventory = LoadSoundMem("Resource/sound/move_takeUi.mp3");
	ChangeVolumeSoundMem(128, m_seInventory);

	m_fontTextureId = ImageLoader::GetInstance()->Load("score_font.png");

	m_menuInventoryUi.Load();
	m_takeItemUi.Load();
	m_slashUi.Load();
}

void MenuInventory::Release()
{
	DeleteSoundMem(m_seInventory);

	m_menuInventoryUi.Release();
	m_takeItemUi.Release();
	m_slashUi.Release();
}

void MenuInventory::Update()
{
	m_menuInventoryUi.Update();
	m_takeItemUi.Update();
	m_slashUi.Update();
		
	m_maxHaveItem = m_enhanceType->GetMaxHaveInventory();

	CheckCanAddItem();

	if (m_destroyItemIcon)
	{
		m_destroyItemIcon = false;
	}

	if (m_isIventory)
	{
		//アイテム選択
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_A))
		{
			m_takeItem--;
			PlaySoundMem(m_seInventory, DX_PLAYTYPE_BACK);
		}
		else if (Input::GetInstance()->IsKeyDown(KEY_INPUT_D))
		{
			m_takeItem++;
			PlaySoundMem(m_seInventory, DX_PLAYTYPE_BACK);
		}

		//インベントリからチェストへ変更
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_C)&& !m_chest->GetTakeChest())
		{
			m_isIventory = false;

			m_chest->SetIsInventory(true);
		}

		//アイテムをチェストに格納する
		if (m_haveItemCount > 0 && m_chest->GetCanStorageItem())
		{
			if (Input::GetInstance()->IsKeyDown(KEY_INPUT_R))
			{
				//格納したアイテムが何番目のアイテムか
				m_destroyTakeItem = m_takeItem;

				//m_chest->AddAdvanceItemList(std::next(m_itemList.begin(), m_takeItem)->GetItemNum());
				m_chest->AddAdvanceItemList(m_itemList[m_takeItem].GetItemNum(), m_itemList[m_takeItem].GetItemData());
				m_nowHaveWeight -= m_itemList[m_takeItem].GetItemWeight();

				m_destroyItemIcon = true;

				m_chest->StringingChest();

				m_haveItemCount--;

				//vectorの中から捨てたアイテムのデータを消す
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
	if (m_isIventory)
	{
		m_takeItemUi.Draw(m_takeItemTransform);

	}

	//現在の持っているアイテムの重さを描画
	Vector2 nowHaveWeightUiPos = Vector2(80, 849);
	nowHaveWeightUiPos.y += FontMargin;
	int nowHaveWeight = m_nowHaveWeight;
	int nowHaveWeightDigit = 1;
	do
	{
		int value = nowHaveWeight % 10;	// 1の位の値を取り出す

		DrawRectGraph(
			static_cast<int>(nowHaveWeightUiPos.x - FontSize.x * nowHaveWeightDigit), static_cast<int>(nowHaveWeightUiPos.y),
			static_cast<int>(FontSize.x) * value, 0,
			static_cast<int>(FontSize.x), static_cast<int>(FontSize.y),
			m_fontTextureId,
			true
		);

		nowHaveWeight /= 10;	// スコアの桁下げ
		nowHaveWeightDigit++;		// 次の桁へ
	} while (nowHaveWeight > 0);

	//現在の持てるアイテムの重さの最大容量を描画
	Vector2 maxHaveWeightUiPos = Vector2(180, 849);
	maxHaveWeightUiPos.y += FontMargin;
	int maxHaveWeight = m_enhanceType->GetMaxHaveWeight();
	int maxHaveWeightDigit = 1;
	do
	{
		int value = maxHaveWeight % 10;	// 1の位の値を取り出す

		DrawRectGraph(
			static_cast<int>(maxHaveWeightUiPos.x - FontSize.x * maxHaveWeightDigit), static_cast<int>(maxHaveWeightUiPos.y),
			static_cast<int>(FontSize.x) * value, 0,
			static_cast<int>(FontSize.x), static_cast<int>(FontSize.y),
			m_fontTextureId,
			true
		);

		maxHaveWeight /= 10;	// スコアの桁下げ
		maxHaveWeightDigit++;		// 次の桁へ
	} while (maxHaveWeight > 0);

	m_slashUi.Draw(m_slashTransform);
}

void MenuInventory::TakeItem(Item* item)
{
	if (m_haveItemCount <= m_maxHaveItem)
	{
		AddChild(new MenuItemIcon(m_haveItemCount - 1, item, this));

		m_nowHaveWeight += item->GetItemWeight();
	}
}

void MenuInventory::CheckCanAddItem()
{
	std::vector<Item*> addItemList = m_addItemList;
	m_addItemList.clear();
	for (Item* item : addItemList)
	{
		if (m_haveItemCount >= m_maxHaveItem) break;
		Item i = *item;

		m_itemList.push_back(i);

		AddItemCount();

		TakeItem(item);

		item->DestroyMine();
	}
}

