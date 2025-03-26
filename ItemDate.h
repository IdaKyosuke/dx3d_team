#pragma once

struct ItemDate
{
	const char* m_modelName;
	int m_sellMoney;

	ItemDate(
		const char* modelName,
		int sellMoney) :
		m_modelName(modelName),
		m_sellMoney(sellMoney) {}
};