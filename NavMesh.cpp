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
			VAdd(m_polyList.Vertexs[refPoly->VIndex[1]].Position, 
				m_polyList.Vertexs[refPoly->VIndex[2]].Position)),
			1.0f / 3.0f
		);
	}

	// �|���S�����m�̗אڏ����쐬
	polyLinkInfo = m_polyLinkInfo;
	refPoly = m_polyList.Polygons;
	PolyLinkInfo* polyLinkInfoSub;
	MV1_REF_POLYGON* refPolySub;

	for (int i = 0; i < m_polyList.PolygonNum; i++, refPoly++, polyLinkInfo++)
	{
		// �ŏ��ɗאڏ������Z�b�g����
		for (int j = 0; j < 3; j++)
		{
			polyLinkInfo->linkPolyIndex[i] = -1;
		}

		// �אڂ���|���S����T�����߂Ƀ|���S���̐������J��Ԃ�
		polyLinkInfoSub = m_polyLinkInfo;
		refPolySub = m_polyList.Polygons;

		for (int j = 0; j < m_polyList.PolygonNum; j++, refPolySub++, polyLinkInfoSub++)
		{
			// �������g�𖳎�
			if (i == j) continue;

			// �|���S�����_�ԍ�(0,1)�Ō`�������ӂƗאڂ��Ă�����אڏ��ɒǉ�
			if (
				polyLinkInfo->linkPolyIndex[0] == -1 &&
				(refPoly->VIndex[0] == refPolySub->VIndex[0] && refPoly->VIndex[1] == refPolySub->VIndex[2]) ||
				(refPoly->VIndex[0] == refPolySub->VIndex[1] && refPoly->VIndex[1] == refPolySub->VIndex[0]) ||
				(refPoly->VIndex[0] == refPolySub->VIndex[2] && refPoly->VIndex[1] == refPolySub->VIndex[1])
			)
			{
				polyLinkInfo->linkPolyIndex[0] = j;
				polyLinkInfo->linkPolyDistance[0] = VSize(VSub(polyLinkInfoSub->centerPos, polyLinkInfo->centerPos));
			}
			// �|���S�����_�ԍ�(1,2)�Ō`�������ӂƗאڂ��Ă�����אڏ��ɒǉ�
			else if(
				polyLinkInfo->linkPolyIndex[0] == -1 &&
				(refPoly->VIndex[1] == refPolySub->VIndex[0] && refPoly->VIndex[2] == refPolySub->VIndex[2]) ||
				(refPoly->VIndex[1] == refPolySub->VIndex[1] && refPoly->VIndex[2] == refPolySub->VIndex[0]) ||
				(refPoly->VIndex[1] == refPolySub->VIndex[2] && refPoly->VIndex[2] == refPolySub->VIndex[1])
				)
			{
				polyLinkInfo->linkPolyIndex[1] = j;
				polyLinkInfo->linkPolyDistance[1] = VSize(VSub(polyLinkInfoSub->centerPos, polyLinkInfo->centerPos));
			}
			// �|���S�����_�ԍ�(2,0)�Ō`�������ӂƗאڂ��Ă�����אڏ��ɒǉ�
			else if (
				polyLinkInfo->linkPolyIndex[0] == -1 &&
				(refPoly->VIndex[2] == refPolySub->VIndex[0] && refPoly->VIndex[0] == refPolySub->VIndex[2]) ||
				(refPoly->VIndex[2] == refPolySub->VIndex[1] && refPoly->VIndex[0] == refPolySub->VIndex[0]) ||
				(refPoly->VIndex[2] == refPolySub->VIndex[2] && refPoly->VIndex[0] == refPolySub->VIndex[1])
				)
			{
				polyLinkInfo->linkPolyIndex[2] = j;
				polyLinkInfo->linkPolyDistance[2] = VSize(VSub(polyLinkInfoSub->centerPos, polyLinkInfo->centerPos));
			}
		}
	}
}

// �|���S�����m�̘A�����̍폜
void NavMesh::RemovePolyLinkInfo()
{
	free(m_polyLinkInfo);
	m_polyLinkInfo = nullptr;
}

// �w��̂Q�_�Ԃ𒼐��I�Ɉړ��ł��邩�i�A�������g�p����j
bool NavMesh::CheckPolyMove(Vector3 startPos, Vector3 goalPos)
{
	int startPoly;
	int goalPoly;
	PolyLinkInfo* polyInfoStart;
	PolyLinkInfo* polyInfoGoal;
	//Vector3 startPos;
	//Vector3 targetPos;
	Vector3 polyPos[3];
	int checkPoly[3];
	int checkPolyPrev[3];
	int checkPolyNum;
	int checkPolyPrevNum;
	int nextCheckPoly[3];
	int nextCheckPolyPrev[3];
	int nextCheckPolyNum;
	int nextCheckPolyPrevNum;

	// �J�n���W�ƖڕW���W��Y��0�ɂ��ĕ��ʏ�̔�����s��
	startPos.y = 0;
	goalPos.y = 0;

	// �J�n���W�ƖڕW���W�̐^���̃|���S���𒲂ׂ�
	startPoly = CheckPolyIndex(startPos);
	goalPoly = CheckPolyIndex(goalPos);

	// �|���S�������݂��Ȃ��ꍇ
	if (startPoly == -1 || goalPoly == -1) return false;

	// �J�n���W�ƖڕW���W�̒��� or �����̃|���S����o�^
	polyInfoStart = &m_polyLinkInfo[startPoly];
	polyInfoGoal = &m_polyLinkInfo[goalPoly];


	// �w�肵��������ɂ��邩�𔻒f���邽�߂̃|���S����o�^�i���� or �����j
	checkPolyNum = 1;
	checkPoly[0] = startPoly;
	checkPolyPrevNum = 0;
	checkPolyPrev[0] = -1;

	// ���ʂ��o��܂Ń��[�v
	while (true)
	{
		// ���̃��[�v�ł̃`�F�b�N�Ώۂ̃|���S���������Z�b�g
		nextCheckPolyNum = 0;

		// ���̃��[�v�Ń`�F�b�N�Ώۂ���O���|���S���������Z�b�g
		nextCheckPolyPrevNum = 0;

		// �`�F�b�N�Ώۂ̃|���S�����������[�v
		for (int i = 0; i < checkPolyNum; i++)
		{

		}
	}

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