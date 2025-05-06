#include "UiTime.h"
#include "Time.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "DxLib.h"

UiTime::UiTime() :
	m_fontTextureId(0),
	m_limitTime(LimitTime),
	m_isFinsh(false)
{
	m_transform.position = Screen::TopRight + Offset;
}

void UiTime::Load()
{
	m_fontTextureId = ImageLoader::GetInstance()->Load("score_font.png");
}

void UiTime::Release()
{
	ImageLoader::GetInstance()->Delete("score_font.png");
}

void UiTime::Update()
{
	m_limitTime -= Time::GetInstance()->GetDeltaTime();

	if (m_limitTime <= 0)
	{
		m_isFinsh = true;
	}
}

void UiTime::Draw()
{
	Vector2 dest = m_transform.position + TimeSize / 2;
	dest.y += FontMargin;
	int score = static_cast<int>(m_limitTime);
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
