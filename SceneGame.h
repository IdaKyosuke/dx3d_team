#pragma once
#include "SceneBase.h"
#include "Item.h"
#include"Vector3.h"
#include <vector>

class Node;
class LoadPlayer;
class Camera;
class Collision3D;
class ItemFactory;
class EnemyFactory;
class UiResult;
class UiTime;
class UiStamina;
class CollisionStage;
class Inventory;
class Chest;
class Map;
class NavMesh;
class KeepChest;
class KeepMoneyCount;
class Item;
class DrawStageView;
class EscapePoint;
class MoneyCount;
class Wallet;
class ScreenFilter;
class EnhanceType;
class LightFactory;
class UiTheWorld;

// ゲームシーン
class SceneGame : public SceneBase
{
private:
	static constexpr int PointNum = 10;

	Node* m_rootNode;
	LoadPlayer* m_loadPlayer;
	Camera* m_cam;
	Collision3D* m_collision3D;
	ItemFactory* m_itemfactory;
	EnemyFactory* m_enemyFactory;
	UiResult* m_uiResult;
	UiTime* m_uiTime;
	UiStamina* m_uiStamina;
	CollisionStage* m_collisionStage;
	Inventory* m_inventory;
	Chest* m_chest;
	Map* m_map;
	NavMesh* m_navMesh;
	KeepChest* m_keepChest;
	KeepMoneyCount* m_keepMoneyCount;
	Item* m_item;
	DrawStageView* m_drawStageView;
	EscapePoint* m_escapePoint;
	MoneyCount* m_moneyCount;
	Wallet* m_wallet;
	ScreenFilter* m_screenFilter;
	EnhanceType* m_enhanceType;
	LightFactory* m_lightFactory;
	UiTheWorld* m_uiTheWorld;

	std::vector<Item> m_chestItem;

	int m_maxHaveInventory;
	int m_maxHaveWeight;
	float m_theWorldTime;
	int m_useCountTheWorld;
	float m_staminaRecovery;	//スタミナの回復量
	float m_staminaDecrease;	//スタミナの消費量

	int m_haveMoney;
	int m_restDays;
	int m_clearcount;

	// 脱出地点とプレイヤーのスポーン座標用
	Vector3 pos[PointNum] =
	{
		Vector3(9694, 0, -1876),	// 90
		Vector3(9699, 0, -256),		// 180
		Vector3(-3696, 0, 4381),	// -90
		Vector3(-8741, 0, 598),		// 180
		Vector3(-9671, 0, -1576),	// 0
		Vector3(4020, 0, -9387),	// 90
		Vector3(9800, 0, 9800),		// 0
		Vector3(-9800, 0, 9800),	// 0
		Vector3(-5400, 0, -8000),	// 0
		Vector3(9055, 0, -4712),	// 0
	};

	float Rotate[PointNum] =
	{
		90.0f,
		180.0f,
		-90.0f,
		180.0f,
		0.0f,
		90.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
	};

	int m_escapePointIndex;

public:
	// コンストラクタ
	SceneGame(std::vector<Item> chestItemList, EnhanceType* enhanceType, int haveMoney, MoneyCount* moneyCount) :
		m_rootNode(nullptr),
		m_loadPlayer(nullptr),
		m_cam(nullptr),
		m_collision3D(nullptr),
		m_itemfactory(nullptr),
		m_enemyFactory(nullptr),
		m_uiResult(nullptr),
		m_uiTime(nullptr),
		m_uiStamina(nullptr),
		m_item(nullptr),
		m_inventory(nullptr),
		m_collisionStage(nullptr),
		m_chest(nullptr),
		m_map(nullptr),
		m_navMesh(nullptr),
		m_keepChest(nullptr),
		m_keepMoneyCount(nullptr),
		m_chestItem(chestItemList),
		m_maxHaveInventory(0),
		m_maxHaveWeight(0),
		m_theWorldTime(0),
		m_useCountTheWorld(0),
		m_haveMoney(haveMoney),
		m_drawStageView(nullptr),
		m_escapePoint(nullptr),
		m_moneyCount(moneyCount),
		m_wallet(nullptr),
		m_restDays(0),
		m_screenFilter(nullptr),
		m_enhanceType(enhanceType),
		m_staminaRecovery(0),
		m_staminaDecrease(0),
		m_escapePointIndex(0),
		m_lightFactory(nullptr),
		m_uiTheWorld(nullptr){};

	virtual void Initialize() override;		// 初期化
	virtual void Finalize() override;		// 終了
	virtual SceneBase* Update() override;	// 更新
	virtual void Draw() override;			// 描画
};
