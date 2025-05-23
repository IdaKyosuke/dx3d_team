#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "Input.h"
#include "PressAny.h"
#include "Node.h"
#include "DxLib.h"

void SceneGameOver::Initialize()
{
	m_rootNode = new Node();

	m_movieGraphHandle = LoadGraph("Resource/movie/GameOver.mp4");

	PlayMovieToGraph(m_movieGraphHandle);
}

void SceneGameOver::Finalize()
{
	m_rootNode->TreeRelease();

	delete m_rootNode;
	m_rootNode = nullptr;

	DeleteGraph(m_movieGraphHandle);
}

SceneBase* SceneGameOver::Update()
{
	if (GetMovieStateToGraph(m_movieGraphHandle) == 0)
	{
		if (Input::GetInstance()->IsAnyKeyDown())
		{
			return new SceneTitle();
		}
	}

	m_rootNode->TreeUpdate();

	return this;
}

void SceneGameOver::Draw()
{
	m_rootNode->TreeDraw();

	DrawExtendGraph(0, 0, 1280, 960, m_movieGraphHandle, FALSE);
}