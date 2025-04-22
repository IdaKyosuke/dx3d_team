#include"EscapePoint.h"
#include"BoxCollider3D.h"
#include "ModelLoader.h"

EscapePoint::EscapePoint() :
	Actor3D("Exit"),
	m_isEscape(false)
{
	// �E�o�n�_�̍��W��ݒ�
	int index = rand() % PointNum;
	m_transform.position = pos[index];

	m_collider = new BoxCollider3D(ColSize, ColOffset);

	m_model = ModelLoader::LoadModel(m_stairPath);

	// ���f���̕`��
	MV1DrawModel(m_model);

	//�|�W�V�����̐ݒ�
	MV1SetPosition(m_model, m_transform.position);
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