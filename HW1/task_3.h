#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>

HANDLE max_create_handle(void) {
	HANDLE event = CreateEvent(
		NULL,               // ���������� ������������
		TRUE,               // ������ ����� - ��, �������������� ����� - ���
		TRUE,              // ��������� ��������� ������� signaled - ��, nonsignaled - ���
		NULL);               // ��� �������
	DWORD size = 0;
	HANDLE handleEvent;
	for (;;) {
		BOOL success = DuplicateHandle(
			GetCurrentProcess(), //���������� ����������
			event, // �������� ���������� �������, ������� �������� ����������
			GetCurrentProcess(), //��������� �� ����������, ������� �������� ��������
			&handleEvent, // ��������� �� ����������, ������� �������� ��������
			0, // ������ ������ ������(����)
			FALSE, //����������� ����������� 
			DUPLICATE_SAME_ACCESS); //���������� 5 ��������. �������� ����� ����� ����� �� ������, ��� �������� �����.
		if (success) {
			size += 1;
		}
		else {
			_tprintf(TEXT("Error code: %d \n"), GetLastError());
			_tprintf(TEXT("Count handles: %d\n"), size);
			break;
		}
	}
	return event;
}