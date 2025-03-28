#pragma once
#include"Node.h"
#include"Vector3.h"
#include"DxLib.h"

class Collision3D : public Node
{
private:
	static constexpr Vector3 DiffCheckLine = Vector3(0, 500, 0);	// ���ꂪ���邩�ǂ������m�F���郌�C�̒���
	static constexpr Vector3 DiffGetLine = Vector3(0, 20, 0);	// �v���C���[�̑����܂ł̃��C�̒���
	static constexpr Vector3 ModelHeight = Vector3(0, 180, 0);		// �g�p���Ă��郂�f���̍���
	static constexpr float Radius = 30.0f;		// �J�v�Z���Ɏg�����̔��a
	static constexpr float CapsuleLength = 180.0f;	// �J�v�Z���̒����i�����m�̋����j
	static constexpr float DiffCapsule = 10.0f;	// �J�v�Z���������Ɠ���̔�����Ƃ�Ȃ����߂̍���

	int m_model;	// �ǂݍ��܂ꂽ���f���n���h��
	MV1_COLL_RESULT_POLY m_stagePoly;	// ���C�̐�ɂ��郁�b�V���̏��
	MV1_COLL_RESULT_POLY m_polyHeight;	// ���C�̐�ɂ��郁�b�V���̏��
	MV1_REF_POLYGONLIST m_refPoly;	// �X�e�[�W���f���S�̂̎Q�Ɨp���b�V�����

	// �ړ��\�͈͂̃`�F�b�N�p(�v���C���[�̑�����艺�ɒ���)
	Vector3 m_checkMoveStart;	
	Vector3 m_checkMoveEnd;

	// ������t�B�[���h�̍����擾�p�i�v���C���[�̑����܂ł̒����j
	Vector3 m_getHeightStart;
	Vector3 m_getHeightEnd;

	Vector3 m_cap1;
	Vector3 m_cap2;

protected:
	virtual void Release() override;	// ���\�[�X�̉��
	virtual void Draw() override;

public:
	Collision3D(const char* modelPath, const Vector3& pos);

	// �w����W�ɑ��ꂪ���邩�ǂ���
	int CheckStage(const Vector3& pos);

	// �w����W�����A����̃|���S���ԍ����擾
	int CheckPolyIndex(const Vector3& pos);

	// �w����W�̃��C(�\���̂�Ԃ�)
	MV1_COLL_RESULT_POLY GetHeight(const Vector3& pos, bool terning);

	// �v���C���[�̃J�v�Z���ƃX�e�[�W�̓����蔻����Ƃ�
	int CapsuleCollider(const Vector3& pos, bool terning);

	// �g�p���Ă��郌�C�̒������擾�i���]��Ԃ�y���W�擾�p�j
	Vector3 RayLength()
	{
		return DiffGetLine;
	}

	// �X�e�[�W�̃��b�V�������擾����
	MV1_REF_POLYGONLIST GetStageMesh()
	{
		return m_refPoly;
	}
};