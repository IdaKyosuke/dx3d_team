#pragma once
#include "SceneBase.h"
#include "Item.h"
#include <vector>

class Node;
class LoadPlayer;
class Camera;
class Collision3D;
class ItemFactory;
class UiScore;
class UiResult;
class CollisionStage;
class Inventory;
class Chest;

class KeepChest;
class Item;

// �Q�[���V�[��
class SceneGame : public SceneBase
{
private:


	static constexpr float ResultTransitionTime = 2.0f;// �v���C���[������ł��烊�U���g��ʂɑJ�ڂ���܂ł̎���

	Node* m_rootNode;

	LoadPlayer* m_loadPlayer;
	Camera* m_cam;
	Collision3D* m_collision3D;
	ItemFactory* m_itemfactory;			// �A�C�e���̃|�C���^
	UiScore* m_uiScore;
	UiResult* m_uiResult;
	CollisionStage* m_collisionStage;
	Inventory* m_inventory;
	Chest* m_chest;

	KeepChest* m_keepChest;
	Item* m_item;

	std::vector<Item> m_chestItem;

	float m_resultTransitionTime;
	bool m_isFinish;

public:
	// �R���X�g���N�^
	SceneGame(std::vector<Item> itemList) :
		m_rootNode(nullptr),
		m_loadPlayer(nullptr),
		m_cam(nullptr),
		m_resultTransitionTime(ResultTransitionTime),
		m_collision3D(nullptr),
		m_itemfactory(nullptr),
		m_uiScore(nullptr),
		m_uiResult(nullptr),
		m_item(nullptr),
		m_inventory(nullptr),
		m_collisionStage(nullptr),
		m_chest(nullptr),

		m_keepChest(nullptr),
		m_chestItem(itemList),

		m_isFinish(false){}

	virtual void Initialize() override;		// ������
	virtual void Finalize() override;		// �I��
	virtual SceneBase* Update() override;	// �X�V
	virtual void Draw() override;			// �`��
};
