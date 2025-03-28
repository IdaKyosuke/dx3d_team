#include"NavMesh.h"
#include"Collision3D.h"

NavMesh::NavMesh(Collision3D* collision3D) :
	m_collision3D(collision3D)
{

}

// �w����W�̒����A����̃|���S���ԍ����擾�i�Ȃ��ꍇ��-1�j
int NavMesh::CheckPolyIndex(Vector3 pos)
{
	return m_collision3D->CheckPolyIndex(pos);
}

// �|���S�����m�̘A�������쐬
void NavMesh::SetPolyLinkInfo()
{
	// �X�e�[�W�̃��b�V�������擾
	m_polyList = m_collision3D->GetStageMesh();

	// �������̈���m�ہimalloc�̓������u���b�N�����蓖�Ă�j
	m_polyLinkInfo = (PolyLinkInfo*)malloc(sizeof(PolyLinkInfo) * m_polyList.PolygonNum);

	// �S�|���S���̒��S���W���Z�o
	PolyLinkInfo* polyLinkInfo = m_polyLinkInfo;
	MV1_REF_POLYGON* refPoly = m_polyList.Polygons;
	for (int i = 0; i < m_polyList.PolygonNum; i++, refPoly++, polyLinkInfo++)
	{
		polyLinkInfo->centerPos = VScale(
			VAdd(m_polyList.Vertexs[refPoly->VIndex[0]].Position,
				VAdd(m_polyList.Vertexs[refPoly->VIndex[1]].Position, m_polyList.Vertexs[refPoly->VIndex[2]].Position)),
			1.0f / 3.0f
		);
	}

	// �|���S�����m�̗אڏ����쐬
	for (int i = 0; i < m_polyList.PolygonNum; i++, refPoly++, polyLinkInfo++)
	{
		// �ŏ��ɗאڏ������Z�b�g����
		for (int i = 0; i < 3; i++)
		{
			polyLinkInfo->linkPolyIndex[i] = -1;
		}

		// �אڂ���|���S����T�����߂Ƀ|���S���̐������J��Ԃ�

	}

}

// �|���S�����m�̘A�����̍폜
void NavMesh::RemovePolyLinkInfo()
{

}

// �w��̂Q�_�Ԃ𒼐��I�Ɉړ��ł��邩
bool NavMesh::CheckPolyMove(Vector3 startPos, Vector3 goalPos)
{
	return false;
}

// �w��̂Q�_�Ԃ��o�H�T��
bool NavMesh::SetPathPlan(Vector3 startPos, Vector3 goalPos)
{
	return false;
}

// �o�H�T�������폜
void NavMesh::RemovePathPlan()
{

}

// �T���o�H�̈ړ������̏�����
void NavMesh::MoveInitialize()
{

}

// �T���o�H�̈ړ�����
void NavMesh::Move()
{

}

// �T���o�H�̈ړ��������X�V�itrue:�ڕW�n�_�ɓ��B, false:�ڕW�n�_�ɖ����B�j
bool NavMesh::RefreshMoveDirection()
{
	return false;
}