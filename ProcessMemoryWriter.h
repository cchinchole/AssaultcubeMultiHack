#pragma once
#include "stdafx.h"
#include <Windows.h>
#include <conio.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <iostream>


class MemoryWriter {

public:
	HANDLE getHandle();
	HWND getWindow();
	int writeProcessMemory(DWORD address, int value);
	int writeProcessMemory(DWORD address, float value);
	int readProcessMemory(DWORD address, int *value);
	int readProcessMemory(DWORD address, float *value);
	int readProcessMemory(DWORD address, float **value);
	int readProcessMemory(DWORD address, DWORD* value);
	int readViewMatrix(float* value[]);
	int setupWindowHandles(char* processName);
	DWORD getProcessBaseAddress(char* processName);
	DWORD FindDmaAddy(int PointerLevel, DWORD Offsets[], DWORD BaseAddress);
	void writeMemory(DWORD address, char* value, int bytes);
	void writeMemory(DWORD address, std::string value, int bytes);
	int closeHandles();

private:
	char * stringToChar(std::string str);
};