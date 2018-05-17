#include "EditControl.h"
#include "resource.h"

EditControl::EditControl() {
    handle = 0;
}

EditControl::~EditControl() {
}

void EditControl::initializeText() {
    HMODULE module = GetModuleHandle(0);
    HRSRC resourceHandle = FindResource(module, MAKEINTRESOURCE(IDR_TEXT3), L"TEXT");
    HGLOBAL handleData = LoadResource(module, resourceHandle);
    DWORD size = SizeofResource(module, resourceHandle);
    LPVOID data = LockResource(handleData);
    SetWindowText(handle, reinterpret_cast<LPCWSTR>(data));
}

bool EditControl::Create(HWND parentHandle) {
    handle = CreateWindowEx(0, L"EDIT", 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | 
        ES_MULTILINE | ES_AUTOVSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, 
        0, GetModuleHandle(0), 0);
    initializeText();
    SetFocus(handle);
    return handle != 0;
}

HWND EditControl::GetHandle() {
    return handle;
}