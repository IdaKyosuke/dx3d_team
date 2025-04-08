#include"NavMesh.h"
#include"Collision3D.h"
#include"Math.h"

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
	Vector3 firstPos;
	Vector3 targetPos;
	Vector3 polyPos[3];
	int checkPoly[3];
	int checkPolyPrev[3];
	int checkPolyNum;
	int checkPolyPrevNum;
	int nextCheckPoly[3];
	int nextCheckPolyPrev[3];
	int nextCheckPolyNum;
	int nextCheckPolyPrevNum;
	int num;

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
			// y���W��0�ɂ��ĕ��ʔ�����s��
			polyPos[0].y = 0.0f;
			polyPos[1].y = 0.0f;
			polyPos[2].y = 0.0f;

			// �|���S���̒��_(0, 1)�ɗאڂ���|���S�������݂��� && �ӂ̐����ƈړ��J�n�_�A�I���_�Ō`������������ڂ��Ă���
			if (m_polyLinkInfo[checkPoly[i]].linkPolyIndex[0] != -1 &&
				Segment_Segment_MinLength_SquareD(firstPos, targetPos, polyPos[0], polyPos[1]) < 0.001
				)
			{
				if (m_polyLinkInfo[checkPoly[i]].linkPolyIndex[0] == goalPoly) return true;

				// �ӂƐڂ��Ă���|���S�������̃`�F�b�N�Ώۂɒǉ�
				for (num = 0; num < nextCheckPolyNum; num++)
				{
					// ���łɒǉ�����Ă���ꍇ�ǉ����Ȃ�
					if (nextCheckPoly[num] == m_polyLinkInfo[checkPoly[i]].linkPolyIndex[0]) break;
				}
				if (num == nextCheckPolyNum)
				{
					// ���̃��[�v�ŏ��O����|���S���̑Ώۂɉ�����
					for (num = 0; num < nextCheckPolyPrevNum; num++)
					{
						// ���łɒǉ�����Ă���ꍇ�ǉ����Ȃ�
						if (nextCheckPolyPrev[num] == checkPoly[i]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						nextCheckPolyPrev[nextCheckPolyPrevNum] = checkPoly[i];
						nextCheckPolyPrevNum++;
					}

					// �P�O�̃��[�v�Ń`�F�b�N�ΏۂɂȂ����|���S���������Ȃ�
					for (num == 0; num < checkPolyPrevNum; num++)
					{
						if (checkPolyPrev[num] == m_polyLinkInfo[checkPoly[i]].linkPolyIndex[0]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						// �`�F�b�N�Ώۂɒǉ�
						nextCheckPoly[nextCheckPolyNum] = m_polyLinkInfo[checkPoly[i]].linkPolyIndex[0];
						nextCheckPolyNum++;
					}
				}
			}

			// �|���S���̒��_(1, 2)�ɗאڂ���|���S�������݂��� && �ӂ̐����ƈړ��J�n�_�A�I���_�Ō`������������ڂ��Ă���
			if (m_polyLinkInfo[checkPoly[i]].linkPolyIndex[1] != -1 &&
				Segment_Segment_MinLength_SquareD(firstPos, targetPos, polyPos[1], polyPos[2]) < 0.001
				)
			{
				if (m_polyLinkInfo[checkPoly[i]].linkPolyIndex[1] == goalPoly) return true;

				// �ӂƐڂ��Ă���|���S�������̃`�F�b�N�Ώۂɒǉ�
				for (num = 0; num < nextCheckPolyNum; num++)
				{
					// ���łɒǉ�����Ă���ꍇ�ǉ����Ȃ�
					if (nextCheckPoly[num] == m_polyLinkInfo[checkPoly[i]].linkPolyIndex[1]) break;
				}
				if (num == nextCheckPolyNum)
				{
					// ���̃��[�v�ŏ��O����|���S���̑Ώۂɉ�����
					for (num = 0; num < nextCheckPolyPrevNum; num++)
					{
						// ���łɒǉ�����Ă���ꍇ�ǉ����Ȃ�
						if (nextCheckPolyPrev[num] == checkPoly[i]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						nextCheckPolyPrev[nextCheckPolyPrevNum] = checkPoly[i];
						nextCheckPolyPrevNum++;
					}

					// �P�O�̃��[�v�Ń`�F�b�N�ΏۂɂȂ����|���S���������Ȃ�
					for (num == 0; num < checkPolyPrevNum; num++)
					{
						if (checkPolyPrev[num] == m_polyLinkInfo[checkPoly[i]].linkPolyIndex[1]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						// �`�F�b�N�Ώۂɒǉ�
						nextCheckPoly[nextCheckPolyNum] = m_polyLinkInfo[checkPoly[i]].linkPolyIndex[1];
						nextCheckPolyNum++;
					}
				}
			}

			// �|���S���̒��_(2, 0)�ɗאڂ���|���S�������݂��� && �ӂ̐����ƈړ��J�n�_�A�I���_�Ō`������������ڂ��Ă���
			if (m_polyLinkInfo[checkPoly[i]].linkPolyIndex[2] != -1 &&
				Segment_Segment_MinLength_SquareD(firstPos, targetPos, polyPos[2], polyPos[0]) < 0.001
				)
			{
				if (m_polyLinkInfo[checkPoly[i]].linkPolyIndex[2] == goalPoly) return true;

				// �ӂƐڂ��Ă���|���S�������̃`�F�b�N�Ώۂɒǉ�
				for (num = 0; num < nextCheckPolyNum; num++)
				{
					// ���łɒǉ�����Ă���ꍇ�ǉ����Ȃ�
					if (nextCheckPoly[num] == m_polyLinkInfo[checkPoly[i]].linkPolyIndex[2]) break;
				}
				if (num == nextCheckPolyNum)
				{
					// ���̃��[�v�ŏ��O����|���S���̑Ώۂɉ�����
					for (num = 0; num < nextCheckPolyPrevNum; num++)
					{
						// ���łɒǉ�����Ă���ꍇ�ǉ����Ȃ�
						if (nextCheckPolyPrev[num] == checkPoly[i]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						nextCheckPolyPrev[nextCheckPolyPrevNum] = checkPoly[i];
						nextCheckPolyPrevNum++;
					}

					// �P�O�̃��[�v�Ń`�F�b�N�ΏۂɂȂ����|���S���������Ȃ�
					for (num == 0; num < checkPolyPrevNum; num++)
					{
						if (checkPolyPrev[num] == m_polyLinkInfo[checkPoly[i]].linkPolyIndex[2]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						// �`�F�b�N�Ώۂɒǉ�
						nextCheckPoly[nextCheckPolyNum] = m_polyLinkInfo[checkPoly[i]].linkPolyIndex[2];
						nextCheckPolyNum++;
					}
				}
			}
		}

		// ���̃��[�v�̃`�F�b�N�Ώۂ��Ȃ��Ƃ��A�����I�ȓ������ł��Ȃ�
		if (nextCheckPolyNum == 0) return false;

		// ���̃`�F�b�N�Ώۂ̃|���S�������R�s�[
		for (int i = 0; i < nextCheckPolyNum; i++)
		{
			checkPoly[i] == nextCheckPoly[i];
		}

		checkPolyNum = nextCheckPolyNum;

		// ���̃`�F�b�N�ΏۊO�ɂȂ�|���S�����R�s�[
		for (int i = 0; i < nextCheckPolyPrevNum; i++)
		{
			checkPolyPrev[i] = nextCheckPolyPrev[i];
		}

		checkPolyPrevNum = nextCheckPolyPrevNum;
	}

	return false;
}

// �w��̂Q�_�Ԃ𒼐��I�Ɉړ��ł��邩�i���w��Łj
bool NavMesh::CheckPolyMove(Vector3 startPos, Vector3 goalPos, float width)
{
	Vector3 direction;
	Vector3 sideDirection;
	Vector3 tempVec;

	// �J�n���W����ڕW���W�ɒ����I�Ɉړ��ł��邩�`�F�b�N
	if (!CheckPolyMove(startPos, goalPos)) return false;

	// �J�n���W����ڕW���W�Ɍ������x�N�g�����Z�o
	direction = goalPos - startPos;

	// ���ʂȃx�N�g���ɕϊ�
	direction.y = 0;

	// �J�n���W����ڕW���W�ւ̃x�N�g���ɒ��p�Ȑ��K���x�N�g��
	sideDirection = direction.CrossP(Vector3(0, 1, 0));
	sideDirection = Math::Normalized(sideDirection);

	// �J�n���W�ƖڕW���W�𐂒�������width�̔����������炵�Ċm�F
	tempVec /= 2;
	if (!CheckPolyMove(startPos + tempVec, goalPos + tempVec)) return false;
	if (!CheckPolyMove(startPos - tempVec, goalPos - tempVec)) return false;

	// �w�蕝�������Ă������I�Ȉړ����\
	return true;
}

// �w��̂Q�_�Ԃ��o�H�T��
bool NavMesh::SetPathPlan(Vector3 startPos, Vector3 goalPos)
{
	int polyIndex;
	PathPlanUnit* pathUnit;
	PathPlanUnit* pathUnit_sub;
	PathPlanUnit* pathUnit_sub2;
	bool isGoal = false;

	// �X�^�[�g�ʒu�ƃS�[���ʒu��ۑ�
	m_startPos = startPos;
	m_endPos = goalPos;

	// �o�H�T���p�̃������m��
	m_unitArray = (PathPlanUnit*)malloc(sizeof(PathPlanUnit) * m_polyList.PolygonNum);

	// �o�H�T���p�̃|���S���̏�����
	pathUnit = m_unitArray;
	for (int i = 0; i < m_polyList.PolygonNum; i++, pathUnit++)
	{
		pathUnit->polyIndex = i;
		pathUnit->totalDistance = 0;
		pathUnit->prevPolyIndex = -1;
		pathUnit->nextPolyIndex = -1;
		pathUnit->activeNext = nullptr;
	}

	// �X�^�[�g�n�_�̃|���S���ԍ����擾 -> �o�H�T���p�̍\���̂̃A�h���X��ۑ�
	polyIndex = CheckPolyIndex(startPos);
	if (polyIndex == -1) return false;
	m_start = &m_unitArray[polyIndex];
	// �o�H�T���p�̃|���S���Ƃ��ăX�^�[�g�n�_�̃|���S����o�^
	m_activeFirst = &m_unitArray[polyIndex];

	// �S�[���n�_�ɂ���|���S���ԍ����擾 -> �o�H�T���p�̍\���̂̃A�h���X��ۑ�
	polyIndex = CheckPolyIndex(goalPos);
	if (polyIndex == -1) return false;
	m_goal = &m_unitArray[polyIndex];

	// �X�^�[�g�n�_�ƃS�[���n�_�̃|���S���������Ƃ�
	if (m_start == m_goal) return false;

	// �S�[���ɂ��ǂ蒅���܂ŒT��
	while (!isGoal)
	{
		// �T���Ώۂ̃|���S��������
		pathUnit = m_activeFirst;
		m_activeFirst = nullptr;
		for (; pathUnit; pathUnit = pathUnit->activeNext)
		{
			// �|���S���̕ӂ̐����J��Ԃ�
			for (int i = 0; i < 3; i++)
			{
				// �אڂ���|���S�����Ȃ��Ƃ�
				if (m_polyLinkInfo[pathUnit->polyIndex].linkPolyIndex[i] == -1) continue;

				// �אڂ���|���S�����o�H�T���������s���Ă��� && ��苗���̒����o�H  �������́@�X�^�[�g�n�_�̃|���S���̎�
				pathUnit_sub = &m_unitArray[m_polyLinkInfo[pathUnit->polyIndex].linkPolyIndex[i]];
				if (pathUnit_sub->prevPolyIndex != -1 &&
					pathUnit_sub->totalDistance <= pathUnit->totalDistance + m_polyLinkInfo[pathUnit->polyIndex].linkPolyDistance[i] ||
					pathUnit_sub->polyIndex == m_start->polyIndex
					)
				{
					continue;
				}

				// �אڂ���|���S�����S�[���n�_�̎��A�S�[���t���O�𗧂Ă�
				if (pathUnit_sub->polyIndex == m_goal->polyIndex)
				{
					isGoal = true;
				}

				// �אڂ���|���S���Ɏ����̃|���S��������
				pathUnit_sub->prevPolyIndex = pathUnit->polyIndex;

				// �אڂ���|���S���ɂ����܂ł̋�������
				pathUnit_sub->totalDistance = pathUnit->totalDistance + m_polyLinkInfo[pathUnit->polyIndex].linkPolyDistance[i];

				// ���̃��[�v�̒T���Ώۂ�ǉ�
				for (pathUnit_sub2 = m_activeFirst; pathUnit_sub2; pathUnit_sub2 = pathUnit_sub2->activeNext)
				{
					// ���łɒǉ�����Ă�����ǉ����Ȃ�
					if (pathUnit_sub2 == pathUnit_sub) break;
				}
				if (!pathUnit_sub2)
				{
					pathUnit_sub->activeNext = m_activeFirst;
					m_activeFirst = pathUnit_sub;
				}
			}
		}

		// m_activeFirst == nullptr �̎��A�S�[���ɂ��ǂ蒅���Ȃ�
		if (!m_activeFirst) return false;
	}

	// �S�[������X�^�[�g�܂ł��ǂ��āA�o�H��̎��Ɉړ�����ׂ��|���S���ԍ�����
	pathUnit = m_goal;
	do
	{
		pathUnit_sub = pathUnit;
		pathUnit = &m_unitArray[pathUnit_sub->polyIndex];
		pathUnit->nextPolyIndex = pathUnit_sub->polyIndex;
	} while (pathUnit != m_start);

	// �T���I��
	return true;
}

// �o�H�T�������폜
void NavMesh::RemovePathPlan()
{
	// �o�H�T���ׂ̈Ɋm�ۂ����������̈�����
	free(m_unitArray);
	m_unitArray = nullptr;
}

// �T���o�H�̈ړ������̏�����
void NavMesh::MoveInitialize()
{
	// �ړ��J�n���ɏ���Ă���|���S�����X�^�[�g�n�_�ɐݒ�
	m_nowPolyIndex = m_start->polyIndex;

	// �ړ��J�n���̍��W���|���S���̒��S���W�ɐݒ�
	m_nowPos = m_polyLinkInfo[m_nowPolyIndex].centerPos;

	// �ړ��J�n���̌o�H�T�������X�^�[�g�n�_�̃|���S�����ɂ���
	m_nowPathPlan = m_start;

	// �ړ��J�n���̈ړ����Ԓn�_�̌o�H�T�������X�^�[�g�n�_�̃|���S��
	m_targetPathPlan = m_start;
}

// �T���o�H�̈ړ�����
void NavMesh::Move()
{
	// �ړ������̍X�V�A�S�[���ɂ��ǂ蒅���Ă�����I��
	if (RefreshMoveDirection()) return;

	// �ړ������̍��W�Ɉړ�
	m_nowPos += m_moveDirection * MoveSpeed;

	// ���݂̍��W�ŏ���Ă���|���S��������
	m_nowPolyIndex = CheckPolyIndex(m_nowPos);

	// ����Ă���|���S���̌o�H�T�������A�h���X�ɑ��
	m_nowPathPlan = &m_unitArray[m_nowPolyIndex];
}

// �T���o�H�̈ړ��������X�V�itrue:�ڕW�n�_�ɓ��B, false:�ڕW�n�_�ɖ����B�j
bool NavMesh::RefreshMoveDirection()
{
	PathPlanUnit* tempPathUnit;

	// ���ݏ���Ă���|���S�����S�[���n�_�̃|���S���̏ꍇ�A�����𕪊�
	if (m_nowPathPlan == m_goal)
	{
		// �����͖ڕW���W
		m_moveDirection = m_endPos - m_nowPos;
		m_moveDirection.y = 0.0f;

		// �ڕW���W�܂ł̋������ړ����x�ȉ��Ȃ瓞�B�����Ƃ���
		if (m_moveDirection.Magnitude() <= MoveSpeed)
		{
			return true;
		}

		m_moveDirection = Math::Normalized(m_moveDirection);

		return false;
	}

	// ���ݏ���Ă���|���S�����ړ����Ԓn�_�̏ꍇ�A���̒��Ԓn�_�����肷�鏈��������
	if (m_nowPathPlan == m_targetPathPlan)
	{
		// ���Ԓn�_�����܂�܂Ń��[�v
		while (true)
		{
			tempPathUnit = &m_unitArray[m_targetPathPlan->nextPolyIndex];

			// �o�H��̎��̃|���S���̒��S���W�ɒ����ړ��ł��Ȃ��Ƃ��A���[�v���甲����
			if (CheckPolyMove(m_nowPos, m_polyLinkInfo[tempPathUnit->polyIndex].centerPos, Width)) break;

			// �`�F�b�N�Ώۂ��o�H��̍X�ɂP��̃|���S���ɕύX
			m_targetPathPlan = tempPathUnit;

			// �S�[���n�_�̃|���S���̎����[�v�𔲂���
			if (m_targetPathPlan == m_goal) break;
		}
	}

	// �ړ����������肷��i�ړ������͌��݂̍��W���璆�Ԓn�_�̃|���S���̒��S���W�Ɍ������j
	m_moveDirection = m_polyLinkInfo[m_targetPathPlan->polyIndex].centerPos - m_nowPos;
	m_moveDirection.y = 0;
	m_moveDirection = Math::Normalized(m_moveDirection);

	// �S�[���ɂ��ǂ蒅���Ă��Ȃ�
	return false;
}