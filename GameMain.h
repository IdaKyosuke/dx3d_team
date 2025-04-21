#pragma once

class SceneManager;
class Chest;

// ゲームメインループクラス
class GameMain
{
private:
	SceneManager* m_sceneManager;	// シーン遷移
	int m_screen;	// 自作スクリーン

	Chest* m_chest;

public:
	// コンストラクタ
	GameMain() :
		m_sceneManager(nullptr),
		m_screen(0),

		m_chest(nullptr)
	{
		// 何もしない
	}

	// デストラクタ
	virtual ~GameMain();
	
	// メインループ実行
	void Run();
};