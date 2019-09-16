#pragma once
#include <string>
#include <tuple>


class BingoSpace
{

public:
	BingoSpace(std::string title);
	~BingoSpace();

	std::string GetText();
	void AddCondition(int *var, int val);
	bool CheckConditions();

private:
	std::string m_title;
	std::tuple <bool, int *, int> m_condition;
};