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

// ‰Šú‰»
void SceneTitle::Initialize()
{
	m_rootNode = new Node();

	// ”wŒi
	m_rootNode->AddChild(new Actor(
		"Background",
		"title_background.png",
		Screen::Center
	));

	// ƒ^ƒCƒgƒ‹ƒƒS
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

// I—¹
void SceneTitle::Finalize()
{
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;

	// BGM
	DeleteSoundMem(m_bgm);
	DeleteSoundMem(m_seClick);
}

// XV
SceneBase* SceneTitle::Update()
{
	int soundTime = 0;

	// ‚¢‚¸‚ê‚©‚ÌƒL[‚ª‰Ÿ‚³‚ê‚½‚çƒQ[ƒ€ƒV[ƒ“‚Ö‘JˆÚ
	if (Input::GetInstance()->IsAnyKeyDown())
	{
		PlaySoundMem(m_seClick, DX_PLAYTYPE_BACK);	
	}
	soundTime = static_cast<int>(GetSoundCurrentTime(m_seClick));

	if (soundTime >= 2)
	{
		return new SceneGame(m_chest->GetItemList(), m_enhanceType, 0, m_moneyCount);
	}

	// ƒm[ƒh‚ÌXV
	m_rootNode->TreeUpdate();

	return this;
}

// •`‰æ
void SceneTitle::Draw()
{
	// ƒm[ƒh‚Ì•`‰æ
	m_rootNode->TreeDraw();
}