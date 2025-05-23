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

class Item;

class EnhanceType;

class EnhanceInventory;
class EnhanceHaveWeight;
class EnhanceTheWorldTime;
class EnhanceUseTheWorld;
class EnhanceStaminaRecovery;
class EnhanceStaminaDecrease;

class SceneMenu : public SceneBase
{
private:
	static constexpr int NeedClearCount = 7;
	Node* m_rootNode;

	Inventory* m_inventory;
	MenuInventory* m_menuInventory;
	Chest* m_chest;
	Wallet* m_wallet;

	Shop* m_shop;
	SellButton* m_sellButton;
	MoneyCount* m_moneyCount;

	Item* m_item;

	EnhanceType* m_enhanceType;

	EnhanceInventory* m_enhanceInventory;
	EnhanceHaveWeight* m_enhanceHaveWeight;
	EnhanceTheWorldTime* m_enhanceTheWorldTime;
	EnhanceStaminaRecovery* m_enhanceStaminaRecovery;
	EnhanceStaminaDecrease* m_enhanceStaminaDecrease;
	EnhanceUseTheWorld* m_enhanceUseTheWorld;

	std::vector<Item> m_chestItem;

	int m_maxHaveInventory;
	int m_maxHaveWeight;
	float m_theWorldTime;
	int m_useCountTheWorld;
	float m_staminaRecovery;
	float m_staminaDecrease;

	int m_haveMoney;

	int m_restDays;
	int m_clearCount;

	int m_clearMovieHandle;

	bool m_clearFlag;

public:
	SceneMenu(std::vector<Item> itemList, Inventory* inventoy, EnhanceType* enhanceType, int haveMoney, MoneyCount* moneyCount) :
		m_rootNode(nullptr),
		m_menuInventory(nullptr),
		m_chest(nullptr),
		m_shop(nullptr),
		m_inventory(inventoy),
		m_wallet(nullptr),
		m_sellButton(nullptr),
		m_chestItem(itemList),
		m_enhanceType(enhanceType),
		m_maxHaveInventory(0),
		m_maxHaveWeight(0),
		m_theWorldTime(0),
		m_useCountTheWorld(0),
		m_staminaRecovery(0),
		m_staminaDecrease(0),
		m_haveMoney(haveMoney),
		m_moneyCount(moneyCount),
		m_restDays(0),
		m_clearCount(0),
		m_clearMovieHandle(0),
		m_clearFlag(false),
		m_enhanceInventory(nullptr),
		m_enhanceHaveWeight(nullptr),
		m_enhanceTheWorldTime(nullptr),
		m_enhanceUseTheWorld(nullptr),
		m_enhanceStaminaRecovery(nullptr),
		m_enhanceStaminaDecrease(nullptr),
		m_item(nullptr){};
	

	virtual void Initialize() override;		// èâä˙âª
	virtual void Finalize() override;		// èIóπ
	virtual SceneBase* Update() override;	// çXêV
	virtual void Draw() override;			// ï`âÊ
};