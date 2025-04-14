#pragma once
#include "SceneBase.h"

class Node;
class MenuInventory;
class Chest;

class LoadPlayer;

class CollisionStage;
class Inventory;

class SceneMenu : public SceneBase
{
private:
	Node* m_rootNode;

	MenuInventory* m_menuInventory;
	Chest* m_chest;

	Inventory* m_inventory;
	LoadPlayer* m_loadPlayer;

	CollisionStage* m_collisionStage;

public:
	SceneMenu(Inventory* inventoy = nullptr) :
		m_rootNode(nullptr),
		m_menuInventory(nullptr),
		m_chest(nullptr),
	
		m_inventory(inventoy),
		m_collisionStage(nullptr),
		m_loadPlayer(nullptr){};

	virtual void Initialize() override;		// ������
	virtual void Finalize() override;		// �I��
	virtual SceneBase* Update() override;	// �X�V
	virtual void Draw() override;			// �`��
};