#include "ProcessTool.h"
wchar_t* GetWC(const char* c)
{
const size_t cSize = strlen(c) + 1;
wchar_t* wc = new wchar_t[cSize];
size_t outSize;
mbstowcs_s(&outSize,wc, cSize,c, cSize-1);
return wc;
}
DWORD GetProcessID(wchar_t* exname)
{
	PROCESSENTRY32 processentry = { 0 };
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	//HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!hsnapshot)return 0;
	processentry.dwSize = sizeof(processentry);
	if (!Process32First(hsnapshot, &processentry))return 0;
	do {
		if (!wcscmp(processentry.szExeFile, exname))
		{
			CloseHandle(hsnapshot);
			//SetConsoleTextAttribute(console, 2);
			std::wcout << "FOUND " << exname << std::endl;
			//SetConsoleTextAttribute(console, 7);
			//CloseHandle(console);
			return processentry.th32ProcessID;
		}
	} while (Process32Next(hsnapshot, &processentry));
	std::wcout << "NOT FOUND " << exname << std::endl;
	CloseHandle(hsnapshot);
	return 0;
}
MODULEENTRY32 GetModule(DWORD procid, wchar_t* modulename)
{
	MODULEENTRY32 modEntry = { 0 };
	HANDLE hsanpshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procid);
	if (hsanpshot != INVALID_HANDLE_VALUE)
	{
		modEntry.dwSize = sizeof(MODULEENTRY32);
		do
		{
			if (!wcscmp(modEntry.szModule,modulename))
			{
				break;
			}
		}while (Module32Next(hsanpshot, &modEntry));
		CloseHandle(hsanpshot);
	}
	return modEntry;
}

unsigned long getModule(DWORD pid,wchar_t* module_name, unsigned long* size)
{
	HANDLE sanpshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);

	/*printf("me32 size=%d\n", pid);
	printf("me32 size=%d\n", me32.dwSize);
	printf("module name %s\n", module_name);*/

	/*std::cout <<"process id in module get "<<std::uppercase<<std::hex<< pid << std::endl;
	std::cout <<"Module Entry Size "<< me32.dwSize << std::endl;
	std::wcout<<"Module Name "<< module_name << std::endl;*/

	int i = 0;
	while (Module32Next(sanpshot, &me32))
	{
		//printf("Eny module %d name %s\n", i, me32.szModule);
		//std::wcout << "Module" << i << "Name = " << me32.szModule << std::endl;
		i++;
		if (wcscmp(me32.szModule, module_name) == 0)
		{
			if (size != 0)
			{
				*size = me32.modBaseSize;
			}
			//printf("me32.modBaseSize=%ul\n", (unsigned long)me32.modBaseAddr);
			printf("get\n");
			return (unsigned long)me32.modBaseAddr;
		}
	}
	printf("not get\n");
	return NULL;
}
std::vector<dllList> ListModule(DWORD proid, unsigned long* size)
{
	std::vector<dllList> mhod;
	HANDLE sanpshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, proid);
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	while (Module32Next(sanpshot, &me32))
	{
		std::wcout << "Module Name = " << me32.szModule << std::endl;
		dllList dump = {const_cast<wchar_t*>(me32.szModule),(uintptr_t)me32.modBaseAddr};
		mhod.push_back(dump);
	}
	return mhod;
}