#pragma once
#include "Registers.h"
#include "Types.h"

void Math_Add_SS_HL(word value, bool addCarry)
{
	int total = HLasWord();
	if (GetFlag(FLAG_C) && addCarry)value++;
	SetFlag(((total & 0x0fff) + (value & 0x0fff)) & 0x1000, FLAG_H);
	total += value;
	SetFlag(total & 0x10000, FLAG_C);
	if (addCarry)
	{
		int partASign = HLasWord() & 0x8000;
		int partBSign = value & 0x8000;
		int resultSign = total & 0x8000;
		SetFlag(partASign == partBSign && resultSign != partASign, FLAG_P);
		SetFlag((total & 0x8000) != 0, FLAG_S);
		SetFlag(total == 0, FLAG_Z);
	}
	_RH_A = (total >> (8 * 1)) & 0xff;
	_RL_A = (total >> (8 * 0)) & 0xff;
	SetFlag(0, FLAG_N);
}

void Math_Add_PP_IX(word value, bool addCarry)
{
	int total =_RIX;
	if (GetFlag(FLAG_C) && addCarry)value++;
	SetFlag(((total & 0x0fff) + (value & 0x0fff)) & 0x1000, FLAG_H);
	total += value;
	SetFlag(total & 0x10000, FLAG_C);
	if (addCarry)
	{
		int partASign = _RIX & 0x8000;
		int partBSign = value & 0x8000;
		int resultSign = total & 0x8000;
		SetFlag(partASign == partBSign && resultSign != partASign, FLAG_P);
		SetFlag((total & 0x8000) != 0, FLAG_S);
		SetFlag(total == 0, FLAG_Z);
	}
	_RIX = total;
	SetFlag(0, FLAG_N);
}


void Math_Sub_SS_HL(word value, bool addCarry)
{
	word total = HLasWord();
	if (GetFlag(FLAG_C) && addCarry)value++;
	SetFlag(((total & 0x0fff) - (value & 0x0fff)) & 0x1000, FLAG_H);
	total -= value;
	SetFlag(total & 0x10000, FLAG_C);
	int partASign = HLasWord() & 0x8000;
	int partBSign = value & 0x8000;
	int resultSign = total & 0x8000;
	SetFlag(partASign != partBSign && resultSign != partASign, FLAG_P);
	SetFlag((total & 0x8000) != 0, FLAG_S);
	SetFlag(total == 0, FLAG_Z);
	SetFlag(1, FLAG_N);
	_RH_A = (total >> (8 * 1)) & 0xff;
	_RL_A = (total >> (8 * 0)) & 0xff;
}

void Math_Add_A_R(byte value, bool addCarry)
{
	byte result = value + _RA_A;
	SetFlag((((value & 0x0F) + (_RA_A & 0x0F)) & 0x10) != 0, FLAG_H);
	if (addCarry)result += GetFlag(FLAG_C);
	SetFlag(0, FLAG_N);
	SetFlag(((result & 0x80) != 0), FLAG_S);
	SetFlag(((result & 0x100) != 0), FLAG_C);
	SetFlag(((result & 0xff) == 0), FLAG_Z);
	int partASign = _RA_A & 0x80;
	int partBSign = value & 0x80;
	int resultSign = result & 0x80;
	SetFlag(partASign == partBSign && resultSign != partASign, FLAG_P);
	_RA_A = result;
}
void Math_Sub_A_R(byte value, bool addCarry)
{
	byte result = _RA_A - value;
	SetFlag((((value & 0x0F) - (_RA_A & 0x0F)) & 0x10) != 0, FLAG_H);
	if (addCarry)result -= GetFlag(FLAG_C);
	SetFlag(1, FLAG_N);
	SetFlag(((result & 0x80) != 0), FLAG_S);
	SetFlag(((result & 0x100) != 0), FLAG_C);
	SetFlag(((result & 0xff) == 0), FLAG_Z);
	int partASign = _RA_A & 0x80;
	int partBSign = value & 0x80;
	int resultSign = result & 0x80;
	SetFlag(partASign != partBSign && resultSign != partASign, FLAG_P);
	_RA_A = result;
}

void Math_And_A_R(byte value)
{
	_RA_A &= value;
	SetFlag((_RA_A & 0x80) != 0, FLAG_S);
	SetFlag((_RA_A == 0), FLAG_Z);
	SetFlag(1, FLAG_H);
	SetFlag(0, FLAG_N);
	SetFlag(0, FLAG_C);
	SetFlag(_ParityLookupTable[_RA_A], FLAG_P);
}

void Math_Or_A_R(byte value)
{
	_RA_A |= value;
	SetFlag((_RA_A & 0x80) != 0, FLAG_S);
	SetFlag((_RA_A == 0), FLAG_Z);
	SetFlag(0, FLAG_H);
	SetFlag(0, FLAG_N);
	SetFlag(0, FLAG_C);
	SetFlag(_ParityLookupTable[_RA_A], FLAG_P);
}

void Math_Xor_A_R(byte value)
{
	_RA_A ^= value;
	SetFlag((_RA_A & 0x80) != 0, FLAG_S);
	SetFlag((_RA_A == 0), FLAG_Z);
	SetFlag(0, FLAG_H);
	SetFlag(0, FLAG_N);
	SetFlag(0, FLAG_C);
	SetFlag(_ParityLookupTable[_RA_A], FLAG_P);
}

void Math_Cp_A_R(byte value)
{
	word result = _RA_A - value;
	SetFlag((((_RA_A & 0x0F) - (value & 0x0F)) & 0x10) != 0, FLAG_H);
	SetFlag(1, FLAG_N);
	SetFlag(((result & 0x80) != 0), FLAG_S);
	SetFlag(((result & 0x100) != 0), FLAG_C);
	SetFlag(((result & 0xff) == 0), FLAG_Z);
	int partASign = _RA_A & 0x80;
	int partBSign = value & 0x80;
	int resultSign = result & 0x80;
	SetFlag(partASign != partBSign && resultSign != partASign, FLAG_P);
}

byte Math_Inc_Byte(byte value)
{
	SetFlag(0, FLAG_N);
	SetFlag(!(value & 0x80) && ((value + 1) & 0x80), FLAG_P);
	value++;
	SetFlag(!(value & 0x0F), FLAG_H);
	SetFlag(value == 0, FLAG_Z);
	SetFlag(((value & 0x80) != 0), FLAG_S);
	return value;
}

byte Math_Dec_Byte(byte value)
{
	SetFlag(1, FLAG_N);
	SetFlag((value & 0x80) && !((value + 1) & 0x80), FLAG_P);
	value++;
	SetFlag((value & 0x0F) == 0x0F, FLAG_H);
	SetFlag(value == 0, FLAG_Z);
	SetFlag(((value & 0x80) != 0), FLAG_S);
	return value;
}


