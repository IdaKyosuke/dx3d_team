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

	Vector3 m_hundlePos;

protected:
	void Release() override;
	void Update() override;
	void Draw() override;

public:
	LightFactory(LoadPlayer* loadPlayer);

};