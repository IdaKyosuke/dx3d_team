#pragma once
#include "Actor.h"
#include "Button.h"
#include "Vector2.h"
#include "EnhanceType.h"
#include  "Wallet.h"

#include <vector>

class Chest;

class Enhance : public Actor
{
private:
	static constexpr Vector2 Size = Vector2(200, 100);

	int m_useItemNum;
	int m_useMoney;

	Chest* m_chest;

protected:
	EnhanceType::EnhanceTypeChoice m_enhanceTypeChoice;	//�����ꂽ���ɋ����������
	Button m_button;	//�{�^���@�\

	EnhanceType* m_enhanceType;
	Wallet* m_wallet;

	//�{�^���������ꂽ���ɌĂ΂��R�[���o�b�N�֐�
	void OnClick();

	//�{�^�����L�����ǂ������`�F�b�N
	virtual bool CheckCondition() = 0;

	virtual void Update() override;	//�X�V
	virtual void Draw() override;	//�`��

public:
	//�R���X�g���N�^
	Enhance(
		EnhanceType::EnhanceTypeChoice enhanceTypeChoice,
		int useItem,
		int useMoney,
		const Vector2& position,
		const char* textureName,
		Chest* chest,
		EnhanceType* const enhanceType,
		Wallet* const wallet
	);
};