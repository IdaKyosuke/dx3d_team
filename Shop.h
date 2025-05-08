#pragma once
#include "Node.h"
#include "Sprite.h"
#include "Transform.h"
#include <vector>

class Chest;
class Wallet;
class SellButton;

class Shop : public Node
{
private:
	static constexpr Vector2 ShopUiPos = Vector2(760, 260);

	int m_takeItem;

	bool m_isSellItem;

	bool m_dontSellFlag;

	int m_seSell;

	Sprite m_shopUi;
	Transform m_shopTransform;

	Chest* m_chest;
	Wallet* m_wallet;
	SellButton* m_sellButton;


protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Shop(Chest* chest, Wallet* wallet,SellButton* sellButton);
};
