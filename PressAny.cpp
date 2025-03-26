#include "PressAny.h"
#include "Time.h"
#include "Screen.h"

// コンストラクタ
PressAny::PressAny() :
	Actor(
		"PressAny",
		"press_any_button.png",
		Screen::BottomCenter - Vector2(0, Screen::Height / 4)
	),
	m_elapsedTime(0),
	m_drawFlag(true)
{
}

// 更新
void PressAny::Update()
{
	// 一定時間経過したら、描画フラグを入れ替える
	m_elapsedTime += Time::GetInstance()->GetDeltaTime();
	if (m_elapsedTime >= BlinkDuration)
	{
		m_elapsedTime -= BlinkDuration;
		m_drawFlag = !m_drawFlag;
	}
}

// 描画
void PressAny::Draw()
{
	if (!m_drawFlag) return;

	// 本来の描画処理
	Actor::Draw();
}
