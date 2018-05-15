#pragma once

#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <ctime>

void random_allocating_memory(void) {
	const DWORD blocks_size = 300;               // Количество фрагментированных блоков
	LPVOID lpvBase[blocks_size];               // Адреса памяти
	DWORD dwPageSize;               // Размер страницы на этом компе


	SYSTEM_INFO info;
	GetSystemInfo(&info);

	_tprintf(TEXT("Page size in this computer %d Kb.\n"), info.dwPageSize / 1024);

	dwPageSize = info.dwPageSize;
	srand(time(NULL));
	for (DWORD i = 0; i < blocks_size; i++) {
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
			//_tprintf(TEXT("Error code: %d"), GetLastError());
			continue;
		}
		else {
			_tprintf(TEXT("Blog allocated. \n"));
		}
	}
}