#include "task_1.h"

void MaxAllocatingMemory(void) {
	const DWORD PAGELIMIT = 16;            // Выделение страниц за раз
	const DWORD ArraySize = 1024 * 1024 / PAGELIMIT;               // Количество выделений памяти (верхняя планка)

	LPVOID lpvBase[ArraySize];               // Храним ссылки на зарезервированную пямять
	LPVOID lpvResult[ArraySize];               // Храним ссылки на закомиченную память
	DWORD dwPageSize;               // Размер страницы на этом компе
	SYSTEM_INFO sSysInfo;      // Структурка для получения инфы о компе

	GetSystemInfo(&sSysInfo);   //Заполнем структурку
	_tprintf(TEXT("Page size in this computer %d Kb.\n"), sSysInfo.dwPageSize / 1024);

	dwPageSize = sSysInfo.dwPageSize;

	DWORD arrayRealReserveSize = 0;     
	for (DWORD i = 0; i < ArraySize; i++) {
		lpvBase[i] = VirtualAlloc(
			NULL,                 // Начальный адрес области выделения
			PAGELIMIT*dwPageSize, // Размер
			MEM_RESERVE,          // Выбеление зарезервированных страниц
			PAGE_NOACCESS);       // Степень защиты 
		if (lpvBase[i] == NULL) {
			break;
		}
		else {
			arrayRealReserveSize += 1;
		}
	}
	_tprintf(TEXT("Reserve: %d Mb %d.\n"), arrayRealReserveSize*(dwPageSize / 1024)*PAGELIMIT / 1024, arrayRealReserveSize);


	DWORD arrayRealCommitSize = 0;
	for (DWORD i = 0; i < arrayRealReserveSize; i++) {
		lpvResult[i] = VirtualAlloc(
			(LPVOID)lpvBase[i],                
			PAGELIMIT*dwPageSize, 
			MEM_COMMIT,         
			PAGE_NOACCESS);       
		if (lpvResult[i] == NULL) {
			continue;
		}
		else {
			arrayRealCommitSize += 1;
		}
	}
	_tprintf(TEXT("Commit: %d Mb %d.\n"), arrayRealCommitSize*(dwPageSize / 1024)*PAGELIMIT / 1024, arrayRealCommitSize);
}