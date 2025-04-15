#pragma once
#include "SceneBase.h"

class Node;
class MenuInventory;
class Chest;
class Shop;
class Inventory;
class ShopButton;

class SceneMenu : public SceneBase
{
private:
	Node* m_rootNode;

	MenuInventory* m_menuInventory;
	Chest* m_chest;

	Shop* m_shop;
	Inventory* m_inventory;
	ShopButton* m_shopButton;

public:
	SceneMenu(Inventory* inventoy = nullptr) :
		m_rootNode(nullptr),
		m_menuInventory(nullptr),
		m_chest(nullptr),
		m_shop(nullptr),
		m_shopButton(nullptr),
		m_inventory(inventoy){};

	virtual void Initialize() override;		// èâä˙âª
	virtual void Finalize() override;		// èIóπ
	virtual SceneBase* Update() override;	// çXêV
	virtual void Draw() override;			// ï`âÊ
};