#pragma once
#include"Sprite.h"
#include"Vector2.h"
#include"Screen.h"
#include"Node.h"

class ItemFactory;

class UiResult : public Node
{
private:
	static constexpr float ShowBackGroundTime = 1.0f;	// 拡大させるのにかかる時間
	static constexpr float StartBackGroundScale = 0.0f;	// 背景の拡大前のサイズ
	static constexpr float EndBackGroundScale = 1.0f;	// 背景の拡大後のサイズ
	static constexpr Vector2 BackGroundPos = Vector2(Screen::Center);	// 背景の位置

	float m_elapsedTime;
	Sprite m_backGround;	// リザルト画面の背景画像
	Sprite m_clearTime;		// クリアタイム（項目名）の画像
	Sprite m_returnTitle;	// タイトルに戻るための指示をする画像

	Transform m_backGroundTransform;
	Transform m_timeTransform;
	Transform m_returnTransform;

	ItemFactory* m_itemFactory;

	bool m_showedBack;	// 背景が表示されているかどうか

	bool m_showTimeFlg;
	bool m_showPressFlg;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	UiResult(ItemFactory* itemFactory);

	// リザルト画面を表示し終わったかどうか
	bool IsShowedPress()
	{
		return m_showPressFlg;
	}
};
