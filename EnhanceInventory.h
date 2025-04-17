#pragma once
#include "Enhance.h"
#include "Button.h"
#include "Actor.h"
#include "SceneBase.h"
#include "Vector2.h"
#include <vector>

class Chest;
class EnhanceType;
class Wallet;

class EnhanceInventory : public Enhance
{
private:
	static constexpr Vector2 Position = Vector2(400, 280);
	static constexpr int NeedItemVolume = 1;
	static constexpr int NeedMoney = 100;

	int m_needItemCount;

	bool m_canEnhance;

	int m_needItemNum;		//�����ɕK�v�ȃA�C�e���̃i���o�[

	Chest* m_chest;
	Wallet* m_wallet;
	EnhanceType* m_enhanceType;

	int m_useItemNum;

protected:
	//�{�^�����L�����ǂ������`�F�b�N
	virtual bool CheckCondition() override;

public:
	EnhanceInventory(Chest* chest,Wallet* wallet,EnhanceType* enhanceType);
};
