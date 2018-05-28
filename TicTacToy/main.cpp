#include "pch.h"
#include "resource.h"
#include "window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   WindowClass wndcls(hInstance, L"TicTacToeWindowClass", MAKEINTRESOURCE(IDR_MENU_TTT), CallWinProc);   

   TicTacToeWindow wnd;
   if(wnd.Create(
      wndcls.Name(), 
      L"Fun C++: TicTacToe", 
      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 
      CW_USEDEFAULT, 
      CW_USEDEFAULT, 
      300, 
      300, 
      hInstance))
   {
      wnd.ShowWindow(nCmdShow);

      MSG msg;
      while(::GetMessage(&msg, nullptr, 0, 0))
      {
         ::TranslateMessage(&msg);
         ::DispatchMessage(&msg);
      }

      return msg.wParam;
   }

   return 0;
}

