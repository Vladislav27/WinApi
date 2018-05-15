#include "task_1.h"

void MaxAllocatingMemory(void) {
	const DWORD PAGELIMIT = 16;            // ��������� ������� �� ���
	const DWORD ArraySize = 1024 * 1024 / PAGELIMIT;               // ���������� ��������� ������ (������� ������)

	LPVOID lpvBase[ArraySize];               // ������ ������ �� ����������������� ������
	LPVOID lpvResult[ArraySize];               // ������ ������ �� ������������ ������
	DWORD dwPageSize;               // ������ �������� �� ���� �����
	SYSTEM_INFO sSysInfo;      // ���������� ��� ��������� ���� � �����

	GetSystemInfo(&sSysInfo);   //�������� ����������
	_tprintf(TEXT("Page size in this computer %d Kb.\n"), sSysInfo.dwPageSize / 1024);

	dwPageSize = sSysInfo.dwPageSize;

	DWORD arrayRealReserveSize = 0;     
	for (DWORD i = 0; i < ArraySize; i++) {
		lpvBase[i] = VirtualAlloc(
			NULL,                 // ��������� ����� ������� ���������
			PAGELIMIT*dwPageSize, // ������
			MEM_RESERVE,          // ��������� ����������������� �������
			PAGE_NOACCESS);       // ������� ������ 
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