#include"EscapePoint.h"
#include"BoxCollider3D.h"

EscapePoint::EscapePoint() :
	Actor3D("Exit"),
	m_isEscape(false)
{
	// �E�o�n�_�̍��W��ݒ�
	int index = rand() % PointNum;
	m_transform.position = pos[index];

	m_collider = new BoxCollider3D(ColSize);
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