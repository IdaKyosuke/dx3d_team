#include "Input.h"
#include "DxLib.h"
#include"Screen.h"
#include <cstring>

// 更新
void Input::Update()
{
	// キーボード

	// 前フレームのキー入力状態を保存
	std::memcpy(m_keyStatePost, m_keyState, sizeof(char) * KeyStateNum);

	// 現フレ―ムのキー入力状態を取得
	GetHitKeyStateAll(m_keyState);

	// マウス

	// 前フレームのマウスボタン入力状態を保存
	m_mouseButtonPost = m_mouseButton;

	// 現在のマウスボタン入力状態を取得
	m_mouseButton = DxLib::GetMouseInput();

	// マウス位置を取得
	int mouseX = 0;
	int mouseY = 0;
	DxLib::GetMousePoint(&mouseX, &mouseY);
	m_mousePoint = Vector2(mouseX, mouseY);

	// マウスホイールを取得
	m_mouseWheel = DxLib::GetMouseWheelRotVol();

	// マウスの左ボタンが入力されたら、Zキーが入力されたことにする
	if (m_mouseButton & MOUSE_INPUT_LEFT) m_keyState[KEY_INPUT_Z] = 1;
}

// いずれかのキーが押された瞬間
bool Input::IsAnyKeyDown()
{
	// マウス
	if (m_mouseButton && (m_mouseButton ^ m_mouseButtonPost))
	{
		return true;
	}

	// キーボード
	// 配列の比較
	if (std::memcmp(m_keyStatePost, m_keyState, sizeof(char) * KeyStateNum) == 0)
	{
		return false;
	}

	// 配列から1を探し出す
	return std::memchr(m_keyState, 1, sizeof(char) * KeyStateNum) != nullptr;
}

// マウス位置を設定する
void Input::SetMousePoint()
{
	int x = static_cast<int>(Screen::Center.x);
	int y = static_cast<int>(Screen::Center.y);
	DxLib::SetMousePoint(x, y);
}
