#pragma once

struct ItemDate
{
public:
	const char* m_modelName;
	const char* m_itemName;
	const char* m_iconName;
	int m_sellMoney;

	ItemDate(
		const char* modelName,
		const char* itemName,
		const char* iconName,
		int sellMoney
	) :
		m_modelName(modelName),
		m_itemName(itemName),
		m_iconName(iconName),
		m_sellMoney(sellMoney) {}
};