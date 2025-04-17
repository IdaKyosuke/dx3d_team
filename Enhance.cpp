#include "Enhance.h"
#include "UiScore.h"
#include "DxLib.h"
#include "Chest.h"

//�R���X�g���N�^
Enhance::Enhance(
	EnhanceType::EnhanceTypeChoice enhanceTypeChoice,
	int useItem,
	int useMoney,
	const Vector2& position,
	const char* textureName,
	Chest* chest,
	EnhanceType* const enhanceType,
	Wallet* const wallet
) :
	Actor("Enhance", textureName, position),
	m_enhanceTypeChoice(enhanceTypeChoice),
	m_button(Size, MOUSE_INPUT_LEFT, std::bind(&Enhance::OnClick, this)),
	m_chest(chest),
	m_useItemNum(useItem),
	m_enhanceType(enhanceType),
	m_useMoney(useMoney),
	m_wallet(wallet)
{
	
}

//�{�^���������ꂽ���ɌĂ΂��R�[���o�b�N�֐�
void Enhance::OnClick()
{
	if (!CheckCondition()) return;

	m_useMoney = -m_useMoney;

	//�����ɕK�v�ȃA�C�e��������
	m_chest->LostItem(m_useItemNum);
	m_wallet->InWalletMoney(m_useMoney);

	//����
	m_enhanceType->Enhance(m_enhanceTypeChoice);
}

//�X�V
void Enhance::Update()
{
	//�{���̍X�V����
	Actor::Update();

	//�{�^��
	m_button.Update(m_transform.position);
}

//�`��@
void Enhance::Draw()
{
	//�����𖞂����ĂȂ��ꍇ�̓{�^�����É�������
	if (!CheckCondition())
	{
		//�ȍ~�A�P�x�������ĕ`�悷��
		SetDrawBright(50, 50, 50);
	}

	//�{���̕`�揈��
	Actor::Draw();

	//�P�x�̐ݒ�����ɖ߂�
	SetDrawBright(255, 255, 255);

#ifdef _DEBUG
	m_button.Draw(m_transform.position);
#endif
}