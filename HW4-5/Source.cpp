#include <Windows.h>
#include "COverlappedWindowParent.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	bool hRegisterClass;
	hRegisterClass = COverlappedWindowParent::RegisterClass();
	
	if (!hRegisterClass) {
		// � ������ ������������� �������� ������ (�������� ��������� � ��):
		MessageBox(NULL, L"�� ���������� ������� ���� (Error Register)!", L"������", MB_OK);
		return 0;
	}
	bool hMainWnd;
	COverlappedWindowParent window;
	hMainWnd = window.Create();
	if (!hMainWnd) {
		// � ������ ������������� �������� ������ (�������� ��������� � ��):
		MessageBox(NULL, L"�� ���������� ������� ���� (Error Create)!", L"������", MB_OK);
		return 0;
	}
	window.Show(nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL)) { // ��������� ��������� �� �������, ���������� ���������, ��
		DispatchMessage(&msg); // ������� ��������� ������� ��
	}
	return 0;
}