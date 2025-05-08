#pragma once

struct ItemDate
{
public:
	const char* m_modelName;
	const char* m_itemName;
	int m_sellMoney;
	int m_weight;
	const char* m_flavorText;

	ItemDate(
		const char* modelName,
		const char* itemName,
		int sellMoney,
		int weight,
		const char* flavorText
	) :
		m_modelName(modelName),
		m_itemName(itemName),
		m_sellMoney(sellMoney),
		m_weight(weight),
		m_flavorText(flavorText){}
};