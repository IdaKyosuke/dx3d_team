#include "GameMain.h"
#include "GameConfig.h"
#include "Screen.h"
#include "SceneManager.h"
#include "SceneMenu.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneGameOver.h"
#include "Input.h"
#include "Time.h"
#include "ActorCollision.h"
#include "ActorCollision3D.h"
#include "Fade.h"
#include "ImageLoader.h"
#include "DxLib.h"

// デストラクタ
GameMain::~GameMain()
{
	// 自作スクリーンの破棄
	DeleteGraph(m_screen);

	// シーンの破棄
	delete m_sceneManager;
	m_sceneManager = nullptr;

	// DxLib 終了
	DxLib_End();
}

// メインループ実行
void GameMain::Run()
{
	SetMainWindowText(GameConfig::Title);	// ウィンドウタイトル
	SetAlwaysRunFlag(GameConfig::AlwaysRunFlag);	// 非アクティブ時に動作する
	ChangeWindowMode(GameConfig::WindowMode);	// ウィンドウモードで起動
	SetGraphMode(Screen::Width, Screen::Height, GameConfig::ColorBit);	// ウィンドウサイズ
	SetBackgroundColor(150, 150, 120, 255);
	Set3DSoundOneMetre(100.0f);	// 3D座標での１メートルの設定を変更（3D音源用）

	// ライティングをピクセル単位で行う場合、ステージの壁のポリゴン数を削れる
	SetUsePixelLighting(true);	// ライティングの計算をポリゴン単位 -> ピクセル単位に変更（重くなるらしいがそこまで変わらなかった）

	// DXライブラリの初期化
	if (DxLib_Init())
	{
		throw -1;
	}

	SetSysCommandOffFlag(true);	// Alt or F10キーで一時停止する機能をオフ

	// シーン起動
	m_sceneManager = new SceneManager(new SceneTitle());

	// スクリーン作成
	m_screen = MakeScreen(Screen::Width, Screen::Height);

	// ゲームループ
	while (ProcessMessage() == 0)
	{
		// 入力システムの更新
		Input::GetInstance()->Update();

		// 時間の更新
		Time::GetInstance()->Update();

		// ESCキーが押されたら終了
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_DELETE))
		{
			break;
		}

		// シーンの更新
		m_sceneManager->Update();

		// 衝突判定
		ActorCollision3D::GetInstance()->Update();

		// 自作スクリーンに描画
		SetDrawScreen(m_screen);

		// 画面に描かれているものを全部消す
		ClearDrawScreen();
		
		// シーンの描画
		m_sceneManager->Draw();

#ifdef _DEBUG
		// 衝突形状の描画
		ActorCollision3D::GetInstance()->Draw();

		// 画像ローダー
		ImageLoader::GetInstance()->Draw();
#endif

		// フェード
		Fade::GetInstance()->Update(m_screen);

		// 自作スクリーンを裏画面に描画
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		DrawGraph(0, 0, m_screen, false);

		// 裏画面と表画面をひっくり返す
		ScreenFlip();
	}
}
