#include"DrawStageView.h"
#include"ModelLoader.h"

DrawStageView::DrawStageView(const char* model)
{
	m_model = ModelLoader::LoadModel(model);

	// モデルの描画
	MV1DrawModel(m_model);
}

void DrawStageView::Draw()
{
	// モデルの描画
	MV1DrawModel(m_model);
}

void DrawStageView::Release()
{
	// ステージのモデルを削除
	MV1DeleteModel(m_model);
}