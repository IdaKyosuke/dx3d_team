#pragma once
#include"Node.h"
#include"Vector3.h"
#include<vector>

class Camera;
class LoadPlayer;

class LightFactory : public Node
{
private:
	int m_lightHundle;
	Camera* m_cam;
	LoadPlayer* m_player;

	Vector3 m_hundlePos;

protected:
	void Release() override;
	void Update() override;
	void Draw() override;

public:
	LightFactory(Camera* camera, LoadPlayer* player);

};