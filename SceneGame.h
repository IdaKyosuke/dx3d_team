#pragma once
#include "SceneBase.h"
#include <list>

class Node;
class LoadPlayer;
class Camera;
class Collision3D;
class ItemFactory;
class UiScore;
class UiResult;

// ゲームシーン
class SceneGame : public SceneBase
{
private:
	const std::list<const char*> ImagePreload =
	{
		"bullet.png",
		"bomb_loop.png",
		"diamond.png"
	};

	static constexpr float ResultTransitionTime = 2.0f;// プレイヤーが死んでからリザルト画面に遷移するまでの時間

	Node* m_rootNode;
	LoadPlayer* m_loadPlayer;
	Camera* m_cam;
	Collision3D* m_collision3D;
	ItemFactory* m_itemfactory;			// アイテムのポインタ
	UiScore* m_uiScore;
	UiResult* m_uiResult;

	float m_resultTransitionTime;
	int m_bgm;
	bool m_isFinish;

public:
	// コンストラクタ
	SceneGame() :
		m_rootNode(nullptr),
		m_loadPlayer(nullptr),
		m_cam(nullptr),
		m_resultTransitionTime(ResultTransitionTime),
		m_collision3D(nullptr),
		m_itemfactory(nullptr),
		m_uiScore(nullptr),
		m_uiResult(nullptr),
		m_bgm(0),
		m_isFinish(false){}

	virtual void Initialize() override;		// 初期化
	virtual void Finalize() override;		// 終了
	virtual SceneBase* Update() override;	// 更新
	virtual void Draw() override;			// 描画
};
