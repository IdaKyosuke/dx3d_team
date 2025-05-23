#include"ScreenFilter.h"
#include"LoadPlayer.h"
#include"Screen.h"
#include"DxLib.h"

const char* ScreenFilter::FilterTexture[FilterNum]
{
	"gray.png",
	"red.png",
	"theWorld_filter.png",
};

ScreenFilter::ScreenFilter(LoadPlayer* player) :
	m_player(player)
{
	// ダメージ用フィルター
	m_grayImg.Register(FilterTexture[0]);
	m_redImg.Register(FilterTexture[1]);
	// 時間停止中のフィルター
	m_whiteImg.Register(FilterTexture[2]);

	m_transform.position = Screen::Center;
}

void ScreenFilter::Load()
{
	m_grayImg.Load();
	m_redImg.Load();
	m_whiteImg.Load();
}

void ScreenFilter::Release()
{
	m_grayImg.Release();
	m_redImg.Release();
	m_whiteImg.Release();
}

void ScreenFilter::Update()
{
	m_grayImg.Update();
	m_redImg.Update();
	m_whiteImg.Update();
}

void ScreenFilter::Draw()
{
	// 時間停止中だけ白っぽい画面になる
	if (m_player->IsTheWorld())
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		m_whiteImg.Draw(m_transform);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
	}

	// プレイヤーの体力の割合を取得
	float ratio = m_player->GetHpRatio();
	int pal = MaxPal - static_cast<int>(MaxPal * ratio);

	// 画像を半透明で表示
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
	m_grayImg.Draw(m_transform);
	m_redImg.Draw(m_transform);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
}
