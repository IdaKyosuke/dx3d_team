#include "EnhanceStaminaRecovery.h"
#include "Chest.h"
#include "Wallet.h"

EnhanceStaminaRecovery::EnhanceStaminaRecovery(Chest* chest, Wallet* wallet, EnhanceType* enhanceType) :
	Actor("Enhance", "enhacne_stamina_recovery.png", Position),
	m_button(Size, MOUSE_INPUT_LEFT, std::bind(&EnhanceStaminaRecovery::OnClick, this)),
	m_chest(chest),
	m_canEnhance(false),
	m_needItemNum(NeedItemNum),
	m_useItemNum(0),
	m_enhanceStep(0),
	m_needMoney(FirstNeedMoney),
	m_enhanceType(enhanceType),
	m_wallet(wallet),
	m_enhanceTypeChoice(EnhanceType::EnhanceTypeChoice::EnhanceStaminaRecovery)
{
}

//更新
void EnhanceStaminaRecovery::Update()
{
	//本来の更新処理
	Actor::Update();

	//強化していくごとに必要素材を増やす
	if (m_enhanceType->GetStaminaRecovery() >= 12)
	{
		m_enhanceStep = 1;
	}
	if (m_enhanceType->GetStaminaRecovery() >= 16)
	{
		m_enhanceStep = 2;
	}
	if (m_enhanceType->GetStaminaRecovery() >= 18)
	{
		m_enhanceStep = 3;
	}

	m_needMoney = NeedMoney[m_enhanceStep];

	//ボタン
	m_button.Update(m_transform.position);
}

//描画　
void EnhanceStaminaRecovery::Draw()
{
	SetFontSize(30);
	DrawFormatString(510, 420,
		GetColor(255, 255, 255),
		"%d$",
		NeedMoney[m_enhanceStep]);

	SetFontSize(20);
	DrawFormatString(260, 455,
		GetColor(255, 255, 255),
		"スタミナ回復量→%.0f:強化後→%.0f",
		m_enhanceType->GetStaminaRecovery(),
		m_enhanceType->GetStaminaRecovery() + m_enhanceType->AddStaminaRecovery);

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
void EnhanceStaminaRecovery::OnClick()
{
	if (!CheckCondition()) return;

	//強化に必要なアイテムを消費
	m_chest->LostItem(m_useItemNum);
	m_wallet->LostMoney(m_needMoney);

	//強化
	m_enhanceType->Enhance(m_enhanceTypeChoice);
}

//ボタンが有効かどうかチェック
bool EnhanceStaminaRecovery::CheckCondition()
{
	//決めた値まで強化したら終了
	if (m_enhanceType->GetStaminaRecovery() <= EndEnhanceCount)
	{
		//持ち物あるかお金あるかの判定
		if (!m_chest->GetItemList().empty())
		{
			for (int i = 0; i <= m_chest->GetItemList().size() - 1; i++)
			{
				if (m_needItemNum != std::next(m_chest->GetItemList().begin(), i)->GetItemNum() ||
					m_needMoney >= m_wallet->HaveMoney())
				{
					m_canEnhance = false;
				}
			}

			for (int i = 0; i <= m_chest->GetItemList().size() - 1; i++)
			{
				if (m_needItemNum == std::next(m_chest->GetItemList().begin(), i)->GetItemNum() &&
					m_needMoney <= m_wallet->HaveMoney())
				{
					m_canEnhance = true;
					m_useItemNum = i;
				}
				if (m_enhanceType->OnStaminaRecoveryButton())
				{
					m_canEnhance = false;
					m_enhanceType->StaminaRecoveryButtonReset();
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