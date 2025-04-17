#include "SceneTitle.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "Actor.h"
#include "PressAny.h"

#include "Chest.h"

// ������
void SceneTitle::Initialize()
{
	m_rootNode = new Node();

	// �^�C�g�����S
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

// �I��
void SceneTitle::Finalize()
{
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;

	// BGM
	DeleteSoundMem(m_bgm);
}

// �X�V
SceneBase* SceneTitle::Update()
{
	// �����ꂩ�̃L�[�������ꂽ��Q�[���V�[���֑J��
	if (Input::GetInstance()->IsAnyKeyDown())
	{
		return new SceneGame(m_chest->GetItemList(), 3);
	}

	// �m�[�h�̍X�V
	m_rootNode->TreeUpdate();

	return this;
}

// �`��
void SceneTitle::Draw()
{
	// �m�[�h�̕`��
	m_rootNode->TreeDraw();
}