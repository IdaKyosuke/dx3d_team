#include"LightFactory.h"
#include"LoadPlayer.h"
LightFactory::LightFactory(LoadPlayer* loadPlayer) :
	m_player(loadPlayer),
	m_lightHundle(0)
{
	m_lightHundle = CreatePointLightHandle(m_player->GetPosition(), 2500.0f, 0.0f, 0.002f, 0.0f);
	// 効果がわかり易いように標準ライトを無効化
	SetLightEnable(false);
}

void LightFactory::Update()
{
	// ライトの位置をプレイヤーに合わせる
	SetLightPositionHandle(m_lightHundle, m_player->GetPosition());
}

void LightFactory::Release()
{
	// ライトハンドルを削除
	DeleteLightHandle(m_lightHundle);
}