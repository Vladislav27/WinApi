#pragma once

#include <Windows.h>

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();
	// Зарегистрировать класс окна
	static bool RegisterClass();
	// Создать экземпляр окна
	bool Create(HWND parent);
	// Показать окно
	void Show(int cmdShow);

	HWND GetHandle();
protected:
	void OnDestroy();
	void OnCreate();
	void OnNCCreate(HWND Handle);
	void OnTimer();
	void OnPaint();
	void OnLButtonDown();
private:
	HWND handle; // Хэндл окна
	UINT_PTR id_for_kill_timer;
	long double time; // Параметр времени

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	double offset(long double t);
};
