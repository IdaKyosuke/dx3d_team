#include "SceneTitle.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "Actor.h"
#include "PressAny.h"

#include "Chest.h"
#include "Wallet.h"
#include "MoneyCount.h"
#include "EnhanceType.h"

// 初期化
void SceneTitle::Initialize()
{
	m_rootNode = new Node();

	// 背景
	m_rootNode->AddChild(new Actor(
		"Background",
		"title_background.png",
		Screen::Center
	));

	// タイトルロゴ
	m_rootNode->AddChild(new Actor(
		"Logo",
		"title_logo.png",
		Screen::TopCenter + Vector2(0, Screen::Height / 3)
	));

	// PressAnyButton
	m_rootNode->AddChild(new Actor(
		"PressAny",
		"press_any_button.png",
		Screen::BottomCenter - Vector2(0, Screen::Height / 4)
	));

	m_chest = new Chest();
	m_wallet = new Wallet();
	m_moneyCount = new MoneyCount(m_wallet, 3, 0);
	m_enhanceType = new EnhanceType(FirstMaxHaveItem, FirstMaxHaveWeight, FirstTheWorldTime, FirstTheWorldCount, StaminaRecoveryAmount, StaminaDecreaseAmount);

	// BGM
	m_bgm = LoadSoundMem("Resource/sound/title_bgm.mp3");
	ChangeVolumeSoundMem(100, m_bgm);
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);

	m_seClick = LoadSoundMem("Resource/sound/title_click.mp3");
	ChangeVolumeSoundMem(130, m_seClick);
}

// 終了
void SceneTitle::Finalize()
{
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;

	// BGM
	DeleteSoundMem(m_bgm);
	DeleteSoundMem(m_seClick);
}

// 更新
SceneBase* SceneTitle::Update()
{
	int soundTime = 0;

	// いずれかのキーが押されたらゲームシーンへ遷移
	if (Input::GetInstance()->IsAnyKeyDown())
	{
		PlaySoundMem(m_seClick, DX_PLAYTYPE_BACK);	
	}
	soundTime = static_cast<int>(GetSoundCurrentTime(m_seClick));

	if (soundTime >= 2)
	{
		return new SceneGame(m_chest->GetItemList(), m_enhanceType, 0, m_moneyCount);
	}

	// ノードの更新
	m_rootNode->TreeUpdate();

	return this;
}

// 描画
void SceneTitle::Draw()
{
	// ノードの描画
	m_rootNode->TreeDraw();
}