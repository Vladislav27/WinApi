#pragma once

#include <Windows.h>
#include "EditControl.h"
#include "Settings.h"
#include "resource.h"

class OverlappedWindow {
public:
    OverlappedWindow();
    ~OverlappedWindow();

    static bool RegisterClass();
    bool Create();
    void Show(int cmdShow);
	HWND GetHandle();
    HWND GetOptionsDialogHandle();
    HWND GetEditControlHandle();
    COLORREF GetBgColor();
    COLORREF GetTextColor();
    BYTE GetOpacity();
    void SetColors(COLORREF bgColor, COLORREF textColor);
    void SetOpacity(BYTE opacity);

protected:
    void OnNCCreate(HWND handle);
    void OnCreate();
    void OnSize();
    bool OnClose();
    void OnCommand(WPARAM wParam, LPARAM lParam);
    void OnCommandMenu(WPARAM wParam);
    void OnCommandAccelerator(WPARAM wParam);

    LRESULT OnControlColorEdit(HDC hdc);
    void OnDestroy();

private:
    HWND handle;
    EditControl editControl; 
    SettingsDialog settingsDialog; 
    bool changed; 
    HMENU menu;
    COLORREF textColor, bgColor;
    BYTE opacity;
    HBRUSH activeBrush;

    static const HICON smallIcon, largeIcon; 
    static const int maxFileNameSize = 256;

    void saveText(); 

    static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
