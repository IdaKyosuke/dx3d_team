#pragma once

struct ItemDate
{
public:
	const char* m_modelName;
	const char* m_itemName;
	int m_sellMoney;

	ItemDate(
		const char* modelName,
		const char* itemName,
		int sellMoney
	) :
		m_modelName(modelName),
		m_itemName(itemName),
		m_sellMoney(sellMoney) {}
};