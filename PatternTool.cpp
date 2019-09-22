#include "PatternTool.h"

void* PatternScan(char* base, size_t size, char* pattern, char* mask)
{
	size_t patternlength = strlen(pattern);
	for(unsigned int i=0;i < (size - patternlength);i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternlength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(base + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return (void*)(base + i);
		}
	}
	return nullptr;
	void* x = nullptr;
}
void* PatternScanEx(HANDLE hprocess, uintptr_t begin, uintptr_t end, char* pattern, char* mask)
{
	uintptr_t currentChuck = begin;
	SIZE_T byteRead;
	while (currentChuck < end)
	{
		char buffer[4096];
		//DWORD oldproc;
		//VirtualProtectEx(hprocess, (void*)currentChuck, sizeof(buffer), PROCESS_VM_READ, &oldproc);
		ReadProcessMemory(hprocess,(void*)currentChuck,buffer,sizeof(buffer),&byteRead);
		//VirtualProtectEx(hprocess, (void*)currentChuck, sizeof(buffer), oldproc,(void*));
		if (byteRead == 0)
		{
			return nullptr;
		}
		void* internalAddress = PatternScan((char*)&buffer,byteRead,pattern,mask);
		if (internalAddress != nullptr)
		{
			uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - (uintptr_t)& buffer;
			return (void*)(currentChuck + offsetFromBuffer);
		}
		else
		{
			currentChuck = currentChuck + byteRead;
		}
	}
	return nullptr;
}
void* PatternScanExModule(HANDLE hprocess,wchar_t* exename, wchar_t* module, char* pattern, char* mask)
{
	DWORD processid = GetProcessID(exename);
	MODULEENTRY32 modentry = GetModule(processid, module);
	uintptr_t begin = (uintptr_t)modentry.modBaseAddr;
	uintptr_t end = begin + modentry.modBaseSize;
	return PatternScanEx(hprocess, begin, end, pattern, mask);
	//return nullptr;
}
std::vector<uintptr_t> ListPatternEx(HANDLE hprocess, uintptr_t begin, uintptr_t end, char* pattern, char* mask)
{
	uintptr_t currentChuck = begin;
	SIZE_T byteRead;
	std::vector<uintptr_t> list_address;
	//uintptr_t **list;
	while (currentChuck < end)
	{
		char buffer[4096];
		//DWORD oldproc;
		//VirtualProtectEx(hprocess, (void*)currentChuck, sizeof(buffer), PROCESS_VM_READ, &oldproc);
		ReadProcessMemory(hprocess, (void*)currentChuck, buffer, sizeof(buffer), &byteRead);
		//VirtualProtectEx(hprocess, (void*)currentChuck, sizeof(buffer), oldproc,(void*));
		std::cout << std::uppercase << std::hex << begin << " " << std::uppercase << std::hex << end << " check chuck " << std::uppercase << std::hex << currentChuck << std::endl;
		if (byteRead == 0)
		{
			return {};
		}
		void* internalAddress = PatternScan((char*)& buffer, byteRead, pattern, mask);
		if (internalAddress != nullptr)
		{
			uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - (uintptr_t)& buffer;
			list_address.push_back(currentChuck + offsetFromBuffer);
			std::cout << "found in chuck" << std::uppercase << std::hex << currentChuck << "and add to list";
			currentChuck = currentChuck + byteRead;
			std::cout << "add byte" << byteRead << "=" << std::uppercase << std::hex << currentChuck << std::endl;

		}
		else
		{
			currentChuck = currentChuck + byteRead;
		}
	}
	return list_address;
}
std::vector<uintptr_t> PatternListExModule(HANDLE hprocess, wchar_t* exename, wchar_t* module, char* pattern, char* mask)
{
	DWORD processid = GetProcessID(exename);
	MODULEENTRY32 modentry = GetModule(processid, module);
	uintptr_t begin = (uintptr_t)modentry.modBaseAddr;
	uintptr_t end = begin + modentry.modBaseSize;
	return ListPatternEx(hprocess, begin, end, pattern, mask);
	//return nullptr;
}