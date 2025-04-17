#pragma once
#include "Actor.h"
#include "Button.h"
#include "Vector2.h"
#include "EnhanceType.h"
#include  "Wallet.h"

#include <vector>

class Chest;

class Enhance : public Actor
{
private:
	static constexpr Vector2 Size = Vector2(200, 100);

	int m_useItemNum;
	int m_useMoney;

	Chest* m_chest;

protected:
	EnhanceType::EnhanceTypeChoice m_enhanceTypeChoice;	//押された時に強化するもの
	Button m_button;	//ボタン機能

	EnhanceType* m_enhanceType;
	Wallet* m_wallet;

	//ボタンが押された時に呼ばれるコールバック関数
	void OnClick();

	//ボタンが有効かどうかをチェック
	virtual bool CheckCondition() = 0;

	virtual void Update() override;	//更新
	virtual void Draw() override;	//描画

public:
	//コンストラクタ
	Enhance(
		EnhanceType::EnhanceTypeChoice enhanceTypeChoice,
		int useItem,
		int useMoney,
		const Vector2& position,
		const char* textureName,
		Chest* chest,
		EnhanceType* const enhanceType,
		Wallet* const wallet
	);
};