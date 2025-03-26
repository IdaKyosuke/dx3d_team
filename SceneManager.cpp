#include "SceneManager.h"
#include "SceneBase.h"
#include "Fade.h"

// デストラクタ
SceneManager::~SceneManager()
{
	// 実行中シーンと遷移先シーンが同じなら、多重deleteしない
	if (m_scene == m_nextScene)
	{
		m_nextScene = nullptr;
	}

	// 実行中シーンの破棄
	if (m_scene)
	{
		m_scene->Finalize();
		delete m_scene;
		m_scene = nullptr;
	}

	// 遷移先シーンの破棄
	if (m_nextScene)
	{
		m_nextScene->Finalize();
		delete m_nextScene;
		m_nextScene = nullptr;
	}
}

// 更新
void SceneManager::Update()
{
	switch (m_phase)
	{
	// 実行中
	case Phase::Run:
		// 実行中シーンの更新
		m_nextScene = m_scene->Update();

		// 遷移先シーンが実行中シーンと違ったら遷移開始
		if (m_nextScene != m_scene)
		{
			// フェードアウト開始
			m_phase = Phase::FadeOut;
			Fade::GetInstance()->StartFadeOut();
		}
		break;

	// フェードアウト
	case Phase::FadeOut:
		// フェードアウトが終わるまで待機
		if (Fade::GetInstance()->IsFade())
		{
			break;
		}

		// フェードが終わったのでシーン遷移する
		m_phase = Phase::Transition;
		break;

	// シーン遷移
	case Phase::Transition:
		// 遷移先シーンの初期化
		m_nextScene->Initialize();

		// 実行中シーンがあれば破棄する
		if (m_scene)
		{
			m_scene->Finalize();
			delete m_scene;
			m_scene = nullptr;
		}

		// 遷移先シーンを実行中シーンにする
		m_scene = m_nextScene;
		m_nextScene = nullptr;

		// フェードイン
		Fade::GetInstance()->StartFadeIn(0.2f);

		m_phase = Phase::Run;
		break;
	}
}

// 描画
void SceneManager::Draw()
{
	switch (m_phase)
	{
	case Phase::Run:
	case Phase::FadeOut:
		if (m_scene)
		{
			m_scene->Draw();
		}
		break;
	}
}
