#pragma once
#include <cmath>
#include <iostream>
#include <string>
#include <Windows.h>

const int BAR_SIZE = 59;
const int BAR_MEMORY_SIZE = 400;
const int BAR_INTERVAL = 40;

char cGameMode = 0;
char cGlobalEXFlash = 0;
char cP1EXFlash = 0;
char cP2EXFlash = 0;
int nFrameCount = 0; //Counts slower during slowdown
int nLastFrameCount = 0;
int nTrueFrameCount = 0; //Counts all frames during slowdown
int nLastTrueFrameCount;
char cFN1Input = 0;
char cFN2Input = 0;
char cDummyState = 0;

int nBarCounter;
int nBarIntervalCounter;
int nBarIntervalMax;
bool bIsBarReset;
bool bDoBarReset;
bool bUpdateBar;
bool bDoAdvantage;

bool bIsStateSaved = false;
DWORD dwSaveState1[74576 / 4];
DWORD dwSaveState2[1632 / 4];
char cSaveState3;
DWORD dwSaveState4[52 / 4];
DWORD dwSaveState5[1004 / 4];
DWORD dwSaveState6;
DWORD dwSaveState7;
DWORD dwSaveState8;
DWORD dwSaveState9;
DWORD dwSaveState10;
DWORD dwSaveState11;

DWORD dwSaveState12[972 / 4];
DWORD dwSaveState13[972 / 4];
DWORD dwSaveState14[972 / 4];
DWORD dwSaveState15[972 / 4];

bool bLockInput = false;

void CheckGameMode(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x14EEE8), &cGameMode, 1, 0);
}

void UpdateGlobals(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
	nLastFrameCount = nFrameCount;
	nLastTrueFrameCount = nTrueFrameCount;
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x158908), &cP1EXFlash, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x158C14), &cP2EXFlash, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15D1CC), &nFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A40), &nTrueFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A48), &cGlobalEXFlash, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x37144C), &cFN1Input, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x37144D), &cFN2Input, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x34D7F8), &cDummyState, 1, 0);
}

struct Player
{
	int nPlayerNumber = 0;
	DWORD dwCharacterBaseAddress = 0x0;
	char cExists = 0;
	int nPattern = 0;
	int nState = 0;
	int nHealth = 0;
	int nRedHealth = 0;
	float fGuardGauge = 0.0f;
	float fGuardQuality = 0.0f;
	int nMagicCircuit = 0;
	int nFrameCount = 0;
	int nXPosition = 0;
	int nYPosition = 0;
	int nXSpeed = 0;
	int nYSpeed = 0;
	short sXAcceleration = 0;
	short sYAcceleration = 0;
	int nMomentum = 0;
	char cHitstop = 0;
	bool bThrowFlag = false;
	bool bTagFlag = false;
	bool bBlockstunFlag = false;
	char sGrantedStrikeInvuln = 0;
	char sGrantedThrowInvuln = 0;
	short sUntechTotal = 0;
	short sUntechCounter = 0;
	int nHitstunRemaining = 0;
	char cCounterHit = 0;
	float fGravity = 0.0f;
	short sUntechPenalty = 0;
	char cRawDirectionalInput = 0;
	char cButtonInput = 0;
	char cMacroInput = 0;
	char cIsOnRight = 0;
	DWORD dwAnimationDataPointer = 0x0;
	DWORD dwStateDataPointer = 0x0;
	DWORD dwAttackDataPointer = 0x0;
	char cStance = 0;
	char cStateInvuln = 0;
	char cNormalCancels = 0;
	char cSpecialCancels = 0;
	int nStateFlagset2 = 0;
	char cBoxIndex = 0;

	int nInactionableFrames = 0;

	std::string sBar1[BAR_MEMORY_SIZE];
	std::string sBar2[BAR_MEMORY_SIZE];
	std::string sBar3[BAR_MEMORY_SIZE];
	std::string sBarString1;
	std::string sBarString2;
	std::string sBarString3;

	int nActiveCounter = 0;
	int nAdvantageCounter = 0;
	int nLastFrameCount = 0;
	int nActiveProjectileCount = 0;
};

void CheckProjectiles(HANDLE hMBAAHandle, DWORD dwBaseAddress, Player& P)
{
	bool bProjectileExists = 0;
	char cProjectileSource = 0;
	int nProjectilePattern = 0;
	DWORD cProjectileAttackDataPointer = 0;
	int nCount = 0;
	for (int i = 0; i < 100; i++)
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x27BDE8 + 0x33C * i), &bProjectileExists, 1, 0);
		if (bProjectileExists)
		{
			ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x27BDE8 + 0x33C * i + 0x8), &cProjectileSource, 1, 0);
			if (cProjectileSource + 1 == P.nPlayerNumber)
			{
				ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x27BDE8 + 0x33C * i + 0x10), &nProjectilePattern, 4, 0);
				ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x27BDE8 + 0x33C * i + 0x324), &cProjectileAttackDataPointer, 4, 0);
				if (cProjectileAttackDataPointer != 0 && nProjectilePattern >= 100)
				{
					nCount++;
				}
			}
		}
	}
	P.nActiveProjectileCount = nCount;
}

void UpdatePlayer(HANDLE hMBAAHandle, DWORD dwBaseAddress, Player &P) {
	P.nLastFrameCount = P.nFrameCount;
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x000), &P.cExists, 1, 0);
	if (!P.cExists)
	{
		return;
	}
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x010), &P.nPattern, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x014), &P.nState, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0BC), &P.nHealth, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0C0), &P.nRedHealth, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0C4), &P.fGuardGauge, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0D8), &P.fGuardQuality, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0E0), &P.nMagicCircuit, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0F0), &P.nFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x108), &P.nXPosition, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x10C), &P.nYPosition, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x11C), &P.nXSpeed, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x120), &P.nYSpeed, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x124), &P.sXAcceleration, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x126), &P.sYAcceleration, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x138), &P.nMomentum, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x172), &P.cHitstop, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x176), &P.bThrowFlag, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x178), &P.bTagFlag, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x17B), &P.bBlockstunFlag, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x185), &P.sGrantedStrikeInvuln, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x186), &P.sGrantedThrowInvuln, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x18E), &P.sUntechTotal, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x190), &P.sUntechCounter, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x1AC), &P.nHitstunRemaining, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x1FA), &P.cCounterHit, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2E4), &P.fGravity, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2E8), &P.sUntechPenalty, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2EB), &P.cRawDirectionalInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2ED), &P.cButtonInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2EE), &P.cMacroInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x315), &P.cIsOnRight, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x320), &P.dwAnimationDataPointer, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x324), &P.dwAttackDataPointer, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimationDataPointer + 0x38), &P.dwStateDataPointer, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimationDataPointer + 0x42), &P.cBoxIndex, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + 0xC), &P.cStance, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + 0xD), &P.cStateInvuln, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + 0xE), &P.cNormalCancels, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + 0xF), &P.cSpecialCancels, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + 0x18), &P.nStateFlagset2, 4, 0);
	if (P.nPlayerNumber % 2 == 1)
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157FC0), &P.nInactionableFrames, 4, 0);
	}
	else if (P.nPlayerNumber % 2 == 0)
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x1581CC), &P.nInactionableFrames, 4, 0);
	}
	CheckProjectiles(hMBAAHandle, dwBaseAddress, P);
}

void SaveState(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x27BD70), &dwSaveState1, 74576, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x158600), &dwSaveState2, 1632, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A48), &cSaveState3, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DD8), &dwSaveState4, 52, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157E10), &dwSaveState5, 1004, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x164B14), &dwSaveState6, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x164B18), &dwSaveState7, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15DEC4), &dwSaveState8, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15DEC8), &dwSaveState9, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DB8), &dwSaveState10, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DBC), &dwSaveState11, 4, 0);

	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x155140), &dwSaveState12, 972, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x155C3C), &dwSaveState13, 972, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x156738), &dwSaveState14, 972, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157234), &dwSaveState15, 972, 0);

	bIsStateSaved = true;
}

void LoadState(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x27BD70), &dwSaveState1, 74576, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x158600), &dwSaveState2, 1632, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A48), &cSaveState3, 1, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DD8), &dwSaveState4, 52, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157E10), &dwSaveState5, 1004, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x164B14), &dwSaveState6, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x164B18), &dwSaveState7, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15DEC4), &dwSaveState8, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15DEC8), &dwSaveState9, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DB8), &dwSaveState10, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DBC), &dwSaveState11, 4, 0);

	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x155140), &dwSaveState12, 972, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x155C3C), &dwSaveState13, 972, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x156738), &dwSaveState14, 972, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157234), &dwSaveState15, 972, 0);
}

void SetStringLength(std::string &sString, int nDesiredLength, std::string sPadding = " ")
{
	int nStringLength = sString.length();
	if (nStringLength > nDesiredLength)
	{
		sString = sString.substr(nStringLength-2, nStringLength);
	}
	else if (nStringLength < nDesiredLength)
	{
		for (int i = 0; i < nDesiredLength - nStringLength; i++)
		{
			sString = sPadding + sString;
		}
	}
}

void CalculateAdvantage(Player& P1, Player& P2)
{
	if (P1.nInactionableFrames == 0 && P2.nInactionableFrames == 0)
	{
		bDoAdvantage = false;
	}
	else if (P1.nInactionableFrames != 0 && P2.nInactionableFrames != 0)
	{
		bDoAdvantage = true;
		P1.nAdvantageCounter = 0;
		P2.nAdvantageCounter = 0;
	}

	if (bDoAdvantage && nFrameCount != nLastFrameCount && cGlobalEXFlash == 0)
	{
		if (P1.nInactionableFrames == 0 && P2.nInactionableFrames != 0)
		{
			P1.nAdvantageCounter++;
		}
		else if (P2.nInactionableFrames == 0 && P1.nInactionableFrames != 0)
		{
			P2.nAdvantageCounter++;
		}
	}
}

void ResetBars(Player& P)
{
	bIsBarReset = true;
	nBarCounter = 0;
	nBarIntervalCounter = 0;
	bDoBarReset = false;
	nBarIntervalMax = BAR_SIZE;
	for (int i = 0; i < BAR_MEMORY_SIZE; i++)
	{
		P.sBar1[i] = "";
		P.sBar2[i] = "";
		P.sBar3[i] = "";
	}
}

void UpdateBars(Player& P, Player& Assist)
{
	// Foreground color -> \x1b[38;2;R;G;Bm
	// Background color -> \x1b[48;2;R;G;Bm
	if (nLastFrameCount != nFrameCount) {
		std::string sFont = "\x1b[0m";
		std::string sBarValue = "  ";
		//Bar 1 - General action information
		if (P.nInactionableFrames != 0)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;65;200;0m";
			sBarValue = std::to_string(P.nInactionableFrames);
			if (P.nPattern >= 35 && P.nPattern <= 37) //Jump Startup
			{
				sFont = "\x1b[38;2;177;177;255m\x1b[48;2;241;224;132m";
			}
			else if (P.nHitstunRemaining != 0 && P.bBlockstunFlag == 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;140;140;140m";
				if (P.cStance == 1)
				{
					if (P.sUntechCounter < P.sUntechTotal)
					{
						sBarValue = std::to_string(P.sUntechTotal - P.sUntechCounter);
					}
				}
				else
				{
					if (P.nHitstunRemaining > 1)
					sBarValue = std::to_string(P.nHitstunRemaining - 1);
				}
			}
			else if(P.bBlockstunFlag)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;180;180;180m";
			}
		}
		else
		{
			sFont = "\x1b[38;2;92;92;92m\x1b[48;2;0;0;0m";
			sBarValue = std::to_string(P.nPattern);

			if (bDoAdvantage)
			{
				sBarValue = std::to_string(P.nAdvantageCounter);
				if (P.nAdvantageCounter != 0)
				{
					sFont = "\x1b[38;2;255;255;255m\x1b[48;2;0;0;0m";
				}
			}
		}

		if (P.bThrowFlag != 0)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;110;110;110m";
			sBarValue = " t";
		}
		else if (P.cBoxIndex == 12)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;225;184;0m";
		}
		else if (P.cBoxIndex == 10)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;145;194;255m";
		}
		else if (P.cBoxIndex <= 1 || P.sGrantedStrikeInvuln != 0 || P.cStateInvuln == 3)
		{
			if (P.nInactionableFrames != 0)
			{
				sFont = "\x1b[38;2;160;160;160m\x1b[48;2;255;255;255m";
			}
			else
			{
				sFont = "\x1b[38;2;100;100;100m\x1b[48;2;255;255;255m";
			}
			
		}

		if (cGlobalEXFlash != 0 || cP1EXFlash != 0 || cP2EXFlash != 0)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;60;60m";
		}
		else if (P.cHitstop != 0)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;80;128m";
		}

		SetStringLength(sBarValue, 2);
		P.sBar1[nBarCounter % BAR_MEMORY_SIZE] = sFont + sBarValue;

		//Bar 2 - Active frames
		sFont = "\x1b[0m";
		sBarValue = "  ";

		if (P.cStance == 1)
		{
			sBarValue = " ^";
		}

		if (P.dwAttackDataPointer != 0)
		{
			sBarValue = std::to_string(P.nActiveCounter);
			if (cGlobalEXFlash != 0 || cP1EXFlash != 0 || cP2EXFlash != 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;60;60m";
			}
			else if (P.cHitstop != 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;80;128m";
			}
			else
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m";
			}

			if (P.cStance == 1)
			{
				sFont += "\x1b[4m";
			}
		}

		SetStringLength(sBarValue, 2);
		P.sBar2[nBarCounter % BAR_MEMORY_SIZE] = sFont + sBarValue;

		//Bar 3
		sFont = "\x1b[0m";
		sBarValue = "  ";

		if (P.nActiveProjectileCount != 0 || Assist.nActiveProjectileCount != 0)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m";
			sBarValue = std::to_string(P.nActiveProjectileCount + Assist.nActiveProjectileCount);
		}

		if (Assist.dwAttackDataPointer != 0)
		{
			sBarValue = std::to_string(Assist.nActiveCounter);
			if (cGlobalEXFlash != 0 || cP1EXFlash != 0 || cP2EXFlash != 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;60;60m";
			}
			else if (Assist.cHitstop != 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;80;128m";
			}
			else
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;255;128;0m";
			}

			if (Assist.cStance == 1)
			{
				sFont += "\x1b[4m";
			}
		}

		SetStringLength(sBarValue, 2);
		P.sBar3[nBarCounter % BAR_MEMORY_SIZE] = sFont + sBarValue;
	}
	else if (nBarCounter > 0)
	{
		P.sBar1[nBarCounter % BAR_MEMORY_SIZE] = P.sBar1[nBarCounter - 1];
		P.sBar2[nBarCounter % BAR_MEMORY_SIZE] = P.sBar2[nBarCounter - 1];
		P.sBar3[nBarCounter % BAR_MEMORY_SIZE] = P.sBar3[nBarCounter - 1];
	}
}

void BarHandling(Player &P1, Player &P2, Player& P1Assist, Player& P2Assist)
{
	CalculateAdvantage(P1, P2);

	bool IsInput = (
		P1.cRawDirectionalInput != 0 || P2.cRawDirectionalInput != 0 ||
		P1.cButtonInput != 0 || P2.cButtonInput ||
		P1.cMacroInput != 0 || P2.cMacroInput != 0
		);
	bool DoBarUpdate = (
		P1.nInactionableFrames != 0 || P2.nInactionableFrames ||
		P1.nStateFlagset2 != 0 || P2.nStateFlagset2 != 0 ||
		P1.nActiveProjectileCount != 0 || P2.nActiveProjectileCount ||
		P1Assist.dwAttackDataPointer != 0 || P2Assist.dwAttackDataPointer ||
		P1Assist.nActiveProjectileCount != 0 || P2Assist.nActiveProjectileCount != 0
		);

	if (DoBarUpdate)
	{
		bIsBarReset = false;
		bUpdateBar = true;
		nBarIntervalCounter = 0;
	}
	else
	{
		bUpdateBar = false;
		nBarIntervalCounter++;
	}

	if (nBarCounter >= BAR_SIZE)
	{
		nBarIntervalMax = BAR_INTERVAL;
	}

	if (nBarIntervalCounter > nBarIntervalMax)
	{
		bDoBarReset = true;
	}

	if (bDoBarReset && bUpdateBar)
	{
		ResetBars(P1);
		ResetBars(P2);
		ResetBars(P1Assist);
		ResetBars(P2Assist);
	}

	if (nBarIntervalCounter < nBarIntervalMax && !bIsBarReset)
	{
		bUpdateBar = true;
	}

	if (bUpdateBar)
	{
		if (P1.dwAttackDataPointer != 0 && P1.cHitstop == 0 && P1.nLastFrameCount != P1.nFrameCount)
		{
			P1.nActiveCounter++;
		}
		else if (P1.dwAttackDataPointer == 0)
		{
			P1.nActiveCounter = 0;
		}

		if (P2.dwAttackDataPointer != 0 && P2.cHitstop == 0 && P2.nLastFrameCount != P2.nFrameCount)
		{
			P2.nActiveCounter++;
		}
		else if (P2.dwAttackDataPointer == 0)
		{
			P2.nActiveCounter = 0;
		}

		if (P1Assist.dwAttackDataPointer != 0 && P1Assist.cHitstop == 0 && P1Assist.nLastFrameCount != P1Assist.nFrameCount)
		{
			P1Assist.nActiveCounter++;
		}
		else if (P1Assist.dwAttackDataPointer == 0)
		{
			P1Assist.nActiveCounter = 0;
		}

		if (P2Assist.dwAttackDataPointer != 0 && P2Assist.cHitstop == 0 && P2Assist.nLastFrameCount != P2Assist.nFrameCount)
		{
			P2Assist.nActiveCounter++;
		}
		else if (P2Assist.dwAttackDataPointer == 0)
		{
			P2Assist.nActiveCounter = 0;
		}

		UpdateBars(P1, P1Assist);
		UpdateBars(P2, P2Assist);
		if (P1Assist.cExists)
		{
			UpdateBars(P1Assist, P1);
		}
		if (P2Assist.cExists)
		{
			UpdateBars(P2Assist, P2);
		}
		nBarCounter++;
	}

	if (nTrueFrameCount == 0)
	{
		ResetBars(P1);
		ResetBars(P2);
		ResetBars(P1Assist);
		ResetBars(P2Assist);
	}
}

void PrintFrameDisplay(Player &P1, Player &P2)
{
	P1.sBarString1 = "";
	P1.sBarString2 = "";
	P1.sBarString3 = "";
	for (int i = (nBarCounter % BAR_MEMORY_SIZE) - BAR_SIZE; i < (nBarCounter % BAR_MEMORY_SIZE); i++)
	{
		if (i < 0)
		{
			P1.sBarString1 += P1.sBar1[i + BAR_MEMORY_SIZE];
			P1.sBarString2 += P1.sBar2[i + BAR_MEMORY_SIZE];
			P1.sBarString3 += P1.sBar3[i + BAR_MEMORY_SIZE];
		}
		else
		{
			P1.sBarString1 += P1.sBar1[i];
			P1.sBarString2 += P1.sBar2[i];
			P1.sBarString3 += P1.sBar3[i];
		}
	}

	P2.sBarString1 = "";
	P2.sBarString2 = "";
	P2.sBarString3 = "";
	for (int i = (nBarCounter % BAR_MEMORY_SIZE) - BAR_SIZE; i < (nBarCounter % BAR_MEMORY_SIZE); i++)
	{
		if (i < 0)
		{
			P2.sBarString1 += P2.sBar1[i + BAR_MEMORY_SIZE];
			P2.sBarString2 += P2.sBar2[i + BAR_MEMORY_SIZE];
			P2.sBarString3 += P2.sBar3[i + BAR_MEMORY_SIZE];
		}
		else
		{
			P2.sBarString1 += P2.sBar1[i];
			P2.sBarString2 += P2.sBar2[i];
			P2.sBarString3 += P2.sBar3[i];
		}
	}

	int nP1XPixelPosition = floor(P1.nXPosition / 128.0);
	int nP1YPixelPosition = floor(P1.nYPosition / 128.0);
	int nP2XPixelPosition = floor(P2.nXPosition / 128.0);
	int nP2YPixelPosition = floor(P2.nYPosition / 128.0);
	printf("\x1b[0m(%6i, %6i)\x1b[7m(%4i, %4i)\x1b[0mpat %3i [%2i]\x1b[7mx-spd %5i\x1b[0mx-acc %5i\x1b[7my-spd %5i\x1b[0my-acc %5i\x1b[7mhp %5i\x1b[0mmc %5i\x1b[0m\x1b[K\n",
		P1.nXPosition, P1.nYPosition, nP1XPixelPosition, nP1YPixelPosition, P1.nPattern, P1.nState,
		P1.nXSpeed, P1.sXAcceleration, P1.nYSpeed, P1.sYAcceleration, P1.nHealth, P1.nMagicCircuit);
	printf("\x1b[0m(%6i, %6i)\x1b[7m(%4i, %4i)\x1b[0mpat %3i [%2i]\x1b[7mx-spd %5i\x1b[0mx-acc %5i\x1b[7my-spd %5i\x1b[0my-acc %5i\x1b[7mhp %5i\x1b[0mmc %5i\x1b[0m\x1b[K\n",
		P2.nXPosition, P2.nYPosition, nP2XPixelPosition, nP2YPixelPosition, P2.nPattern, P2.nState,
		P2.nXSpeed, P2.sXAcceleration, P2.nYSpeed, P2.sYAcceleration, P2.nHealth, P2.nMagicCircuit);
	int nXDistance = abs(P1.nXPosition - P2.nXPosition);
	int nYDistance = abs(P1.nYPosition - P2.nYPosition);
	int nXPixelDistance = abs(floor(P1.nXPosition / 128.0) - floor(P2.nXPosition / 128.0));
	int nYPixelDistance = abs(floor(P1.nYPosition / 128.0) - floor(P2.nYPosition / 128.0));
	printf("\x1b[0m(%6i, %6i)\x1b[7m(%4i, %4i)\x1b[0madv %3i\x1b[K\n", nXDistance, nYDistance, nXPixelDistance, nYPixelDistance, P1.nAdvantageCounter % 100);
	printf("\x1b[4m 1 2 3 4 5 6 7 8 9\x1b[7m10\x1b[0m"
		"\x1b[4m 1 2 3 4 5 6 7 8 9\x1b[7m20\x1b[0m"
		"\x1b[4m 1 2 3 4 5 6 7 8 9\x1b[7m30\x1b[0m"
		"\x1b[4m 1 2 3 4 5 6 7 8 9\x1b[7m40\x1b[0m"
		"\x1b[4m 1 2 3 4 5 6 7 8 9\x1b[7m50\x1b[0m"
		"\x1b[4m 1 2 3 4 5 6 7 8 9\x1b[0m\x1b[0m\x1b[K\n");
	std::cout << P1.sBarString1 << "\x1b[0m\x1b[K" << std::endl;
	std::cout << P1.sBarString2 << "\x1b[0m\x1b[K" << std::endl;
	std::cout << P1.sBarString3 << "\x1b[0m\x1b[K" << std::endl;
	std::cout << P2.sBarString1 << "\x1b[0m\x1b[K" << std::endl;
	std::cout << P2.sBarString2 << "\x1b[0m\x1b[K" << std::endl;
	std::cout << P2.sBarString3 << "\x1b[0m\x1b[K" << std::endl;
}

void FrameDisplay(HANDLE hMBAAHandle, DWORD dwBaseAddress, Player& P1, Player& P2, Player& P3, Player& P4)
{
	CheckGameMode(hMBAAHandle, dwBaseAddress);

	if (cGameMode != 1)
	{
		return;
	}

	if (cFN1Input > 0)
	{
		if (P1.cRawDirectionalInput == 8)
		{
			bIsStateSaved = false;
		}
		else if (!bLockInput)
		{
			bLockInput = true;
			SaveState(hMBAAHandle, dwBaseAddress);
			char cFF = 0xFF;
			WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A48), &cFF, 1, 0);
		}
	}
	else if (cFN2Input > 0)
	{
		if (!bLockInput)
		{
			bLockInput = true;
			if (cDummyState == 5 || cDummyState == -1)
			{
				char cFF = 0xFF;
				WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15DEC3), &cFF, 1, 0);
			}
		}
	}
	else if (bLockInput)
	{
		bLockInput = false;
		char c00 = 0x00;
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A48), &c00, 1, 0);
	}

	UpdateGlobals(hMBAAHandle, dwBaseAddress);
	Player* Player1 = &P1;
	Player* Player2 = &P2;
	Player* Player3 = &P3;
	Player* Player4 = &P4;
	if (P1.bTagFlag)
	{
		Player1 = &P3;
		Player3 = &P1;
	}
	if (P2.bTagFlag)
	{
		Player2 = &P4;
		Player4 = &P2;
	}

	if (nLastTrueFrameCount != nTrueFrameCount)
	{
		UpdatePlayer(hMBAAHandle, dwBaseAddress, P1);
		UpdatePlayer(hMBAAHandle, dwBaseAddress, P2);
		UpdatePlayer(hMBAAHandle, dwBaseAddress, P3);
		UpdatePlayer(hMBAAHandle, dwBaseAddress, P4);
		
		BarHandling(*Player1, *Player2, *Player3, *Player4);

		if (nTrueFrameCount == 1 && bIsStateSaved)
		{
			LoadState(hMBAAHandle, dwBaseAddress);
		}
	}

	PrintFrameDisplay(*Player1, *Player2);
}