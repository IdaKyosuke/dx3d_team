#pragma once
#include"Vector3.h"
#include"DxLib.h"

// �|���S�����m�̘A�����
struct PolyLinkInfo
{
	int linkPolyIndex[3];	// �|���S����3�̕ӂƗאڂ��Ă���|���S���ԍ�(-1:�אڂȂ�, ����ȊO:�|���S���ԍ�)
	float linkPolyDistance[3];	// �אڂ��Ă���|���S���Ƃ̋���
	Vector3 centerPos;	// �|���S���̒��S���W
};

// �P�|���S���̏��
struct PathPlanUnit
{
	int polyIndex;	// �|���S���ԍ�
	float totalDistance;	// �|���S���ɓ��B����܂łɒʉ߂����|���S���Ԃ̋����̍��v
	int prevPolyIndex;	// �m�肵�Ă���P�O�̃|���S���i�|���S�����Ȃ��Ƃ���-1�j
	int nextPolyIndex;	// �m�肵�Ă���P��̃|���S���i�|���S�����Ȃ��Ƃ���-1�j
	PathPlanUnit* activeNext;	// �o�H�T���Ώۂ̎��̃|���S���̃A�h���X���i�[
};

class CollisionStage;

class NavMesh
{
private:
	const float MoveSpeed = 1.0f;
	const float Width = 1.0f;

	// �o�H�T���Ŏg�p�������ۑ�
	Vector3 m_startPos;	// �T���J�n�ʒu
	Vector3 m_endPos;	// �T���ڕW�ʒu
	PathPlanUnit* m_unitArray;	// �S�|���S���̏��z�񂪊i�[���ꂽ�������̈�̐擪�A�h���X
	PathPlanUnit* m_activeFirst;	// �|���S���Q�̍ŏ��̃|���S�����ւ̃A�h���X
	PathPlanUnit* m_start;	// �T���̃X�^�[�g�n�_�ɂ���|���S�����ւ̃A�h���X
	PathPlanUnit* m_goal;	// �T���̖ڕW�n�_�ɂ���|���S�����ւ̃A�h���X

	// �ړ������p
	int m_nowPolyIndex;	// ���ݏ���Ă���|���S���ԍ�
	Vector3 m_nowPos;	// ���݈ʒu
	Vector3 m_moveDirection;	// �ړ�����
	PathPlanUnit* m_nowPathPlan;	// ���ݏ���Ă���|���S���̌o�H�T����񂪊i�[����Ă���A�h���X
	PathPlanUnit* m_targetPathPlan;	// ���̒��Ԓn�_�ɂȂ�o�H��̃|���S�����ւ̃A�h���X

	MV1_REF_POLYGONLIST m_polyList;	// �X�e�[�W�̃|���S�����
	PolyLinkInfo* m_polyLink;	// �X�e�[�W���f���̑S�|���S�����̘A����񂪊i�[���ꂽ�z��

	CollisionStage* m_collisionStage;

public:
	NavMesh(CollisionStage* collisionStage);

	// �w����W�̒����A����̃|���S���ԍ����擾�i�Ȃ��ꍇ��-1�j
	int CheckPolyIndex(Vector3 pos);

	// �|���S�����m�̘A�������쐬
	void SetPolyLinkInfo();

	// �|���S�����m�̘A�����̍폜
	void RemovePolyLinkInfo();

	// �w��̂Q�_�Ԃ𒼐��I�Ɉړ��ł��邩
	bool CheckPolyMove(Vector3 startPos, Vector3 goalPos);

	// �w��̂Q�_�Ԃ𒼐��I�Ɉړ��ł��邩�i���w��Łj
	bool CheckPolyMoveWidth(Vector3 startPos, Vector3 goalPos, float width);

	// �w��̂Q�_�Ԃ��o�H�T��
	bool SetPathPlan(Vector3 startPos, Vector3 goalPos);
	
	// �o�H�T�������폜
	void RemovePathPlan();

	// �T���o�H�̈ړ������̏�����
	void MoveInitialize();

	// �T���o�H�̈ړ�����
	Vector3 Move(const Vector3& pos, const float speed);

	// �T���o�H�̈ړ��������X�V�itrue:�ڕW�n�_�ɓ��B, false:�ڕW�n�_�ɖ����B�j
	bool RefreshMoveDirection(const float speed);
};
