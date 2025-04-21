#pragma once
#include "Vector2.h"
#include <functional>	//std::functionを使うのに必要

class Button
{
private:
	Vector2 m_size;
	int m_mouseButton;	//マウスのどのボタンに反応するか
	bool m_isClickDown;	//↑のボタンが押されているか
	std::function<void()> m_callbackFunction;	//ボタンが押された時に実行する関数

	//マウスがボタンの範囲内にいるかどうか
	bool IsMouseContain(const Vector2& position);

public:
	//コンストラクタ
	Button(const Vector2& size, int mouseButton, std::function<void()> collbackFunction);
	
	void Update(const Vector2& position);	//更新
	void Draw(const Vector2& position);		//描画

};