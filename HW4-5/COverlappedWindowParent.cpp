#include "COverlappedWindow.h"
#include "COverlappedWindowParent.h"
#include <Windows.h>

#define N 3
#define M 3

COverlappedWindowParent::COverlappedWindowParent() {
	handle = 0;

	for (int i = 0; i < N * M; i++) {
		children.push_back(COverlappedWindow());
	}
}

COverlappedWindowParent::~COverlappedWindowParent() {
}

bool COverlappedWindowParent::RegisterClass() {
	WNDCLASSEX wc; // ������ ���������, ��� ��������� � ������ ������ WNDCLASSEX
	ZeroMemory(&wc, sizeof(wc)); // ��������� ���� ������ ������
	wc.cbSize = sizeof(WNDCLASSEX); // ������ ��������� (� ������)
	wc.style = CS_HREDRAW | CS_VREDRAW; // ����� ������ ������
	wc.lpfnWndProc = windowProc; // ��������� �� ���������������� �������
	wc.lpszMenuName = NULL; // ��������� �� ��� ����
	wc.cbWndExtra = NULL; // ����� ������������� ������ � ����� ���������
	wc.cbClsExtra = NULL; // ����� ������������� ������ ��� �������� ���������� ����������
	wc.hInstance = GetModuleHandle(0); // ��������� �� ������, ���������� ��� ����, ������������ ��� ������
	wc.lpszClassName = L"OverlappedWindowParent"; // ��������� �� ��� ������
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ���������� ����� ��� �������� ���� ����
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // ��������� �����������
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // ���������� ��������� ����������� (� ����)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ���������� �������
	return (RegisterClassEx(&wc) != 0);
}

bool COverlappedWindowParent::Create() {
	CreateWindowEx(0, L"OverlappedWindowParent", L"Parent Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), this);
	return (handle != 0);
}

void COverlappedWindowParent::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	for (unsigned int i = 0; i < N * M; i++) {
		children[i].Show(cmdShow);
	}
}

void COverlappedWindowParent::OnNCCreate(HWND _handle) {
	handle = _handle;
}

void COverlappedWindowParent::OnCreate() {
	COverlappedWindow::RegisterClass();
	for (int i = 0; i < N * M; i++) {
		children[i].Create(handle);
	}
}

void COverlappedWindowParent::OnSize() {
	RECT rect;
	GetClientRect(handle, &rect);

	int steps_x = (rect.right - rect.left) / M;
	int steps_y = (rect.bottom - rect.top) / N;

	int left_up_x = rect.left;
	int left_up_y = rect.top;
	int index = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			SetWindowPos(children[index].GetHandle(), HWND_TOP, left_up_x, left_up_y, steps_x, steps_y, 0);
			index++;
			left_up_x += steps_x;
		}
		left_up_x = rect.left;
		left_up_y += steps_y;
	}
}

void COverlappedWindowParent::OnDestroy() {
}

LRESULT COverlappedWindowParent::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_NCCREATE: {
		COverlappedWindowParent* window = (COverlappedWindowParent*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetLastError(0);
		SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)window);
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CREATE: {
		COverlappedWindowParent* window = (COverlappedWindowParent*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_SIZE: {
		COverlappedWindowParent* window = (COverlappedWindowParent*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnSize();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY: {
		COverlappedWindowParent* window = (COverlappedWindowParent*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnDestroy();
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}