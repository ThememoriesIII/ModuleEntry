#pragma once
#include "ProcessTool.h"
#include <vector>
#include <iostream>
/*
->PatternScanExModule �᡹������ŷ���շ����������� process id ��� handle �ͧ���
-->PatternScanEx �᡹����ŷ���������������ҹ address ���º
--->PatternScan ��º������� adress ��ҵç�Ѻ Pattern �����
*/

//scan pattern in address
void* PatternScan(char* base, size_t size, char* pattern, char* mask);
//scan pattern in module
void* PatternScanEx(HANDLE hprocess,uintptr_t begin,uintptr_t end, char* pattern, char* mask);
// initilial begin and endaddress for a scan pattern module
void* PatternScanExModule(HANDLE hprocess, wchar_t* exename, wchar_t* module, char* pattern, char* mask);

// List Pattern module in game
std::vector<uintptr_t> ListPatternEx(HANDLE hprocess, uintptr_t begin, uintptr_t end, char* pattern, char* mask);
// initilial begin and endaddress for a List module name under game process 
std::vector<uintptr_t> PatternListExModule(HANDLE hprocess, wchar_t* exename, wchar_t* module, char* pattern, char* mask);