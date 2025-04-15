#include "SceneGame.h"
#include"SceneTitle.h"
#include "Screen.h"
#include "Input.h"
#include "Node.h"
#include "Actor.h"
#include "Time.h"
#include "Fade.h"
#include "ImageLoader.h"
#include"LoadPlayer.h"
<<<<<<< HEAD
#include "Enemy.h"
#include"Collision3D.h"
=======
#include"NavMesh.h"
#include"Enemy.h"
#include"CollisionStage.h"
>>>>>>> navMesh
#include"ItemFactory.h"
#include"UiScore.h"
#include"UiResult.h"
#include "DxLib.h"

#include "Item.h"
#include "Inventory.h"
#include"CollisionStage.h"


// ������
void SceneGame::Initialize()
{
	// �摜�̎��O�ǂݍ���
	for (auto image : ImagePreload)
	{
		ImageLoader::GetInstance()->Load(image);
	}

	// ���[�g
	m_rootNode = new Node();

	// �A�N�^�[���C���[
	Node* actorLayer = new Node();
	m_rootNode->AddChild(actorLayer);

	// UI���C���[
	Node* uiLayer = new Node();
	m_rootNode->AddChild(uiLayer);

	// �X�e�[�W�̓����蔻����쐬
<<<<<<< HEAD
	m_collisionStage = new CollisionStage("Resource/stage.mv1", Vector3(0, 0, 0));
=======
	m_collisionStage = new CollisionStage("Resource/PathPlanning.mqo", Vector3(0, 0, 0));
>>>>>>> navMesh
	uiLayer->AddChild(m_collisionStage);

	// �v���C���[
	m_loadPlayer = new LoadPlayer(m_collisionStage);
<<<<<<< HEAD
	actorLayer->AddChild(m_loadPlayer);

	// �G
	m_enemy = new Enemy(m_collision3D);
	actorLayer->AddChild(m_enemy);

	//�C���x���g��
	m_inventory = new Inventory(m_loadPlayer);
	uiLayer->AddChild(m_inventory);

	//�A�C�e��
	m_item = new Item(0, Vector3(100, 50, 100), m_loadPlayer, m_inventory);
	actorLayer->AddChild(m_item);

	//�A�C�e��
	m_item = new Item(1, Vector3(400, 50, 100), m_loadPlayer, m_inventory);
	actorLayer->AddChild(m_item);

	//�A�C�e��
	m_item = new Item(2, Vector3(100, 50, 400), m_loadPlayer, m_inventory);
	actorLayer->AddChild(m_item);


=======
	actorLayer->AddChild(m_loadPlayer);	

	// �i�r���b�V��
	m_navMesh = new NavMesh(m_collisionStage);
	
	// �G
	m_enemy = new Enemy(m_navMesh, Vector3(800, 110, 10), m_loadPlayer);
	actorLayer->AddChild(m_enemy);
	
>>>>>>> navMesh
	// �X�R�A
	m_uiScore = new UiScore();
	uiLayer->AddChild(m_uiScore);
	
	// �A�C�e���̐���
	m_itemfactory = new ItemFactory(m_uiScore);
	actorLayer->AddChild(m_itemfactory);

	// ���U���g���
	m_uiResult = new UiResult(m_itemfactory);
	uiLayer->AddChild(m_uiResult);
<<<<<<< HEAD

	
	
=======
	
	// BGM
	m_bgm = LoadSoundMem("Resource/sound/game_bgm.mp3");
	ChangeVolumeSoundMem(60, m_bgm);
	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);
>>>>>>> navMesh
}

// �I��
void SceneGame::Finalize()
{
	// ���O�ǂݍ��݂����摜�̔j��
	for (auto image : ImagePreload)
	{
		ImageLoader::GetInstance()->Delete(image);
	}

	// �v���C���[���̍폜
	m_loadPlayer->Finalize();

	// �m�[�h�̍폜
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;

	// navMesh���̔j��
	m_navMesh->RemovePathPlan();
	m_navMesh->RemovePolyLinkInfo();

	// BGM
	DeleteSoundMem(m_bgm);
}

// �X�V
SceneBase* SceneGame::Update()
{
	// �m�[�h�̍X�V
	m_rootNode->TreeUpdate();
	
	if (m_uiResult->IsShowedPress())
	{
		if (Input::GetInstance()->IsAnyKeyDown())
		{
			return new SceneTitle();
		}
	}

	return this;
}

// �`��
void SceneGame::Draw()
{
	// �m�[�h�̕`��
	m_rootNode->TreeDraw();
}
