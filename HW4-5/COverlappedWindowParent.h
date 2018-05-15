#pragma once
#include "COverlappedWindow.h"
#include <Windows.h>
#include <vector>

class COverlappedWindowParent {
public:
	COverlappedWindowParent();
	~COverlappedWindowParent();
	// ���������������� ����� ����
	static bool RegisterClass();
	// ������� ��������� ����
	bool Create();
	// �������� ����
	void Show(int cmdShow);
protected:
	void OnDestroy();
	void OnCreate();
	void OnNCCreate(HWND Handle);
	void OnSize();
private:
	HWND handle; // ����� ����
	std::vector<COverlappedWindow> children;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};