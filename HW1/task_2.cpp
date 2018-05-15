#pragma once
#include "task_2.h"

void RandomAllocatingMemory(void) {
	const DWORD ArraySize = 300;               // Количество фрагментированных блоков
	LPVOID lpvBase[ArraySize];               // Адреса памяти
	DWORD dwPageSize;               // Размер страницы на этом компе


	SYSTEM_INFO sSysInfo;       
	GetSystemInfo(&sSysInfo);  

	_tprintf(TEXT("Page size in this computer %d Kb.\n"), sSysInfo.dwPageSize / 1024);

	dwPageSize = sSysInfo.dwPageSize;
	srand(time(NULL));
	for (DWORD i = 0; i < ArraySize; i++) {
		LPVOID address = LPVOID(NULL + rand() * rand() * rand());
		SIZE_T size = rand();
		DWORD flag;
		if (rand() % 2 == 0) {
			flag = PAGE_GUARD;
		}
		else
		{
			flag = PAGE_NOACCESS;
		}
		lpvBase[i] = VirtualAlloc(
			address,                 
			size * dwPageSize, 
			MEM_RESERVE | MEM_COMMIT,          
			flag);     
		if (lpvBase[i] == NULL) {
			continue;
		}
		else {
			_tprintf(TEXT("Blog allocated. \n"));
		}
	}
}