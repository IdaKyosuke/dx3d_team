#include "Button.h"
#include "Input.h"

//コンストラクタ
Button::Button(const Vector2& size, int mouseButton, std::function<void()> callbackFunction) :
	m_size(size),
	m_mouseButton(mouseButton),
	m_isClickDown(false),
	m_callbackFunction(callbackFunction)
{
}

//マウスがボタンの範囲内にいるかどうか
bool Button::IsMouseContain(const Vector2& position)
{
	//矩形(ボタン)と点(マウス)の当たり判定
	Vector2 mousePoint = Input::GetInstance()->GetMousePoint();
	if (position.x - m_size.x / 2 <= mousePoint.x && mousePoint.x <= position.x + m_size.x / 2
	 && position.y - m_size.y / 2 <= mousePoint.y && mousePoint.y <= position.y + m_size.y / 2)
	{
		return true;
	}
	return false;
}

//更新
void Button::Update(const Vector2& position)
{
	//ボタン上でマウスボタンを押下したうえで、ボタン上でマウスボタンを離したら、ボタンが押されたことにする

	//ボタン上で、指定されたマウスボタンが押されたか
	if (Input::GetInstance()->IsMouseDown(m_mouseButton))
	{
		m_isClickDown = IsMouseContain(position);
	}

	//ボタン上で、指定されたマウスボタンが離れたかどうか
	if (m_isClickDown && Input::GetInstance()->IsMouseUp(m_mouseButton))
	{
		m_isClickDown = false;
		if (IsMouseContain(position))
		{
			//コールバック関数の呼び出し
			m_callbackFunction();
		}
	}
}

//描画
void Button::Draw(const Vector2& position)
{
#ifdef _DEBUG
	DrawBox(
		static_cast<int>(position.x - m_size.x / 2), static_cast<int>(position.y - m_size.y / 2),
		static_cast<int>(position.x + m_size.x / 2), static_cast<int>(position.y + m_size.y / 2),
		GetColor(0, 0, 255),0
	);
#endif 

}