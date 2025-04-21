#pragma once
#include "Button.h"
#include "Actor.h"
#include "SceneBase.h"
#include "Vector2.h"

class EnhanceButton : public Actor
{
private:
	static constexpr Vector2 Position = Vector2(125, 280);
	static constexpr Vector2 Size = Vector2(200, 100);

	bool m_shopOrEnhance;
	int m_shopEnhanceNum;

	bool m_checkOnClick;

protected:
	Button m_button;	//�{�^���@�\	

	//�{�^���������ꂽ���ɌĂ΂��R�[���o�b�N�֐�
	void OnClick();

	virtual void Update() override;	//�X�V
	virtual void Draw() override;	//�`��

public:
	EnhanceButton();

	bool GetCheckOnClick()
	{
		return m_checkOnClick;
	}
};
