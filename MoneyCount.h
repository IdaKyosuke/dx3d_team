#pragma once
#include "Node.h"
#include "Vector2.h"
#include "Transform.h"
#include "Sprite.h"

class Wallet;

class MoneyCount : public Node
{
private:
	static constexpr int RestDays = 3;	//残りの日付の初期値
	static constexpr int NeedMoney = 200;	//最初に必要なお金
	static constexpr Vector2 UiPos = Vector2(125, 250);
	static constexpr Vector2 FontSize = Vector2(20, 35);	// 数字1文字の幅・高さ
	static constexpr int FontMargin = 5;					// 数字と数字の間の余白
	static constexpr int AddNeedMoney = 100;

	int m_needMoney;
	int m_restDays;

	bool m_taskClear;	//条件合格したか
	int m_clearCount;	//何回クリアしたか

	bool m_backMenu;	//メニュー画面に来たか

	Wallet* m_wallet;

	int m_fontTextureId;	// 数字フォント
	Sprite m_moneyCountUi;
	Transform m_transform;

	bool m_isClear;

protected:
	virtual void Load();
	virtual void Release();
	virtual void Update();
	virtual void Draw() override;

public:
	MoneyCount(Wallet* wallet, int restDays, int clearCount);

	int GetRestDays()
	{
		return m_restDays;
	}

	int GetClearCount()
	{
		return m_clearCount;
	}

	int GetNeedMoney()
	{
		return m_needMoney;
	}

	bool GetTaskClear() const
	{
		return m_taskClear;
	}

	void BackMenu()
	{
		m_backMenu = true;
	}

	void AddClearCount()
	{
		m_clearCount++;
	}
};