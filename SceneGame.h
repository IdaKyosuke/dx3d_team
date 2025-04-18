#pragma once
#include "SceneBase.h"
#include <list>

class Node;
class LoadPlayer;
class Enemy;
class Collision3D;
class NavMesh;
class Camera;
class ItemFactory;
class UiScore;
class UiResult;
class CollisionStage;

class Item;
class Inventory;


// ゲームシーン
class SceneGame : public SceneBase
{
private:
	const std::list<const char*> ImagePreload =
	{

	};

	static constexpr float ResultTransitionTime = 2.0f;// プレイヤーが死んでからリザルト画面に遷移するまでの時間

	Node* m_rootNode;
	LoadPlayer* m_loadPlayer;
	Enemy* m_enemy;
	Collision3D* m_collision3D;
	NavMesh* m_navMesh;
	Camera* m_cam;
	CollisionStage* m_collisionStage;
	ItemFactory* m_itemfactory;			// アイテムのポインタ
	UiScore* m_uiScore;
	UiResult* m_uiResult;

	Item* m_item;
	Inventory* m_inventory;


	float m_resultTransitionTime;
	bool m_isFinish;



	int m_model;

public:
	// コンストラクタ
	SceneGame() :
		m_rootNode(nullptr),
		m_loadPlayer(nullptr),
		m_enemy(nullptr),
		m_navMesh(nullptr),
		m_cam(nullptr),
		m_resultTransitionTime(ResultTransitionTime),
		m_collisionStage(nullptr),
		m_itemfactory(nullptr),
		m_uiScore(nullptr),
		m_uiResult(nullptr),
		m_item(nullptr),
		m_inventory(nullptr),
		m_isFinish(false),
		m_model(0){}

	virtual void Initialize() override;		// 初期化
	virtual void Finalize() override;		// 終了
	virtual SceneBase* Update() override;	// 更新
	virtual void Draw() override;			// 描画
};
