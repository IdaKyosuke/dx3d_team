#pragma once
#include "SceneBase.h"
#include <list>

class Node;
class LoadPlayer;
class Enemy;
class Collision3D;
class NavMesh;
class Camera;
class ItemFactory;
class UiScore;
class UiResult;
class CollisionStage;

class Item;
class Inventory;


// �Q�[���V�[��
class SceneGame : public SceneBase
{
private:
	const std::list<const char*> ImagePreload =
	{

	};

	static constexpr float ResultTransitionTime = 2.0f;// �v���C���[������ł��烊�U���g��ʂɑJ�ڂ���܂ł̎���

	Node* m_rootNode;
	LoadPlayer* m_loadPlayer;
	Enemy* m_enemy;
	Collision3D* m_collision3D;
	NavMesh* m_navMesh;
	Camera* m_cam;
	CollisionStage* m_collisionStage;
	ItemFactory* m_itemfactory;			// �A�C�e���̃|�C���^
	UiScore* m_uiScore;
	UiResult* m_uiResult;

	Item* m_item;
	Inventory* m_inventory;


	float m_resultTransitionTime;
	bool m_isFinish;



	int m_model;

public:
	// �R���X�g���N�^
	SceneGame() :
		m_rootNode(nullptr),
		m_loadPlayer(nullptr),
		m_enemy(nullptr),
		m_navMesh(nullptr),
		m_cam(nullptr),
		m_resultTransitionTime(ResultTransitionTime),
		m_collisionStage(nullptr),
		m_itemfactory(nullptr),
		m_uiScore(nullptr),
		m_uiResult(nullptr),
		m_item(nullptr),
		m_inventory(nullptr),
		m_isFinish(false),
		m_model(0){}

	virtual void Initialize() override;		// ������
	virtual void Finalize() override;		// �I��
	virtual SceneBase* Update() override;	// �X�V
	virtual void Draw() override;			// �`��
};
