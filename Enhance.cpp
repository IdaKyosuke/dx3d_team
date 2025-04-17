#include "Enhance.h"
#include "UiScore.h"
#include "DxLib.h"
#include "Chest.h"

//コンストラクタ
Enhance::Enhance(
	EnhanceType::EnhanceTypeChoice enhanceTypeChoice,
	int useItem,
	int useMoney,
	const Vector2& position,
	const char* textureName,
	Chest* chest,
	EnhanceType* const enhanceType,
	Wallet* const wallet
) :
	Actor("Enhance", textureName, position),
	m_enhanceTypeChoice(enhanceTypeChoice),
	m_button(Size, MOUSE_INPUT_LEFT, std::bind(&Enhance::OnClick, this)),
	m_chest(chest),
	m_useItemNum(useItem),
	m_enhanceType(enhanceType),
	m_useMoney(useMoney),
	m_wallet(wallet)
{
	
}

//ボタンが押された時に呼ばれるコールバック関数
void Enhance::OnClick()
{
	if (!CheckCondition()) return;

	m_useMoney = -m_useMoney;

	//強化に必要なアイテムを消費
	m_chest->LostItem(m_useItemNum);
	m_wallet->InWalletMoney(m_useMoney);

	//強化
	m_enhanceType->Enhance(m_enhanceTypeChoice);
}

//更新
void Enhance::Update()
{
	//本来の更新処理
	Actor::Update();

	//ボタン
	m_button.Update(m_transform.position);
}

//描画　
void Enhance::Draw()
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