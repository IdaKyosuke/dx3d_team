#pragma once

class SceneManager;

// ゲームメインループクラス
class GameMain
{
private:
	SceneManager* m_sceneManager;	// シーン遷移
	int m_screen;	// 自作スクリーン

public:
	// コンストラクタ
	GameMain() :
		m_sceneManager(nullptr),
		m_screen(0)
	{
		// 何もしない
	}

	// デストラクタ
	virtual ~GameMain();
	
	// メインループ実行
	void Run();
};