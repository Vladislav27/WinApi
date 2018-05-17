#pragma once

#include <Windows.h>
#include <Commctrl.h>
#include "resource.h"

struct Settings {
    LOGFONT font;
    BYTE opacity;
    DWORD fontColor, bgColor;
};

class SettingsDialog {
public:
    SettingsDialog();
    ~SettingsDialog();
    bool Create(HWND parentHandle);
    HWND GetHandle();

protected:
    void OnInit(HWND handle_);
    INT_PTR OnCommand(WPARAM wParam);
    void OnDestroy();
    void OnScroll(WPARAM wParam, LPARAM lParam);

private:
    HWND handle, parentHandle, editControlHandle;
    bool preview;
    Settings oldSettings, newSettings;
    HFONT actualFont;
    COLORREF chosenPalitre[16];

    void updateSettings(const Settings& settings);
    void tryUpdate();
    void selectColor(DWORD& targetColor);

    static INT_PTR CALLBACK dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam);
};