#include "EnhanceStamina.h"
#include "Chest.h"
#include "Wallet.h"

EnhanceStamina::EnhanceStamina(Chest* chest, Wallet* wallet, EnhanceType* enhanceType) :
	Actor("Enhance", "enhance_stamina.png", Position),
	m_button(Size, MOUSE_INPUT_LEFT, std::bind(&EnhanceStamina::OnClick, this)),
	m_chest(chest),
	m_canEnhance(false),
	m_needItemNum(0),
	m_useItemNum(0),
	m_enhanceCount(0),
	m_needMoney(FirstNeedMoney),
	m_enhanceType(enhanceType),
	m_wallet(wallet),
	m_enhanceTypeChoice(EnhanceType::EnhanceTypeChoice::EnhanceStamina)
{
	m_needItemNum = 2;
}

//�X�V
void EnhanceStamina::Update()
{
	//�{���̍X�V����
	Actor::Update();

	//�������Ă������ƂɕK�v�f�ނ𑝂₷
	if (m_enhanceType->GetAddStaminaRecovery() >= 2)
	{
		m_needMoney = NeedMoney[0];
	}
	if (m_enhanceType->GetAddStaminaRecovery() >= 6)
	{
		m_needMoney = NeedMoney[1];
	}
	if (m_enhanceType->GetAddStaminaRecovery() >= 8)
	{
		m_needMoney = NeedMoney[2];
	}

	//�{�^��
	m_button.Update(m_transform.position);
}

//�`��@
void EnhanceStamina::Draw()
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

//�{�^���������ꂽ���ɌĂ΂��R�[���o�b�N�֐�
void EnhanceStamina::OnClick()
{
	if (!CheckCondition()) return;

	//�����ɕK�v�ȃA�C�e��������
	m_chest->LostItem(m_useItemNum);
	m_wallet->LostMoney(m_needMoney);

	//����
	m_enhanceType->Enhance(m_enhanceTypeChoice);
}

//�{�^�����L�����ǂ����`�F�b�N
bool EnhanceStamina::CheckCondition()
{
	//���߂��l�܂ŋ���������I��
	if (m_enhanceType->GetAddStaminaRecovery() < EndEnhanceCount)
	{
		//���������邩�������邩�̔���
		if (!m_chest->GetItemList().empty())
		{
			for (int i = 0; i <= m_chest->GetItemList().size() - 1; i++)
			{
				if (m_needItemNum == std::next(m_chest->GetItemList().begin(), i)->GetItemNum() &&
					m_needMoney <= m_wallet->HaveMoney())
				{
					m_canEnhance = true;
					m_useItemNum = i;
				}
				if (m_enhanceType->OnStaminaButton())
				{
					m_canEnhance = false;
					m_enhanceType->StaminaButtonReset();
				}
			}
		}
		else
		{
			m_canEnhance = false;
		}
	}

	return m_canEnhance;
}