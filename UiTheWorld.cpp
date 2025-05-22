#include "UiTheWorld.h"
#include "ImageLoader.h"
#include "LoadPlayer.h"
#include "Time.h"
#include"Lerp.h"
#include"Math.h"
#include "Screen.h"

UiTheWorld::UiTheWorld(LoadPlayer* player) :
	Actor("UiTheWorld"),
	m_player(player),
	m_glassImageId(0),
	m_elapsedTime(0),
	m_isCoolDown(false),
	m_isStop(false),
	m_angle(0)
{
	m_transform.position = Screen::TopLeft + Offset;
	
	float t = (GridNum - 1) / m_player->GetStopTime();

	m_glass_backImageId.Register("hourglass_back.png");

	m_sprite = new Sprite();
	m_sprite->Register(AnimeName[0], Animation("hourglass.png", GridNum, t));
	m_sprite->gridSize = GridSize;
}

void UiTheWorld::Load()
{
	Actor::Load();
	m_glassImageId = ImageLoader::GetInstance()->Load("hourglass.png");
	m_glass_backImageId.Load();
}

void UiTheWorld::Release()
{
	Actor::Release();
	ImageLoader::GetInstance()->Delete("hourglass.png");
	m_glass_backImageId.Release();
}

void UiTheWorld::Update()
{
	m_glass_backImageId.Update();

	if (!m_isCoolDown)
	{
		// スキルがクールダウン中か取得
		m_isCoolDown = m_player->IsCoolDown();
	}
	else
	{
		if (!m_player->IsCoolDown())
		{
			m_elapsedTime += Time::GetInstance()->GetDeltaTime();
			float t = m_elapsedTime / ChangeAngleTime;
			if (t >= 1.0f)
			{
				t = 1.0f;
			}
			// クールダウン時間が終了したとき
			m_angle = Math::DegtoRad(Lerp::Exec(180, 0, t));
			if (m_angle == 0)
			{
				m_isCoolDown = false;
				m_elapsedTime = 0;
			}
		}
		else
		{
			m_angle = Math::DegtoRad(180);
		}
	}

	if (!m_isStop && m_player->IsTheWorld())
	{
		m_sprite->ResetElapsedTime();
	}

	// スキル使用中か取得
	m_isStop = m_player->IsTheWorld();

	m_sprite->Play(AnimeName[0]);
}

void UiTheWorld::Draw()
{
	if (m_isStop)
	{
		m_glass_backImageId.Draw(m_transform);
		// 時間停止中
		Actor::Draw();
	}
	else
	{
		// スキル使用中ではないとき
		if (m_isCoolDown)
		{
			// クールダウン中は暗化させる
			SetDrawBright(100, 100, 100);
			m_glass_backImageId.Draw(m_transform);
			DrawRectRotaGraph(
				static_cast<int>(m_transform.position.x), static_cast<int>(m_transform.position.y),
				static_cast<int>(GridSize.x) * 0, 0,
				static_cast<int>(GridSize.x), static_cast<int>(GridSize.y),
				m_transform.scale,
				m_angle,
				m_glassImageId,
				true,
				false, false
			);
			SetDrawBright(255, 255, 255);
		}
		else
		{
			m_glass_backImageId.Draw(m_transform);
			DrawRectRotaGraph(
				static_cast<int>(m_transform.position.x), static_cast<int>(m_transform.position.y),
				static_cast<int>(GridSize.x) * 0, 0,
				static_cast<int>(GridSize.x), static_cast<int>(GridSize.y),
				m_transform.scale,
				m_angle,
				m_glassImageId,
				true,
				false, false
			);
		}
	}
	
}
