#include "ProcessTool.h"
DWORD GetProcessID(wchar_t* exname)
{
	PROCESSENTRY32 processentry = { 0 };
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (!hsnapshot)return 0;
	processentry.dwSize = sizeof(processentry);
	if (!Process32First(hsnapshot, &processentry))return 0;
	do {
		if (!wcscmp(processentry.szExeFile, exname))
		{
			CloseHandle(hsnapshot);
			return processentry.th32ProcessID;
		}
	} while (Process32Next(hsnapshot, &processentry));
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

unsigned long getModule(DWORD pid, LPCTSTR module_name, unsigned long* size)
{
	HANDLE sanpshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);

	printf("me32 size=%d\n", pid);
	printf("me32 size=%d\n", me32.dwSize);
	printf("module name %s\n", module_name);

	int i = 0;
	while (Module32Next(sanpshot, &me32))
	{
		printf("Eny module %d name %s\n", i, me32.szModule);
		i++;
		if (strcmp(me32.szModule, module_name) == 0)
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