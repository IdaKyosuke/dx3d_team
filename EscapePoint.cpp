#include"EscapePoint.h"
#include"BoxCollider3D.h"
#include "ModelLoader.h"
#include"Quaternion.h"
#include"Math.h"

EscapePoint::EscapePoint() :
	Actor3D("Exit"),
	m_isEscape(false)
{
	// �E�o�n�_�̍��W��ݒ�
	int index = rand() % PointNum;
	m_transform.position = pos[index];

	m_collider = new BoxCollider3D(ColSize, ColOffset);

	m_model = ModelLoader::LoadModel(m_stairPath);

	// ���f���̉�]
	MV1SetRotationXYZ(m_model, Vector3(0, Math::DegtoRad(Rotate[index]), 0));

	// ���f���̍��W�w��
	MV1SetPosition(m_model, m_transform.position);

	// ���f���̕`��
	MV1DrawModel(m_model);
}

void EscapePoint::OnCollision(const Actor3D* other)
{
	// �E�o�n�_�Ƀv���C���[������������
	if (other->GetName() == "Player")
	{
		// �E�o�ł������Ƃɂ���
		m_isEscape = true;
	}
}

void EscapePoint::Draw()
{
	// ���f���̕`��
	MV1DrawModel(m_model);
}