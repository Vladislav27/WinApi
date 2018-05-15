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
	WNDCLASSEX wc; // создаём экземпляр, для обращения к членам класса WNDCLASSEX
	ZeroMemory(&wc, sizeof(wc)); // заполняет блок памяти нулями
	wc.cbSize = sizeof(WNDCLASSEX); // размер структуры (в байтах)
	wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
	wc.lpfnWndProc = windowProc; // указатель на пользовательскую функцию
	wc.lpszMenuName = NULL; // указатель на имя меню
	wc.cbWndExtra = NULL; // число освобождаемых байтов в конце структуры
	wc.cbClsExtra = NULL; // число освобождаемых байтов при создании экземпляра приложения
	wc.hInstance = GetModuleHandle(0); // указатель на строку, содержащую имя меню, применяемого для класса
	wc.lpszClassName = L"OverlappedWindow"; // указатель на имя класса
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // дескриптор кисти для закраски фона окна
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // декриптор пиктограммы
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // дескриптор маленькой пиктограммы (в трэе)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора
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
	RECT clientRect; //  структура определяет координаты верхнего-левого и нижнего правого углов прямоугольника
	PAINTSTRUCT paintStruct; // Структура содержит сведения, которые можно использовать для прорисовки клиентской области окна
	HDC descriptor_window = BeginPaint(handle, &paintStruct); //В BeginPaint функция подготавливает указанное окно для рисования и заполняет PAINTSTRUCT структура с информацией о картине.
	//Если функция завершается успешно, возвращаемое значение является дескриптор контекста устройства отображения для указанного окна.
	GetClientRect(handle, &clientRect); // Возвращает координаты клиентской области окна. Клиентские координаты определяют верхний левый и нижний правый углы клиентской области. 
	HDC DC = CreateCompatibleDC(descriptor_window); // В CreateCompatibleDC функция создает контекст устройства памяти (DC), совместимый с указанным устройством.
	HBITMAP display_buffer = CreateCompatibleBitmap(descriptor_window, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	// В CreateCompatibleBitmap функция создает растровую совместимость с устройством, которое связанно с указанным устройством связи.
	HGDIOBJ oldDisplayBuffer = SelectObject(DC, display_buffer); // функция выделяет объект в заданный контекст устройства (DC). Новый объект заменяет предыдущий объект того же типа

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
	// В FillRect функция заполняет прямоугольник, используя указанную кисть. Эта функция включает в себя левую и верхнюю границы, но исключает правой и нижней границ прямоугольника.
	FillRect(DC, &clientRect, (HBRUSH)GetStockObject(WHITE_PEN)); // 3 аргумент - дискриптор кисти (это все фон)
	
	HBRUSH brush;
	if (GetFocus() == handle) {
		brush = CreateSolidBrush(RGB(0, 255, 0)); // В CreateSolidBrush функция создает логическую кисть, которая имеет указанный сплошной цвет.
	}
	else {
		brush = CreateSolidBrush(RGB(255, 0, 0)); // В CreateSolidBrush функция создает логическую кисть, которая имеет указанный сплошной цвет.
	}
	HGDIOBJ old_brush = SelectObject(DC, brush);
	Ellipse(DC, x - 40, y - 30, x + 40, y + 30); // рисует элипс
	
	SelectObject(DC, old_brush); // возвращаемое значение является дескриптором для объекта замены
	BitBlt(descriptor_window, clientRect.left, clientRect.top, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, DC, 0, 0, SRCCOPY); //
	// функция выполняет bit-block передачу цветовых данных, соответствующий прямоугольник пикселей из указанного исходного контекста устройства в целевое устройство связи.
	SelectObject(DC, oldDisplayBuffer);
	DeleteObject(display_buffer);
	DeleteDC(DC);
	EndPaint(handle, &paintStruct); // функция завершается роспись в указанном окне. Эта функция требуется для каждого вызова BeginPaint
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