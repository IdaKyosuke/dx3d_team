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
class MoneyCount;

class EnhanceType;

class EnhanceInventory;
class EnhanceTheWorldTime;
class EnhanceStamina;

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
	MoneyCount* m_moneyCount;

	EnhanceType* m_enhanceType;

	EnhanceInventory* m_enhanceInventory;
	EnhanceTheWorldTime* m_enhanceWorldTime;
	EnhanceStamina* m_enhanceStamina;

	std::vector<Item> m_chestItem;

	int m_maxHaveItem;
	int m_maxTheWorldTime;
	int m_haveMoney;

	int m_restDays;
	int m_clearCount;

public:
	SceneMenu(std::vector<Item> itemList, Inventory* inventoy, EnhanceType* enhanceType, int haveMoney,MoneyCount* moneyCount) :
		m_rootNode(nullptr),
		m_menuInventory(nullptr),
		m_chest(nullptr),
		m_shop(nullptr),
		m_inventory(inventoy),
		m_wallet(nullptr),
		m_sellButton(nullptr),
		m_chestItem(itemList),
		m_enhanceInventory(nullptr),
		m_enhanceType(enhanceType),
		m_maxHaveItem(0),
		m_maxTheWorldTime(0),
		m_haveMoney(haveMoney),
		m_moneyCount(moneyCount),
		m_restDays(0),
		m_clearCount(0),
		m_enhanceWorldTime(nullptr),
		m_enhanceStamina(nullptr){};
	

	virtual void Initialize() override;		// èâä˙âª
	virtual void Finalize() override;		// èIóπ
	virtual SceneBase* Update() override;	// çXêV
	virtual void Draw() override;			// ï`âÊ
};