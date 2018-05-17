#include "OverlappedWindow.h"

OverlappedWindow::OverlappedWindow() {
    handle = 0;
    menu = 0;
    changed = 0;
    textColor = 0;
    bgColor = 0;
    opacity = 0;
    activeBrush = 0;
    editControl = EditControl();
    settingsDialog = SettingsDialog();
}

OverlappedWindow::~OverlappedWindow() {
}

bool OverlappedWindow::RegisterClass() {
    WNDCLASSEX windowClass;
    ::ZeroMemory(&windowClass, sizeof(windowClass));
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.lpfnWndProc = OverlappedWindow::windowProc;
    windowClass.hInstance = GetModuleHandle(0);
    windowClass.lpszClassName = L"OverlappedWindow";
    windowClass.hIcon = reinterpret_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));;
    windowClass.hIconSm = reinterpret_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));;
    windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    return (::RegisterClassEx(&windowClass) != 0);
}

bool OverlappedWindow::Create() {
    LPTSTR windowTitle = new wchar_t[256];
    LoadString(GetModuleHandle(0), IDS_NOTEPAD_TITLE, windowTitle, 256);
    CreateWindowEx(WS_EX_LAYERED, L"OverlappedWindow", windowTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), this);
    SetLayeredWindowAttributes(handle, 0, 255, LWA_ALPHA);
    opacity = 255;
    changed = false;
    delete[] windowTitle;
    return (handle != 0);
}

void OverlappedWindow::OnCreate() {
    textColor = RGB(0, 0, 0);
    bgColor = RGB(255, 255, 255);
    editControl.Create(handle);
    menu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
    HFONT hFont = CreateFont(14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    SendMessage(editControl.GetHandle(), WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
}

void OverlappedWindow::OnSize() {
    RECT rect;
    GetClientRect(handle, &rect);
    SetWindowPos(editControl.GetHandle(), HWND_TOP, rect.left, rect.top, rect.right - rect.left,
        rect.bottom - rect.top, 0);
}

void OverlappedWindow::saveText() {
    int length = SendMessage(editControl.GetHandle(), WM_GETTEXTLENGTH, 0, 0);
    wchar_t *text = new wchar_t[length + 1];
    SendMessage(editControl.GetHandle(), WM_GETTEXT, length + 1, reinterpret_cast<LPARAM>(text));
    wchar_t fileName[maxFileNameSize];
    OPENFILENAME ofn = {};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFile = fileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(fileName);
    ofn.hwndOwner = handle;
    if (GetSaveFileName(&ofn)) {
        HANDLE file = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        unsigned short a = 0xFEFF;
        WriteFile(file, &a, 2, 0, 0);
        WriteFile(file, text, length * sizeof(wchar_t), 0, 0);
        CloseHandle(file);
    }
    delete[] text;
}

bool OverlappedWindow::OnClose() {
    if (changed) {
        int msgBoxID = MessageBox(handle, L"Сохранить?", L"SaveBox", MB_YESNOCANCEL);
        switch (msgBoxID)
        {
            case IDYES:
            {
                saveText();
                return true;
            }
            case IDNO:
            {
                return true;
            }
            case IDCANCEL:
            {
                return false;
            }
        }
    }
    return true;
}

void OverlappedWindow::OnCommandMenu(WPARAM wParam) {
    switch LOWORD(wParam) {
        case ID_FILE_SAVE: {
            saveText();
            break;
        }
        case ID_FILE_EXIT: {
            SendMessage(handle, WM_CLOSE, 0, 0);
            break;
        }
        case ID_VIEW_SETTINGS: {
            settingsDialog.Create(handle);
            break;
        }
    }
}

void OverlappedWindow::OnCommandAccelerator(WPARAM wParam) {
    switch LOWORD(wParam)
    {
        case ID_ACCELERATOR_QUIT: {
            SendMessage(handle, WM_DESTROY, 0, 0);
            break;
        }
    }
}

void OverlappedWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
    switch HIWORD(wParam)
    {
        case EN_CHANGE:
        {
            changed = true;
            break;
        }
        case 0:
        {
            OnCommandMenu(wParam);
            break;
        }
        case 1:
        {
            OnCommandAccelerator(wParam);
            break;
        }
    }
}

LRESULT OverlappedWindow::OnControlColorEdit(HDC hdc) {
    SetTextColor(hdc, textColor);
    SetBkColor(hdc, bgColor);
    if (activeBrush != 0) {
        DeleteObject(activeBrush);
    }
    activeBrush = CreateSolidBrush(bgColor);
    return (LRESULT)activeBrush;
}

void OverlappedWindow::OnDestroy() {
    DestroyIcon(reinterpret_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0)));
    DestroyIcon(reinterpret_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0)));
    DestroyMenu(menu);
    HFONT font = reinterpret_cast<HFONT>(SendMessage(editControl.GetHandle(), WM_GETFONT, 0, 0));
    DeleteObject(font);
    if (activeBrush != 0) {
        DeleteObject(activeBrush);
    }
    PostQuitMessage(0);
}

HWND OverlappedWindow::GetHandle() {
	return handle;
}

HWND OverlappedWindow::GetOptionsDialogHandle() {
	return settingsDialog.GetHandle();
}

HWND OverlappedWindow::GetEditControlHandle() {
	return editControl.GetHandle();
}

COLORREF OverlappedWindow::GetBgColor() {
	return bgColor;
}

COLORREF OverlappedWindow::GetTextColor() {
	return textColor;
}

BYTE OverlappedWindow::GetOpacity() {
	return opacity;
}

void OverlappedWindow::SetColors(COLORREF bgColor_, COLORREF textColor_) {
	bgColor = bgColor_;
	textColor = textColor_;
}

void OverlappedWindow::SetOpacity(BYTE opacity_) {
	opacity = opacity_;
}

void OverlappedWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
}

void OverlappedWindow::OnNCCreate(HWND _handle) {
	handle = _handle;
}

LRESULT OverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_NCCREATE) {
        OverlappedWindow* window = reinterpret_cast<OverlappedWindow*>((reinterpret_cast<CREATESTRUCT*>(lParam))->lpCreateParams);
        SetLastError(0);
        SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)window);
        if (GetLastError() != 0) {
            return GetLastError();
        }
        window->OnNCCreate(handle);
        return DefWindowProc(handle, message, wParam, lParam);
    }
    OverlappedWindow* window = reinterpret_cast<OverlappedWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA));
    switch (message) {
        case WM_CREATE: {
            window->OnCreate();
            return DefWindowProc(handle, message, wParam, lParam);
        }
        case WM_SIZE: {
            window->OnSize();
            return DefWindowProc(handle, message, wParam, lParam);
        }
        case WM_CLOSE: {
            if( window->OnClose() ) {
                return DefWindowProc(handle, message, wParam, lParam);
            }
            else {
                return 0;
            }
        }
        case WM_COMMAND: {
            window->OnCommand(wParam, lParam);
            return DefWindowProc(handle, message, wParam, lParam);
        }
        case WM_DESTROY: {
            window->OnDestroy();
            return 0;
        }
        case WM_CTLCOLOREDIT: {
            return window->OnControlColorEdit(reinterpret_cast<HDC>(wParam));
        }
        default: {
            return DefWindowProc(handle, message, wParam, lParam);
        }
    }
}