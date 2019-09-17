#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include "ProcessTool.h"

void* PatternScan(char* base, size_t size, char* pattern, char* mask);
void* PatternScanEx(HANDLE hprocess,uintptr_t begin,uintptr_t end, char* pattern, char* mask);
void* PatternScanExModule(HANDLE hprocess, wchar_t exename,wchar_t* module,char* pattern,char* mask);