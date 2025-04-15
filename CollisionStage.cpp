#include"CollisionStage.h"
#include"LoadPlayer.h"

// �X�e�[�W�̓����蔻����쐬
CollisionStage::CollisionStage(const char* modelPath, const Vector3& pos) :
	m_cap1(Vector3(0, 0, 0)),
	m_cap2(Vector3(0, 0, 0))
{
	// �X�e�[�W�̓ǂݍ���
	m_model = MV1LoadModel(modelPath);

	MV1SetPosition(m_model, Vector3(0, 0, 0));
	// ���f���S�̂̃R���W���������\�z
	MV1SetupCollInfo(m_model, -1, 8, 8, 8);
	/*
	MV1SetupReferenceMesh(m_model, -1, true);
	m_refPoly = MV1GetReferenceMesh(m_model, -1, true);
	*/

	// ���C�̐ݒ�
	m_checkMoveStart = Vector3(0, 0, 0);
	m_checkMoveEnd = Vector3(0, 0, 0); -DiffCheckLine;

	m_getHeightStart = Vector3(0, 0, 0) + DiffGetLine;
	m_getHeightEnd = Vector3(0, 0, 0);

	// �\���̂̏�����
	m_stagePoly = MV1CollCheck_Line(m_model, -1, m_checkMoveStart, m_checkMoveEnd);
	m_polyHeight = MV1CollCheck_Line(m_model, -1, m_getHeightStart, m_getHeightEnd);
}

void CollisionStage::Release()
{
	MV1DeleteModel(m_model);
}

// �ړ��\���ɑ��ꂪ���邩�ǂ���
int CollisionStage::CheckStage(const Vector3& pos)
{
	m_checkMoveStart = pos;
	m_checkMoveEnd = pos - DiffCheckLine;

	// ���f���Ɛ����i�v���C���[�j�Ƃ̓����蔻��
	m_stagePoly = MV1CollCheck_Line(m_model, -1, m_checkMoveStart, m_checkMoveEnd);

	return m_stagePoly.HitFlag;
}

// �v���C���[�̑����܂ł̃��C
MV1_COLL_RESULT_POLY CollisionStage::GetHeight(const Vector3& pos, bool terning)
{
	if (!terning)
	{
		m_getHeightStart = pos + DiffGetLine;
		m_getHeightEnd = pos;
		m_getHeightEnd.y -= 1;
	}
	else
	{
		m_getHeightStart = pos - DiffGetLine;
		m_getHeightEnd = pos;
		m_getHeightEnd.y += 1;
	}
	m_polyHeight = MV1CollCheck_Line(m_model, -1, m_getHeightStart, m_getHeightEnd);

	return m_polyHeight;
}

// �v���C���[�̃J�v�Z���ƃX�e�[�W�̓����蔻����Ƃ�(�J�v�Z���ƃ��f���̓��������|���S����)
int CollisionStage::CapsuleCollider(const Vector3& pos, bool terning)
{
	if (!terning)
	{
		m_cap1 = pos + Vector3(0, Radius + DiffCapsule, 0);			// ���̔��a�� + �]�蕪���S����ɂ�����
		m_cap2 = pos + Vector3(0, CapsuleLength - Radius - DiffCapsule, 0);	// ���̋��������S���グ�āA���a��������
	}
	else
	{
		m_cap1 = pos - Vector3(0, Radius + DiffCapsule, 0);			// ���̔��a�� + �]�蕪���S����ɂ�����
		m_cap2 = pos - Vector3(0, CapsuleLength - Radius - DiffCapsule, 0);	// ���̋��������S���グ�āA���a��������
	}

	return MV1CollCheck_Capsule(m_model, -1, m_cap1, m_cap2, Radius).HitNum;

}

// �w��̍��W�̒����A�Ⴕ���͒���ɂ���|���S���̔ԍ����擾����
int CollisionStage::CheckOnPolyIndex(const Vector3& pos, const MV1_REF_POLYGONLIST& polyList)
{
	Vector3 linePos1;
	Vector3 linePos2;
	HITRESULT_LINE hitRes;
	MV1_REF_POLYGON* refPoly;

	// �w����W��Y�������ɑ傫���L�т�������Z�b�g
	linePos1 = Vector3(pos.x, 100000.0f, pos.z);
	linePos2 = Vector3(pos.x, -100000.0f, pos.z);

	// �X�e�[�W���f���̃|���S���������J��Ԃ�
	refPoly = polyList.Polygons;
	for (int i = 0; i < polyList.PolygonNum; i++, refPoly++)
	{
		// �����Ɛڂ���|���S����Ԃ�
		hitRes = HitCheck_Line_Triangle(
			linePos1,
			linePos2,
			polyList.Vertexs[refPoly->VIndex[0]].Position,
			polyList.Vertexs[refPoly->VIndex[1]].Position,
			polyList.Vertexs[refPoly->VIndex[2]].Position
			);

		if (hitRes.HitFlag) return i;
	}

	// �|���S���������ꍇ
	return -1;
}

void CollisionStage::Draw()
{
	MV1DrawModel(m_model);
	
#ifdef _DEBUG
	DrawLine3D(m_getHeightStart, m_getHeightEnd, GetColor(255, 255, 0));
	/*
	// �|���S���̐������J��Ԃ�
	for (int i = 0; i < m_refPoly.PolygonNum; i++)
	{
		// �|���S�����`������O���_���g�p���ă��C���[�t���[����`�悷��
		DrawLine3D(
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[0]].Position,
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[1]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[1]].Position,
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[2]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[2]].Position,
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[0]].Position,
			GetColor(255, 255, 0));
	}

	*/
	DrawCapsule3D(m_cap1, m_cap2, Radius, 8, GetColor(255, 255, 0), GetColor(255, 255, 0), FALSE);

	// �����������ǂ�����\������
	DrawFormatString(0, 80, GetColor(255, 255, 255), "HIT:%d", m_polyHeight.HitFlag);
#endif // DEBUG
}
