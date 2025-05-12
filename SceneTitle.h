#pragma once
#include "SceneBase.h"

class Node;
class Chest;
class MoneyCount;
class Wallet;
class EnhanceType;

// タイトルシーン
class SceneTitle : public SceneBase
{
private:
	Node* m_rootNode;
	int m_bgm;
	int m_seClick;

	Chest* m_chest;
	MoneyCount* m_moneyCount;
	Wallet* m_wallet;
	EnhanceType* m_enhanceType;

	static constexpr int FirstMaxHaveItem = 3;		//最初にアイテムを持てる数の最大量
	static constexpr int FirstMaxHaveWeight = 50;	//最初に持てる最大容量
	static constexpr float FirstTheWorldTime = 5;	//最初のザワールドの継続時間
	static constexpr int FirstTheWorldCount = 1;	//最初にザワールドできる回数
	static constexpr float StaminaRecoveryAmount = 10;	// スタミナの回復量
	static constexpr float StaminaDecreaseAmount = 20;	// スタミナの消費量


public:
	// コンストラクタ
	SceneTitle() :
		m_rootNode(nullptr),
		m_bgm(0),
		m_seClick(0),
		m_chest(nullptr),
		m_wallet(nullptr),
		m_moneyCount(nullptr),
		m_enhanceType(nullptr){}

	virtual void Initialize() override;		// 初期化
	virtual void Finalize() override;		// 終了
	virtual SceneBase* Update() override;	// 更新
	virtual void Draw() override;			// 描画
};
