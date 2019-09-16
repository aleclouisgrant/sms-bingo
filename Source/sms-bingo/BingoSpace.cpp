#include "BingoSpace.h"
#include <tuple>


BingoSpace::BingoSpace(std::string title)
{
	m_title = title;
}


BingoSpace::~BingoSpace()
{
}

std::string BingoSpace::GetText() {
	return m_title;
}

/*
Sets the conditions that must be met for the BingoSpace to be selectable
to a Player.
*/
void BingoSpace::AddCondition(int *var, int value) {
	m_condition = std::make_tuple(false, var, value);
}

/*
Checks all of the required conditions for the BingoSpace to be selectable
to that Player.
*/
bool BingoSpace::CheckConditions() {
	if (!std::get<0>(m_condition)) {
		if (*std::get<1>(m_condition) == std::get<2>(m_condition) ||
			*std::get<1>(m_condition) > std::get<2>(m_condition)) {
			std::get<0>(m_condition) = true;
			return true;
		}
	}
	return false;
}