#include <Windows.h>
#include "COverlappedWindowParent.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	bool hRegisterClass;
	hRegisterClass = COverlappedWindowParent::RegisterClass();
	
	if (!hRegisterClass) {
		// в случае некорректного создания окошка (неверные параметры и тп):
		MessageBox(NULL, L"Не получилось создать окно (Error Register)!", L"Ошибка", MB_OK);
		return 0;
	}
	bool hMainWnd;
	COverlappedWindowParent window;
	hMainWnd = window.Create();
	if (!hMainWnd) {
		// в случае некорректного создания окошка (неверные параметры и тп):
		MessageBox(NULL, L"Не получилось создать окно (Error Create)!", L"Ошибка", MB_OK);
		return 0;
	}
	window.Show(nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL)) { // извлекаем сообщения из очереди, посылаемые функциями, ОС
		DispatchMessage(&msg); // передаём сообщения обратно ОС
	}
	return 0;
}