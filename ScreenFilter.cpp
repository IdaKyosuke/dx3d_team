#include"ScreenFilter.h"
#include"LoadPlayer.h"
#include"Screen.h"
#include"DxLib.h"

const char* ScreenFilter::FilterTexture[2]
{
	"gray.png",
	"red.png",
};

ScreenFilter::ScreenFilter(LoadPlayer* player) :
	Actor("Filter", FilterTexture[1]),
	m_player(player)
{
	m_transform.position = Screen::Center;
}

void ScreenFilter::Draw()
{
	// �v���C���[�̗̑͂̊������擾
	float ratio = m_player->GetHpRatio();

	int pal = MaxPal - static_cast<int>(MaxPal * ratio);

	// �摜�𔼓����ŕ\��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
	Actor::Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
}
