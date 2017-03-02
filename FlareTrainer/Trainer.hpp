#pragma once

#include<Windows.h>
#include<ctime>

class Trainer
{

public:

	/*
	 * run will try to enter a loop that periodically overwrites 
	 * the player's health in Flare
	 * 
	 * If it can't access Flare for whatever reason, it will return
	 * an error code
	 */
	int run();

	/*
	 * This will set the override value for the players health
	 */
	void setHealthValue(DWORD value);	

	/*
	 * this will determine how often the trainer overwrites the players
	 * health
	 * 
	 * Letting the trainer update every single loop iteration would
	 * probably be a bad idea
	 */
	void setUpdateFrequency(int delta);

private:
	
	/*
	 * This will overwrite the players health in Flare
	 */
	void overwriteHealth();
	
	/**
	 * This will try to determine the details about attaching to 
	 * the Flare process through the Windows API. This involves
	 * getting the windows, process id, and proces handle.
	 * 
	 * If it is unable to do so, it will return false
	 */
	bool attachToFlareProcess();

	/**
	 * This will determine the address a pointer is storing and store that
	 * address plus the given offset into receiver
	 */
	void pointerLookahead(DWORD & address, DWORD offset);

	/*
	 *	 This will determine the location
	 *	 of the player health data in
	 *	 process memory	 
	 */
	DWORD getHealthTargetAddress();

	/*
	 * This is to hold a handle to the Flare process
	 */
	HANDLE m_flareProcess = nullptr;

	/*
	 * This is to store the process id of the Flare process
	 */
	DWORD m_flareProcessId = -1;

	/*
	 * This is to hold a handle to the Flare window
	 */
	HWND m_flareWindow = nullptr;

	/*
	 * This is the address of a pointer who is statically addressed
	 * Since it is not allocated on the heap and its address never changes
	 * we can be sure it will always be there when we run the game
	 */
	static const DWORD m_baseAddress = { 0x00594154 };

	/*
	 * This is the value we use to override health
	 * Instead of storing it in something like an int or DWORD we have it as bytes
	 * since the Windows API will expect bytes when overwrite memory
	 */
	BYTE m_healthValue[4] = { 0 };

	/*
	 * This is the name of the window that opens when we run Flare
	 */
	LPCWSTR m_LFlareWindowName = L"Flare";

	/*
	 * This holds the last time the trainer ran
	 */
	int m_lastUpdateTime = clock();

	/*
	 * This holds the update frequency for how often the trainer
	 * updates the players health
	 */
	int m_updateFrequency = 0;

	unsigned char byte = 0b11010000;
};
