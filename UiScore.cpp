#include "UiScore.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "DxLib.h"

UiScore::UiScore() :
	m_fontTextureId(0),
	m_score(0),
	m_se(0),
	m_itemNum(0)
{
	m_transform.position = Screen::TopCenter + Offset;
	m_transform.scale = 0.3f;
	m_scoreImg.Register("item_count.png");
}

void UiScore::Load()
{
	m_scoreImg.Load();
	m_fontTextureId = ImageLoader::GetInstance()->Load("score_font.png");

	m_se = LoadSoundMem("Resource/sound/get_item.wav");
	ChangeVolumeSoundMem(128, m_se);
}

void UiScore::Release()
{
	m_scoreImg.Release();
	ImageLoader::GetInstance()->Delete("score_font.png");

	DeleteSoundMem(m_se);
}

void UiScore::Update()
{
	m_scoreImg.Update();
}

void UiScore::Draw()
{
	// SCORE画像を描画
	m_scoreImg.Draw(m_transform);

	Vector2 dest = m_transform.position + Vector2(ScoreSize.x / 4, -8);
	dest.y += FontMargin;
	int score = m_itemNum;
	int digit = 1;
	do
	{
		int value = score % 10;	// 1の位の値を取り出す

		DrawRectGraph(
			static_cast<int>(dest.x - FontSize.x * digit), static_cast<int>(dest.y),
			static_cast<int>(FontSize.x) * value, 0,
			static_cast<int>(FontSize.x), static_cast<int>(FontSize.y),
			m_fontTextureId,
			true
		);

		score /= 10;	// スコアの桁下げ
		digit++;		// 次の桁へ
	} while (score > 0);
}

// アイテム獲得後にカウントを減らす
void UiScore::GetItem(int num)
{
	m_itemNum -= num;
	PlaySoundMem(m_se, DX_PLAYTYPE_BACK);
}
