#include"ItemFactory.h"
#include"UiResult.h"
#include"Lerp.h"
#include"Time.h"
 
UiResult::UiResult(ItemFactory* itemFactory) :
	m_elapsedTime(0),
	m_showedBack(false),
	m_itemFactory(itemFactory),
	m_showTimeFlg(false),
	m_showPressFlg(false)
{
	// 背景のtransform
	m_backGroundTransform.position = BackGroundPos;
	m_backGroundTransform.scale = 0.0f;

	// clearTimeのtransform
	m_timeTransform.position = BackGroundPos + Vector2(0, -200);
	m_timeTransform.scale = 0.7f;

	// returnTitleのtransform
	m_returnTransform.position = BackGroundPos + Vector2(0, 200);
	m_returnTransform.scale = 0.5f;

	// リザルト画面の背景
	m_backGround.Register("result_backGround.png");
	m_clearTime.Register("game_clear.png");
	m_returnTitle.Register("press_any_key.png");
}

void UiResult::Load()
{
	m_backGround.Load();
	m_clearTime.Load();
	m_returnTitle.Load();
}

void UiResult::Release()
{
	m_backGround.Release();
	m_clearTime.Release();
	m_returnTitle.Release();
}

void UiResult::Update()
{
	if (!m_itemFactory->IsFinish()) return;

	// リザルト画面をすべて表示し終えたらカウントストップ
	if(!m_showPressFlg) m_elapsedTime += Time::GetInstance()->GetDeltaTime();

	if (!m_showedBack)
	{
		float t = m_elapsedTime / ShowBackGroundTime;
		if (t >= 1.0f)
		{
			t = 1.0f;
			m_showedBack = true;
			m_elapsedTime = 0;
		}

		float size = Lerp::Exec(StartBackGroundScale, EndBackGroundScale, t);
		m_backGroundTransform.scale = size;
	}
	else
	{
		if (m_elapsedTime >= 2.0f)
		{
			m_showPressFlg = true;
		}
		else if (m_elapsedTime >= 1.0f)
		{
			m_showTimeFlg = true;
		}
	}
}

void UiResult::Draw()
{
	if (!m_itemFactory->IsFinish()) return;
	m_backGround.Draw(m_backGroundTransform);

	if (m_showTimeFlg)
	{
		m_clearTime.Draw(m_timeTransform);
	}
	if (m_showPressFlg)
	{
		m_returnTitle.Draw(m_returnTransform);
	}
}


