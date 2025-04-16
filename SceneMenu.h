#pragma once
#include "SceneBase.h"
#include "Item.h"
#include <vector>

class Node;
class MenuInventory;
class Chest;
class Shop;
class Inventory;
class ShopButton;
class Wallet;
class SellButton;
class EnhanceShop;
class EnhanceShopButton;

class SceneMenu : public SceneBase
{
private:
	Node* m_rootNode;

	Inventory* m_inventory;
	MenuInventory* m_menuInventory;
	Chest* m_chest;
	Wallet* m_wallet;

	Shop* m_shop;
	ShopButton* m_shopButton;
	SellButton* m_sellButton;
	EnhanceShop* m_enhanceShop;
	EnhanceShopButton* m_enhanceShopButton;

	std::vector<Item> m_chestItem;

public:
	SceneMenu(std::vector<Item> itemList, Inventory* inventoy = nullptr) :
		m_rootNode(nullptr),
		m_menuInventory(nullptr),
		m_chest(nullptr),
		m_shop(nullptr),
		m_shopButton(nullptr),
		m_inventory(inventoy),
		m_wallet(nullptr),
		m_sellButton(nullptr),
		m_chestItem(itemList),
		m_enhanceShop(nullptr),
		m_enhanceShopButton(nullptr){};

	virtual void Initialize() override;		// èâä˙âª
	virtual void Finalize() override;		// èIóπ
	virtual SceneBase* Update() override;	// çXêV
	virtual void Draw() override;			// ï`âÊ
};