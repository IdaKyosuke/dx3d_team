#include "EnhanceInventory.h"
#include "Chest.h"
#include "Wallet.h"

EnhanceInventory::EnhanceInventory(Chest* chest, Wallet* wallet, EnhanceType* enhanceType) :
	Actor("Enhance", "ehance_inventory.png", Position),
	m_button(Size, MOUSE_INPUT_LEFT, std::bind(&EnhanceInventory::OnClick, this)),
	m_chest(chest),
	m_canEnhance(false),
	m_needItemNum(NeedItemNumber),
	m_useItemNum(0),
	m_enhanceCount(0),
	m_needMoney(FirstNeedMoney),
	m_enhanceType(enhanceType),
	m_wallet(wallet),
	m_enhanceTypeChoice(EnhanceType::EnhanceTypeChoice::EnhanceInventory)
{
}

//更新
void EnhanceInventory::Update()
{
	//本来の更新処理
	Actor::Update();

	//強化していくごとに必要素材を増やす
	if (m_enhanceType->GetMaxHaveInventory() >= 5)
	{
		m_needMoney = NeedMoney[0];
	}
	if (m_enhanceType->GetMaxHaveInventory() >= 7)
	{
		m_needMoney = NeedMoney[1];
	}
	if (m_enhanceType->GetMaxHaveInventory() >= 8)
	{
		m_needMoney = NeedMoney[2];
	}

	//ボタン
	m_button.Update(m_transform.position);
}

//描画　
void EnhanceInventory::Draw()
{
	//条件を満たしてない場合はボタンを暗化させる
	if (!CheckCondition())
	{
		//以降、輝度を下げて描画する
		SetDrawBright(50, 50, 50);
	}

	//本来の描画処理
	Actor::Draw();

	//輝度の設定を元に戻す
	SetDrawBright(255, 255, 255);

#ifdef _DEBUG
	m_button.Draw(m_transform.position);
#endif
}

//ボタンが押された時に呼ばれるコールバック関数
void EnhanceInventory::OnClick()
{
	if (!CheckCondition()) return;

	//強化に必要なアイテムを消費
	m_chest->LostItem(m_useItemNum);
	m_wallet->LostMoney(m_needMoney);

	//強化
	m_enhanceType->Enhance(m_enhanceTypeChoice);
}

//ボタンが有効かどうかチェック
bool EnhanceInventory::CheckCondition()
{
	//決めた値まで強化したら終了
	if (m_enhanceType->GetMaxTheWorldTime() < 10)
	{
		//持ち物あるかお金あるかの判定
		if (!m_chest->GetItemList().empty())
		{
			for (int i = 0; i <= m_chest->GetItemList().size() - 1; i++)
			{
				if (m_needItemNum == std::next(m_chest->GetItemList().begin(), i)->GetItemNum() &&
					m_needMoney <= m_wallet->HaveMoney())
				{
					m_canEnhance = true;
					m_useItemNum = i;
				}
				if (m_enhanceType->OnInventoryButton())
				{
					m_enhanceType->InventoryButtonReset();
					m_canEnhance = false;
				}
			}
		}
		else
		{
			m_canEnhance = false;
		}
	}

	return m_canEnhance;
}