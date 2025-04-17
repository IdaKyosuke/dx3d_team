#include "SceneTitle.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "Actor.h"
#include "PressAny.h"

#include "Chest.h"

// 初期化
void SceneTitle::Initialize()
{
	m_rootNode = new Node();

	// タイトルロゴ
	m_rootNode->AddChild(new Actor(
		"Logo",
		"game_title.png",
		Screen::TopCenter + Vector2(0, Screen::Height / 3)
	));

	// Press Any Button
	m_rootNode->AddChild(new PressAny());

	m_chest = new Chest();

	// BGM
	m_bgm = LoadSoundMem("Resource/sound/title_bgm.mp3");
	ChangeVolumeSoundMem(70, m_bgm);
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);
}

// 終了
void SceneTitle::Finalize()
{
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;

	// BGM
	DeleteSoundMem(m_bgm);
}

// 更新
SceneBase* SceneTitle::Update()
{
	// いずれかのキーが押されたらゲームシーンへ遷移
	if (Input::GetInstance()->IsAnyKeyDown())
	{
		return new SceneGame(m_chest->GetItemList(), 3);
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