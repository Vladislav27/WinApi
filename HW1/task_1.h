#pragma once

#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>   

void max_allocating_memory(void) {
	const DWORD count_pages = 16;            // ��������� ������� �� ���
	const DWORD max_size = 100000;               // ���������� ��������� ������ (������� ������)

	LPVOID lpvReserve[max_size];               // ������ ������ �� ����������������� ������
	LPVOID lpvCommit[max_size];               // ������ ������ �� ������������ ������

	DWORD dwPageSize;               // ������ �������� �� ���� �����
	SYSTEM_INFO info;      // ���������� ��� ��������� ���� � �����
	GetSystemInfo(&info);   //�������� ����������
	_tprintf(TEXT("Page size in this computer %d Kb.\n"), info.dwPageSize / 1024);
	dwPageSize = info.dwPageSize;

	DWORD real_reserve_size = 0;
	for (DWORD i = 0; i < max_size; i++) {
		lpvReserve[i] = VirtualAlloc(
			NULL,                 // ��������� ����� ������� ���������
			count_pages * dwPageSize, // ������
			MEM_RESERVE,          // ��������� ����������������� �������
			PAGE_NOACCESS);       // ������� ������ 
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