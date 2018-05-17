#pragma once
#include <Windows.h>

class EditControl {
public:
    EditControl();
    ~EditControl();

    bool Create(HWND parentHandle);
    HWND GetHandle();

private:
    void initializeText();
    HWND handle; 
};