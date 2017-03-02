#include"Trainer.hpp"
#include<stdexcept>
#include<exception>
#include<iostream>
#include<string>

void assertWindow(HWND win)
{
	if(win == NULL)
	{
		std::string error = "assertWindow failed";
		std::cout << error << std::endl;
		throw std::runtime_error(error);
	}
}

void assertPID(DWORD pid)
{
	if(!pid || pid == -1)
	{
		std::string error = "assertPID failed";
		std::cout << error << std::endl;
		throw std::runtime_error(error);
	}
}

void assertHandle(HANDLE handle)
{
	if(handle == NULL || handle == INVALID_HANDLE_VALUE)
	{
		std::string error = "assertHandle failed";
		std::cout << error << std::endl;
		throw std::runtime_error(error);
	}
}

int Trainer::run()
{
	int status = 0;

	if(attachToFlareProcess())
	{
		// When the user htis escape the trainer will close
		while(!GetAsyncKeyState(VK_ESCAPE))
		{	
			if(clock() - m_lastUpdateTime > m_updateFrequency && m_updateFrequency > 0)
			{	
				m_lastUpdateTime = clock();
				overwriteHealth();
			}
		}
	}
	else
	{
		status = 1;
	}

	return status;
}

void Trainer::setHealthValue(DWORD value)
{
	m_healthValue[3] = (value >> 24) & 0xff;
	m_healthValue[2] = (value >> 16) & 0xff;
	m_healthValue[1] = (value >> 8) & 0xff;
	m_healthValue[0] = value & 0xff;
}

void Trainer::setUpdateFrequency(int delta)
{
	m_updateFrequency = delta;
}


void Trainer::overwriteHealth()
{	
	DWORD healthAddress = getHealthTargetAddress();
	WriteProcessMemory(m_flareProcess, (BYTE*)healthAddress, &m_healthValue, sizeof(DWORD), NULL);
}

bool Trainer::attachToFlareProcess()
{
	
	bool success = true;
	try
	{
		m_flareWindow = FindWindow(NULL, m_LFlareWindowName);		
		assertWindow(m_flareWindow);

		GetWindowThreadProcessId(m_flareWindow, &m_flareProcessId);
		assertPID(m_flareProcessId);

		m_flareProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_flareProcessId);	
		assertHandle(m_flareProcess);

		std::cout << "Attach success" << std::endl;
	}
	catch(std::exception e)
	{
		std::cout << "Failure" << std::endl;
		success = false;
	}

	return success;
}

void Trainer::pointerLookahead(DWORD& address, DWORD offset)
{
	ReadProcessMemory(m_flareProcess, (LPCVOID)address, &address, sizeof(DWORD), NULL);
	address += offset;
}


DWORD Trainer::getHealthTargetAddress()
{
	DWORD address = m_baseAddress;

	/**
	 * Here we'll use the offsets that were found using CheatEngine
	 */

	pointerLookahead(address, 0x4);
	pointerLookahead(address, 0x18);
	pointerLookahead(address, 0x00000134);
	pointerLookahead(address, 0x28);
	pointerLookahead(address, 0x00000334);

	return address;
}

