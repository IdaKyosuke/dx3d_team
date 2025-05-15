#include "Map.h"
#include "ImageLoader.h"
#include "Input.h"

Map::Map() :
	m_isOpenMap(false)
{
	m_transform.position = MapPos;
	m_drawMap.Register("map.png");
}

void Map::Load()
{
	m_drawMap.Load();
}

void Map::Release()
{
	m_drawMap.Release();
}

void Map::Update()
{
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_M))
	{
		m_isOpenMap = !m_isOpenMap;
	}
}

void Map::Draw()
{
	if (m_isOpenMap)
	{
		m_drawMap.Draw(m_transform);
	}
}
