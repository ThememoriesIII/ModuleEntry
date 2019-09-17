#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
DWORD GetProcessID(wchar_t* exname);
MODULEENTRY32 GetModule(DWORD procid,wchar_t* modulename);
unsigned long GetModule(DWORD pid, LPCTSTR module_name, unsigned long* size);