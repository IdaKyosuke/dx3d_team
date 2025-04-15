#include"Sphere.h"
#include"Lerp.h"
#include"Time.h"
#include"LoadPlayer.h"
#include"ItemFactory.h"
#include"ModelLoader.h"
#include"BoxCollider3D.h"

Sphere::Sphere(const Vector3& pos, ItemFactory* itemFactory) :
	Actor3D("Sphere", pos),
	m_itemFactory(itemFactory),
	m_rotate(Vector3(0, 0, 0)),
	m_froatHeight(FroatHeight),
	m_elapsedTime(0),
	m_changeDir(false),
	m_dist(0)
{
	// ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý
	m_model = ModelLoader::LoadModel(PathName);
	// ƒ‚ƒfƒ‹‚ðŽw’èêŠ‚É•`‰æ
	MV1SetPosition(m_model, m_transform.position);
	MV1DrawModel(m_model);

	m_collider = new BoxCollider3D(Vector3(200, 200, 200));

	m_startY = m_transform.position.y;
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

	m_transform.position.y = Lerp::Exec(m_startY, m_startY + m_froatHeight, t);

	// —h‚ê‚éŒü‚«‚ð”½“]‚³‚¹‚é
	if (m_changeDir)
	{
		m_changeDir = false;
		m_froatHeight = -m_froatHeight;
		m_startY = m_transform.position.y;
		m_elapsedTime = 0;
	}

	// ƒAƒCƒeƒ€‚ð‰ñ“]‚³‚¹‚é
	MV1SetRotationXYZ(m_model, m_rotate);
	m_rotate.y += RotateSpeed;
	if (m_rotate.y >= 2.5f) m_rotate.y = 0;
}

void Sphere::Update()
{
	MoveSphere();
}

void Sphere::Draw()
{
	// ƒ‚ƒfƒ‹‚ðŽw’èêŠ‚É•`‰æ
	MV1SetPosition(m_model, m_transform.position);
	MV1DrawModel(m_model);
}

void Sphere::OnCollision(const Actor3D* other)
{
	if (other->GetName() == "Player")
	{
		m_itemFactory->ItemCount();
		Destroy();
	}
}