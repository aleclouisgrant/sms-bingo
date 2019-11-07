#pragma once
enum class GameOptions : unsigned {

	/* Privacy */
	Public = 0x1,
	SpectatorsAllowed,
	
	/* Special Rules */
	Lockout,
	Connect_Five,
	Row_Control,
	Free_Space,

	/* Delayed Clicks */
	Allow_Delayed_Click,
	Delayed_Click_Timer,
	Show_Delayed_Click

	//default room is a private room, spectators not allowed, no special rules
};