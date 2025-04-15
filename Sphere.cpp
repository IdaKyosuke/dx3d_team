#include"Sphere.h"
#include"Lerp.h"
#include"Time.h"
#include"LoadPlayer.h"
#include"ItemFactory.h"
#include"ModelLoader.h"

Sphere::Sphere(LoadPlayer* loadPlayer, const Vector3& pos, ItemFactory* itemFactory) :
	Actor("Sphere"),
	m_loadPlayer(loadPlayer),
	m_itemFactory(itemFactory),
	m_pos(pos),
	m_rotate(Vector3(0,0,0)),
	m_froatHeight(FroatHeight),
	m_elapsedTime(0),
	m_changeDir(false),
	m_dist(0)
{
	// モデルの読み込み
	m_model = ModelLoader::LoadModel(PathName);
	// モデルを指定場所に描画
	MV1SetPosition(m_model, m_pos);
	MV1DrawModel(m_model);

	m_startY = m_pos.y;
}

Sphere::~Sphere()
{
	MV1DeleteModel(m_model);
}

void Sphere::MoveSphere()
{
	m_elapsedTime += Time::GetInstance()->GetDeltaTime();
	float t = m_elapsedTime / LerpTime;

	if (t >= 1.0f)
	{
		t = 1.0f;
		m_changeDir = true;
	}

	m_pos.y = Lerp::Exec(m_startY, m_startY + m_froatHeight, t);

	// 揺れる向きを反転させる
	if (m_changeDir)
	{
		m_changeDir = false;
		m_froatHeight = -m_froatHeight;
		m_startY = m_pos.y;
		m_elapsedTime = 0;
	}

	// アイテムを回転させる
	MV1SetRotationXYZ(m_model, m_rotate);
	m_rotate.y += RotateSpeed;
	if (m_rotate.y >= 2.5f) m_rotate.y = 0;
}

void Sphere::CheckDistance()
{
	m_dist = (m_pos - m_loadPlayer->GetPosition()).Magnitude();
	if (m_dist <= DeleteDistance)
	{
		m_itemFactory->ItemCount();
		Destroy();
	}
}

void Sphere::Update()
{
	MoveSphere();
	CheckDistance();
}

void Sphere::Draw()
{
	// モデルを指定場所に描画
	MV1SetPosition(m_model, m_pos);
	MV1DrawModel(m_model);
}