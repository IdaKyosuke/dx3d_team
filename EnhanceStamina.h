#pragma once
#include "Button.h"
#include "Actor.h"
#include "SceneBase.h"
#include "Vector2.h"
#include "EnhanceType.h"
#include <vector>

class Chest;
class EnhanceType;
class Wallet;

class EnhanceStamina : public Actor
{
private:
	static constexpr Vector2 Position = Vector2(500, 400);
	static constexpr Vector2 Size = Vector2(250, 100);
	static constexpr int FirstNeedMoney = 100;				//必要なお金の初期金
	static constexpr int NeedMoney[3] = { 150,200,300 };	//強化していくごとに上がっていく必要なお金
	static constexpr int EndEnhanceCount = 10;		//ここまでの数値強化したら終わり

	int m_needMoney;		//必要なお金

	int m_enhanceCount;		//何回強化したか
	bool m_canEnhance;		//強化できるか

	int m_needItemNum;		//強化に必要なアイテムのナンバー

	Chest* m_chest;

	int m_useItemNum;


protected:
	EnhanceType::EnhanceTypeChoice m_enhanceTypeChoice;	//押された時に強化するもの
	Button m_button;	//ボタン機能

	EnhanceType* m_enhanceType;
	Wallet* m_wallet;

	//ボタンが押された時に呼ばれるコールバック関数
	void OnClick();

	//ボタンが有効かどうかをチェック
	virtual bool CheckCondition();

	virtual void Update() override;	//更新
	virtual void Draw() override;	//描画

public:
	EnhanceStamina(Chest* chest, Wallet* wallet, EnhanceType* enhanceType);
};
