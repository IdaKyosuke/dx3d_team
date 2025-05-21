#include "UiTheWorld.h"
#include "Screen.h"
#include "ImageLoader.h"

UiTheWorld::UiTheWorld() :
	Actor("UiTheWorld"),
	m_imageId(0)
{
	m_transform.position = Screen::TopLeft + Offset;
	
	m_sprite = new Sprite();
	m_sprite->Register("Alive", Animation("theworld_icon.png", GridNum));
	m_sprite->gridSize = GridSize;
}

void UiTheWorld::Load()
{
	Actor::Load();
	//m_imageId = ImageLoader::GetInstance()->Load("theworld_icon.png");
}

void UiTheWorld::Release()
{
	Actor::Release();
	//ImageLoader::GetInstance()->Delete("theworld_icon.png");
}

void UiTheWorld::Update()
{
	m_sprite->Play(AnimeName[0]);
}

void UiTheWorld::Draw()
{
	Actor::Draw();
}
