#pragma once
#include "SceneBase.h"

class Node;
class MenuInventory;
class Chest;

class Inventory;

class SceneMenu : public SceneBase
{
private:
	Node* m_rootNode;

	MenuInventory* m_menuInventory;
	Chest* m_chest;

	Inventory* m_inventory;

public:
	SceneMenu(Inventory* inventoy = nullptr) :
		m_rootNode(nullptr),
		m_menuInventory(nullptr),
		m_chest(nullptr),
	
		m_inventory(inventoy){};

	virtual void Initialize() override;		// ‰Šú‰»
	virtual void Finalize() override;		// I—¹
	virtual SceneBase* Update() override;	// XV
	virtual void Draw() override;			// •`‰æ
};