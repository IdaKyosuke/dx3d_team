#pragma once
#include"Node.h"
#include"Vector3.h"
#include<vector>

class LoadPlayer;

class LightFactory : public Node
{
private:
	int m_lightHundle;
	LoadPlayer* m_player;

protected:
	void Release() override;
	void Update() override;

public:
	LightFactory(LoadPlayer* loadPlayer);

};