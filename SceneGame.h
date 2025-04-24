#pragma once
#include "SceneBase.h"
#include "Item.h"
#include <vector>

class Node;
class LoadPlayer;
class Camera;
class Collision3D;
class ItemFactory;
class EnemyFactory;
class UiScore;
class UiResult;
class CollisionStage;
class Inventory;
class Chest;
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

// ゲームシーン
class SceneGame : public SceneBase
{
private:
	static constexpr float ResultTransitionTime = 2.0f;// プレイヤーが死んでからリザルト画面に遷移するまでの時間

	Node* m_rootNode;

	LoadPlayer* m_loadPlayer;
	Camera* m_cam;
	Collision3D* m_collision3D;
	ItemFactory* m_itemfactory;
	EnemyFactory* m_enemyFactory;
	UiScore* m_uiScore;
	UiResult* m_uiResult;
	CollisionStage* m_collisionStage;
	Inventory* m_inventory;
	Chest* m_chest;
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

	std::vector<Item> m_chestItem;

	float m_resultTransitionTime;
	bool m_isFinish;

	int m_maxHaveInventory;
	int m_theWorldTime;
	int m_useCountTheWorld;

	int m_haveMoney;
	int m_restDays;

public:
	// コンストラクタ
	SceneGame(std::vector<Item> itemList, EnhanceType* enhanceType, int haveMoney, MoneyCount* moneyCount) :
		m_rootNode(nullptr),
		m_loadPlayer(nullptr),
		m_cam(nullptr),
		m_resultTransitionTime(ResultTransitionTime),
		m_collision3D(nullptr),
		m_itemfactory(nullptr),
		m_enemyFactory(nullptr),
		m_uiScore(nullptr),
		m_uiResult(nullptr),
		m_item(nullptr),
		m_inventory(nullptr),
		m_collisionStage(nullptr),
		m_chest(nullptr),
		m_navMesh(nullptr),
		m_keepChest(nullptr),
		m_keepMoneyCount(nullptr),
		m_chestItem(itemList),
		m_maxHaveInventory(0),
		m_theWorldTime(0),
		m_useCountTheWorld(0),
		m_haveMoney(haveMoney),
		m_drawStageView(nullptr),
		m_isFinish(false),
		m_escapePoint(nullptr),
		m_moneyCount(moneyCount),
		m_wallet(nullptr),
		m_restDays(0),
		m_screenFilter(nullptr),
		m_enhanceType(enhanceType) {};

	virtual void Initialize() override;		// 初期化
	virtual void Finalize() override;		// 終了
	virtual SceneBase* Update() override;	// 更新
	virtual void Draw() override;			// 描画
};
