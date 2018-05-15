#pragma once

#include <Windows.h>

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();
	// ���������������� ����� ����
	static bool RegisterClass();
	// ������� ��������� ����
	bool Create(HWND parent);
	// �������� ����
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
	HWND handle; // ����� ����
	UINT_PTR id_for_kill_timer;
	long double time; // �������� �������

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	double offset(long double t);
};
