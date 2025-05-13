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

class EnhanceHaveWeight : public Actor
{
private:
	static constexpr Vector2 Position = Vector2(750, 100);
	static constexpr Vector2 Size = Vector2(320, 100);
	static constexpr int FirstNeedMoney = 100;				//必要なお金の初期金
	static constexpr int NeedMoney[4] = { 100, 200, 300, 400 };	//強化していくごとに上がっていく必要なお金
	static constexpr int EndEnhanceCount = 15;		//ここまでの数値強化したら終わり
	static constexpr int NeedItemNum = 6;		//必要なアイテムの番号

	int m_needMoney;		//必要なお金
	int m_enhanceStep;		//どれだけ強化したか

	bool m_canEnhance;		//強化できるか

	int m_needItemNum;		//強化に必要なアイテムのナンバー

	Chest* m_chest;

	int m_useItemNum;		//使ったアイテムを何番目に持っていたか


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
	EnhanceHaveWeight(Chest* chest, Wallet* wallet, EnhanceType* enhanceType);
};
