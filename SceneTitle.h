#pragma once
#include "SceneBase.h"

class Node;
class Chest;

// タイトルシーン
class SceneTitle : public SceneBase
{
private:
	Node* m_rootNode;
	int m_bgm;

	Chest* m_chest;
public:
	// コンストラクタ
	SceneTitle() :
		m_rootNode(nullptr),
		m_bgm(0),
		m_chest(nullptr){}

	virtual void Initialize() override;		// 初期化
	virtual void Finalize() override;		// 終了
	virtual SceneBase* Update() override;	// 更新
	virtual void Draw() override;			// 描画
};
