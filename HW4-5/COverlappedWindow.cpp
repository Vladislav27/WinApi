#include"COverlappedWindow.h"
#include <Windows.h>
#include <cmath>

COverlappedWindow::COverlappedWindow() {
	handle = 0;
	id_for_kill_timer = 0;
	time = 0;
}

COverlappedWindow::~COverlappedWindow() {
}

bool COverlappedWindow::RegisterClass() {
	WNDCLASSEX wc; // ������ ���������, ��� ��������� � ������ ������ WNDCLASSEX
	ZeroMemory(&wc, sizeof(wc)); // ��������� ���� ������ ������
	wc.cbSize = sizeof(WNDCLASSEX); // ������ ��������� (� ������)
	wc.style = CS_HREDRAW | CS_VREDRAW; // ����� ������ ������
	wc.lpfnWndProc = windowProc; // ��������� �� ���������������� �������
	wc.lpszMenuName = NULL; // ��������� �� ��� ����
	wc.cbWndExtra = NULL; // ����� ������������� ������ � ����� ���������
	wc.cbClsExtra = NULL; // ����� ������������� ������ ��� �������� ���������� ����������
	wc.hInstance = GetModuleHandle(0); // ��������� �� ������, ���������� ��� ����, ������������ ��� ������
	wc.lpszClassName = L"OverlappedWindow"; // ��������� �� ��� ������
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ���������� ����� ��� �������� ���� ����
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // ��������� �����������
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // ���������� ��������� ����������� (� ����)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ���������� �������
	return (RegisterClassEx(&wc) != 0);
}

bool COverlappedWindow::Create(HWND parent) {
	CreateWindowEx(0, L"OverlappedWindow", L"Window", WS_CHILD | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, 0, GetModuleHandle(0), this);
	return (handle != 0);
}

void COverlappedWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
}

void COverlappedWindow::OnNCCreate(HWND Handle) {
	handle = Handle;
}

void COverlappedWindow::OnCreate() {
	id_for_kill_timer = SetTimer(handle, 0, 20, 0);
}

double COverlappedWindow::offset(long double t) {
	switch (int(t) % 4) {
	case 0:
		return (t - int(t));
	case 1:
		return (1 - t + int(t));
	case 2:
		return (int(t) - t);
	case 3:
		return (t - 1 - int(t));
	default:
		return 0;
	}
}

void COverlappedWindow::OnPaint() {
	RECT clientRect; //  ��������� ���������� ���������� ��������-������ � ������� ������� ����� ��������������
	PAINTSTRUCT paintStruct; // ��������� �������� ��������, ������� ����� ������������ ��� ���������� ���������� ������� ����
	HDC descriptor_window = BeginPaint(handle, &paintStruct); //� BeginPaint ������� �������������� ��������� ���� ��� ��������� � ��������� PAINTSTRUCT ��������� � ����������� � �������.
	//���� ������� ����������� �������, ������������ �������� �������� ���������� ��������� ���������� ����������� ��� ���������� ����.
	GetClientRect(handle, &clientRect); // ���������� ���������� ���������� ������� ����. ���������� ���������� ���������� ������� ����� � ������ ������ ���� ���������� �������. 
	HDC DC = CreateCompatibleDC(descriptor_window); // � CreateCompatibleDC ������� ������� �������� ���������� ������ (DC), ����������� � ��������� �����������.
	HBITMAP display_buffer = CreateCompatibleBitmap(descriptor_window, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	// � CreateCompatibleBitmap ������� ������� ��������� ������������� � �����������, ������� �������� � ��������� ����������� �����.
	HGDIOBJ oldDisplayBuffer = SelectObject(DC, display_buffer); // ������� �������� ������ � �������� �������� ���������� (DC). ����� ������ �������� ���������� ������ ���� �� ����

	int x, y;
	if (GetFocus() == handle) {
		int offset = (clientRect.right - clientRect.left) / 2 - 50;
		x = int((clientRect.left + clientRect.right) / 2 + offset * COverlappedWindow::offset(time));
		y = (clientRect.top + clientRect.bottom) / 2;
	}
	else {
		x = (clientRect.right - clientRect.left ) / 2;
		y = (clientRect.bottom - clientRect.top) / 2;
	}
	// � FillRect ������� ��������� �������������, ��������� ��������� �����. ��� ������� �������� � ���� ����� � ������� �������, �� ��������� ������ � ������ ������ ��������������.
	FillRect(DC, &clientRect, (HBRUSH)GetStockObject(WHITE_PEN)); // 3 �������� - ���������� ����� (��� ��� ���)
	
	HBRUSH brush;
	if (GetFocus() == handle) {
		brush = CreateSolidBrush(RGB(0, 255, 0)); // � CreateSolidBrush ������� ������� ���������� �����, ������� ����� ��������� �������� ����.
	}
	else {
		brush = CreateSolidBrush(RGB(255, 0, 0)); // � CreateSolidBrush ������� ������� ���������� �����, ������� ����� ��������� �������� ����.
	}
	HGDIOBJ old_brush = SelectObject(DC, brush);
	Ellipse(DC, x - 40, y - 30, x + 40, y + 30); // ������ �����
	
	SelectObject(DC, old_brush); // ������������ �������� �������� ������������ ��� ������� ������
	BitBlt(descriptor_window, clientRect.left, clientRect.top, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, DC, 0, 0, SRCCOPY); //
	// ������� ��������� bit-block �������� �������� ������, ��������������� ������������� �������� �� ���������� ��������� ��������� ���������� � ������� ���������� �����.
	SelectObject(DC, oldDisplayBuffer);
	DeleteObject(display_buffer);
	DeleteDC(DC);
	EndPaint(handle, &paintStruct); // ������� ����������� ������� � ��������� ����. ��� ������� ��������� ��� ������� ������ BeginPaint
}

void COverlappedWindow::OnTimer() {
	if (GetFocus() == handle) {
		RECT rect;
		GetClientRect(handle, &rect);
		time += 0.05;
		InvalidateRect(handle, &rect, FALSE);
	}
}

void COverlappedWindow::OnDestroy() {
	KillTimer(handle, id_for_kill_timer);
	PostQuitMessage(0);
}

void COverlappedWindow::OnLButtonDown() {
	SetFocus(handle);
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_NCCREATE: {
		COverlappedWindow* window = (COverlappedWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetLastError(0);
		SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)window);
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CREATE: {
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY: {
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnDestroy();
		return 0;
	}
	case WM_PAINT: {
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnPaint();
		return 0;
	}
	case WM_TIMER: {
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnTimer();
		return 0;
	}
	case WM_LBUTTONDOWN: {
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnLButtonDown();
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}

HWND COverlappedWindow::GetHandle() {
	return handle;
}