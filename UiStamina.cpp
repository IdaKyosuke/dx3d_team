#include "UiStamina.h"
#include "Time.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "Input.h"
#include "DxLib.h"

UiStamina::UiStamina() :
	m_topLeft(LeftEdge),
	m_bottomRight(RightEdge),
	m_color(0)
{
}

void UiStamina::Update()
{
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT))
	{
		m_topLeft++;
		m_bottomRight--;
	}
	else
	{
		if (m_topLeft >= LeftEdge) m_topLeft--;
		if (m_bottomRight <= RightEdge) m_bottomRight++;
		
	}
}

void UiStamina::Draw()
{
	m_color = GetColor(200, 200, 200);

	DrawBox(m_topLeft, 800, m_bottomRight, 819, m_color, TRUE);    // ŽlŠpŒ`‚ð•`‰æ
}