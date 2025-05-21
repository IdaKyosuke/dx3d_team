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
	m_imageId(0),
	m_elapsedTime(0),
	m_isCoolDown(false),
	m_isStop(false),
	m_angle(0)
{
	m_transform.position = Screen::TopLeft + Offset;
	
	float t = (GridNum - 1) / m_player->GetStopTime();

	m_sprite = new Sprite();
	m_sprite->Register(AnimeName[0], Animation("theworld_icon.png", 5, t));
	m_sprite->gridSize = GridSize;
}

void UiTheWorld::Load()
{
	Actor::Load();
	m_imageId = ImageLoader::GetInstance()->Load("theworld_icon.png");
}

void UiTheWorld::Release()
{
	Actor::Release();
	ImageLoader::GetInstance()->Delete("theworld_icon.png");
}

void UiTheWorld::Update()
{
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
			DrawRectRotaGraph(
				static_cast<int>(m_transform.position.x), static_cast<int>(m_transform.position.y),
				static_cast<int>(GridSize.x) * 0, 0,
				static_cast<int>(GridSize.x), static_cast<int>(GridSize.y),
				m_transform.scale,
				m_angle,
				m_imageId,
				true,
				false, false
			);
			SetDrawBright(255, 255, 255);
		}
		else
		{
			DrawRectRotaGraph(
				static_cast<int>(m_transform.position.x), static_cast<int>(m_transform.position.y),
				static_cast<int>(GridSize.x) * 0, 0,
				static_cast<int>(GridSize.x), static_cast<int>(GridSize.y),
				m_transform.scale,
				m_angle,
				m_imageId,
				true,
				false, false
			);
		}
	}
	
}
