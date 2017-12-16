#pragma once
#include "stdafx.h"
#include "ProcessMemoryWriter.h"

	char *MemoryWriter::stringToChar(std::string str)
	{
		char *character = new char[str.length() + 1];
		std::strcpy(character, str.c_str());
		return character;
	}


	HWND window;
	DWORD pid;
	HANDLE windowHandle;


	int MemoryWriter::setupWindowHandles(char* processName)
	{
		window = FindWindow(0, processName);
		GetWindowThreadProcessId(window, &pid);
		windowHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
		//GetWindowThreadProcessId(window, &pid); //Get the process id and place it in pid
		//windowHandle = OpenProcess(PROCESS_VM_READ, 0, pid);
		return 1;
	}

	int MemoryWriter::closeHandles()
	{
		CloseHandle(windowHandle);
		window = NULL;
		return 1;
	}

	DWORD MemoryWriter::getProcessBaseAddress(char* processName)
	{
		const char* processName_ = processName;
		DWORD  processID_ = NULL;
		DWORD  processBaseAddress_ = 0;
		
			PROCESSENTRY32 processEntry_; // Entry into process you wish to inject to
			HANDLE hProcSnapshot_ = NULL;
			/* Takes a snapshot of the system's processes */
			hProcSnapshot_ = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //?

																			  /* While process has not been found, keep looking for it */
			while (!processID_)
			{
				/* If a process on the system exists */
				if (Process32First(hProcSnapshot_, &processEntry_)) //?
				{
					/* Check all processes in the system's processes snapshot */
					do
					{
						/* Compare the name of the process to the one we want */
						if (!strcmp(processEntry_.szExeFile, processName_)) //?
						{
							/* Save the processID and break out */
							processID_ = processEntry_.th32ProcessID;
							break;
						}
					} while (Process32Next(hProcSnapshot_, &processEntry_));
				}

				/* Didnt find process, sleep for a bit */
				if (!processID_)
				{
					system("CLS");
					std::cout << "Make sure " << processName_ << " is running." << std::endl;
					Sleep(200);
				}
			}

			/* Process found */
			std::cout << "Found Process: " << processName_ << std::endl;
		


		/* Find Base Address of process */
		HANDLE moduleSnapshotHandle_ = INVALID_HANDLE_VALUE;
		MODULEENTRY32 moduleEntry_;

		/* Take snapshot of all the modules in the process */
		moduleSnapshotHandle_ = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID_);

		/* Snapshot failed */
		if (moduleSnapshotHandle_ == INVALID_HANDLE_VALUE)
		{
			std::cout << "Module Snapshot error" << std::endl;
			MessageBoxA(0, "Module snapshot error", "", 0);
			return 0;
		}

		/* Size the structure before usage */
		moduleEntry_.dwSize = sizeof(MODULEENTRY32);

		/* Retrieve information about the first module */
		if (!Module32First(moduleSnapshotHandle_, &moduleEntry_))
		{
			std::cout << "First module not found" << std::endl;
			MessageBoxA(0, "Module not found", "", 0);
			CloseHandle(moduleSnapshotHandle_);
			return 0;
		}

		/* Find base address */
		while (!processBaseAddress_)
		{
			/* Find module of the executable */
			do
			{

				/* Compare the name of the process to the one we want */
				if (!strcmp(moduleEntry_.szModule, processName_)) //?
				{
					/* Save the processID and break out */
					processBaseAddress_ = (unsigned int)moduleEntry_.modBaseAddr;
					break;
				}

			} while (Module32Next(moduleSnapshotHandle_, &moduleEntry_));


			if (!processBaseAddress_)
			{
				system("CLS");
				std::cout << "Failed to find module" << processName_ << std::endl;
				MessageBoxA(0, "Module !_!_!", "", 0);
				Sleep(200);
			}
		}

		/* Found module and base address successfully */
		std::cout << "Base Address: " << std::hex << processBaseAddress_ << std::dec << std::endl;
		CloseHandle(moduleSnapshotHandle_);
		return processBaseAddress_;
	}


	DWORD MemoryWriter::FindDmaAddy(int PointerLevel, DWORD Offsets[], DWORD BaseAddress)
	{
		//DECLARE BASE ADDRESS
		DWORD pointer = BaseAddress;             // Declare a pointer of DWORD
												 //USED TO output the contents in the pointer
		DWORD pTemp;

		DWORD pointerAddr;
		for (int i = 0; i < PointerLevel; i++)
		{
			if (i == 0)
			{
				ReadProcessMemory(windowHandle, (LPCVOID)pointer, &pTemp, 4, NULL);
			}
			//add first offset to that address
			pointerAddr = pTemp + Offsets[i];   // Set p1 to content of p + offset

												//Read memory one more time and exit the loop
			ReadProcessMemory(windowHandle, (LPCVOID)pointerAddr, &pTemp, 4, NULL);
		}
		return pointerAddr;
	}


	void MemoryWriter::writeMemory(DWORD address, char* value, int bytes)
	{
		unsigned long Oldprotection;
		VirtualProtect((LPVOID)address, bytes, PAGE_EXECUTE_READWRITE, &Oldprotection);
		memcpy((LPVOID)address, value, bytes);
		VirtualProtect((LPVOID)address, bytes, Oldprotection, NULL);
	}


	void MemoryWriter::writeMemory(DWORD address, std::string value, int bytes)
	{
		unsigned long Oldprotection;
		VirtualProtect((LPVOID)address, bytes, PAGE_EXECUTE_READWRITE, &Oldprotection);
		memcpy((LPVOID)address, stringToChar(value), bytes);
		VirtualProtect((LPVOID)address, bytes, Oldprotection, NULL);
	}




	HANDLE MemoryWriter::getHandle()
	{
		return windowHandle;
	}

	HWND MemoryWriter::getWindow()
	{
		return window;
	}

	int MemoryWriter::writeProcessMemory(DWORD address, int value)
	{
		return WriteProcessMemory(windowHandle, (void*)address, &value, sizeof(value), 0);
	}

	int MemoryWriter::writeProcessMemory(DWORD address, float value)
	{
		return WriteProcessMemory(windowHandle, (void*)address, &value, sizeof(value), 0);
	}

	int MemoryWriter::readProcessMemory(DWORD address, int *value)
	{
		return ReadProcessMemory(windowHandle, (void*)address, value, sizeof(value), 0);
	}

	int MemoryWriter::readProcessMemory(DWORD address, float *value)
	{
		return ReadProcessMemory(windowHandle, (void*)address, value, sizeof(value), 0);
	}
	int MemoryWriter::readProcessMemory(DWORD address, float **value)
	{
		return ReadProcessMemory(windowHandle, (void*)address, value, sizeof(value), 0);
	}
	int MemoryWriter::readProcessMemory(DWORD address, DWORD* value)
	{
		return ReadProcessMemory(windowHandle, (void*)address, value, sizeof(value), NULL);
	}
	int MemoryWriter::readViewMatrix(float* value[])
	{
		return ReadProcessMemory(windowHandle, (void*)0x501AE8, value, sizeof(value), NULL);
	}