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
		"game_title.png",
		Screen::TopCenter + Vector2(0, Screen::Height / 3)
	));

	// Press Any Button
	m_rootNode->AddChild(new PressAny());

	m_chest = new Chest();
	m_wallet = new Wallet();
	m_moneyCount = new MoneyCount(m_wallet, 3, 0);

	// BGM
	m_bgm = LoadSoundMem("Resource/sound/title_bgm.mp3");
	ChangeVolumeSoundMem(70, m_bgm);
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);
}

// I—¹
void SceneTitle::Finalize()
{
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;

	// BGM
	DeleteSoundMem(m_bgm);
}

// XV
SceneBase* SceneTitle::Update()
{
	// ‚¢‚¸‚ê‚©‚ÌƒL[‚ª‰Ÿ‚³‚ê‚½‚çƒQ[ƒ€ƒV[ƒ“‚Ö‘JˆÚ
	if (Input::GetInstance()->IsAnyKeyDown())
	{
		return new SceneGame(m_chest->GetItemList(), FirstMaxHaveItem,0,m_moneyCount);
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