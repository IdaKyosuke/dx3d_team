#pragma once
#include "SceneBase.h"
#include "Item.h"
#include <vector>

class Node;
class MenuInventory;
class Chest;
class Shop;
class Inventory;
class Wallet;
class SellButton;

class EnhanceType;

class EnhanceInventory;

class SceneMenu : public SceneBase
{
private:
	Node* m_rootNode;

	Inventory* m_inventory;
	MenuInventory* m_menuInventory;
	Chest* m_chest;
	Wallet* m_wallet;

	Shop* m_shop;
	SellButton* m_sellButton;

	EnhanceType* m_enhanceType;

	EnhanceInventory* m_enhanceInventory;

	std::vector<Item> m_chestItem;

	int m_maxHaveItem;
	int m_haveMoney;

public:
	SceneMenu(std::vector<Item> itemList, Inventory* inventoy, int maxHaveItem, int haveMoney) :
		m_rootNode(nullptr),
		m_menuInventory(nullptr),
		m_chest(nullptr),
		m_shop(nullptr),
		m_inventory(inventoy),
		m_wallet(nullptr),
		m_sellButton(nullptr),
		m_chestItem(itemList),
		m_enhanceInventory(nullptr),
		m_enhanceType(nullptr),
		m_maxHaveItem(maxHaveItem),
		m_haveMoney(haveMoney) {};
	

	virtual void Initialize() override;		// èâä˙âª
	virtual void Finalize() override;		// èIóπ
	virtual SceneBase* Update() override;	// çXêV
	virtual void Draw() override;			// ï`âÊ
};