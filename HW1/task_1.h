#pragma once

#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>   

void max_allocating_memory(void) {
	const DWORD count_pages = 16;            // Выделение страниц за раз
	const DWORD max_size = 100000;               // Количество выделений памяти (верхняя планка)

	LPVOID lpvReserve[max_size];               // Храним ссылки на зарезервированную пямять
	LPVOID lpvCommit[max_size];               // Храним ссылки на закомиченную память

	DWORD dwPageSize;               // Размер страницы на этом компе
	SYSTEM_INFO info;      // Структурка для получения инфы о компе
	GetSystemInfo(&info);   //Заполнем структурку
	_tprintf(TEXT("Page size in this computer %d Kb.\n"), info.dwPageSize / 1024);
	dwPageSize = info.dwPageSize;

	DWORD real_reserve_size = 0;
	for (DWORD i = 0; i < max_size; i++) {
		lpvReserve[i] = VirtualAlloc(
			NULL,                 // Начальный адрес области выделения
			count_pages * dwPageSize, // Размер
			MEM_RESERVE,          // Выбеление зарезервированных страниц
			PAGE_NOACCESS);       // Степень защиты 
		if (lpvReserve[i] == NULL) {
			_tprintf(TEXT("Error code: %d \n"), GetLastError());
			break;
		}
		else {
			real_reserve_size += 1;
		}
	}
	_tprintf(TEXT("Reserve: %d Mb %d.\n"), real_reserve_size * dwPageSize * count_pages / 1024 / 1024,
		real_reserve_size);

	DWORD real_commit_size = 0;
	for (DWORD i = 0; i < real_reserve_size; i++) {
		lpvCommit[i] = VirtualAlloc(
			(LPVOID)lpvReserve[i],
			count_pages* dwPageSize,
			MEM_COMMIT,
			PAGE_NOACCESS);
		if (lpvCommit[i] == NULL) {
			_tprintf(TEXT("Error code: %d \n"), GetLastError());
			continue;
		}
		else {
			real_commit_size += 1;
		}
	}
	_tprintf(TEXT("Commit: %d Mb %d.\n"), real_commit_size * dwPageSize * count_pages / 1024 / 1024,
		real_commit_size);
}