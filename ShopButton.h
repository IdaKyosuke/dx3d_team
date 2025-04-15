#pragma once
#include "Button.h"
#include "Actor.h"
#include "SceneBase.h"
#include "Vector2.h"

class ShopButton : public Actor
{
private:
	static constexpr Vector2 Position = Vector2(125, 100);
	static constexpr Vector2 Size = Vector2(50, 50);

	bool m_checkOnClick;

protected:
	Button m_button;	//�{�^���@�\	

	//�{�^���������ꂽ���ɌĂ΂��R�[���o�b�N�֐�
	void OnClick();

	virtual void Update() override;	//�X�V
	virtual void Draw() override;	//�`��

public:
	ShopButton();

	bool GetCheckOnClick()
	{
		return m_checkOnClick;
	}
};