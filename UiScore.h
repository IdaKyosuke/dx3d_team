#pragma once
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class UiScore : public Node
{
private:
	static constexpr Vector2 ScoreSize = Vector2(40, 11);	// SCORE�摜�̕��E����
	static constexpr Vector2 FontSize = Vector2(20, 35);	// ����1�����̕��E����
	static constexpr Vector2 Offset = Vector2(0, 35);		// ��ʉE�ォ��̍���
	static constexpr int FontMargin = 5;					// �����Ɛ����̊Ԃ̗]��

	Sprite m_scoreImg;		// SCORE�摜
	int m_fontTextureId;	// �����t�H���g
	Transform m_transform;	// �p��
	int m_score;			// �X�R�A�̒l
	int m_se;
	int m_itemNum;			// �A�C�e���̑���

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	// �R���X�g���N�^
	UiScore();

	// �A�C�e���l����ɃJ�E���g�����炷
	void GetItem(int num);

	// �A�C�e�����̐ݒ�
	void SetNum(int num)
	{
		m_itemNum = num;
	}

	// ���W��ݒ�
	void SetPosition(const Vector2& position)
	{
		m_transform.position = position;
	}
};
