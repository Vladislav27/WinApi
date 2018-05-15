#pragma once
#include "COverlappedWindow.h"
#include <Windows.h>
#include <vector>

class COverlappedWindowParent {
public:
	COverlappedWindowParent();
	~COverlappedWindowParent();
	// Зарегистрировать класс окна
	static bool RegisterClass();
	// Создать экземпляр окна
	bool Create();
	// Показать окно
	void Show(int cmdShow);
protected:
	void OnDestroy();
	void OnCreate();
	void OnNCCreate(HWND Handle);
	void OnSize();
private:
	HWND handle; // Хэндл окна
	std::vector<COverlappedWindow> children;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};