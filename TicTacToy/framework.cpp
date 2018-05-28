#include "pch.h"
#include "framework.h"

std::map<HWND, Window*> g_wndMap;

WindowClass::WindowClass(HINSTANCE hInstance, LPCTSTR lpszName, LPCTSTR lpszMenu, WNDPROC winProc)
{
   ::ZeroMemory(&wndcls, sizeof(wndcls));

   wndcls.cbSize = sizeof(wndcls);
   wndcls.style = CS_HREDRAW | CS_VREDRAW;
   wndcls.lpfnWndProc = winProc;
   wndcls.hInstance = hInstance;
   wndcls.hCursor = ::LoadCursor(NULL, IDC_ARROW);
   wndcls.hbrBackground = (HBRUSH)COLOR_WINDOW;
   wndcls.lpszClassName = lpszName;
   wndcls.lpszMenuName = lpszMenu;

   ::RegisterClassEx(&wndcls);
}

WindowClass::~WindowClass()
{
   ::UnregisterClass(wndcls.lpszClassName, wndcls.hInstance);
}

bool Window::Create(LPCTSTR lpszClassName, LPCTSTR lpszText, DWORD style, int x, int y, int cx, int cy, HINSTANCE hInstance)
{
   RECT wndrc = {0, 0, cx, cy};
   ::AdjustWindowRect(&wndrc, style, TRUE);

   hWnd = ::CreateWindowEx(
      NULL,
      lpszClassName,
      lpszText,
      style,
      x,
      y,
      wndrc.right - wndrc.left,
      wndrc.bottom - wndrc.top,
      nullptr,
      nullptr,
      hInstance,
      nullptr);

   if(hWnd != nullptr)
      g_wndMap.insert(std::make_pair(hWnd, this));

   return hWnd != nullptr;
}

void Window::ShowWindow(int nCmdShow)
{
   ::ShowWindow(hWnd, nCmdShow);
}

void Window::OnPaint(DeviceContext* dc)
{

}

void Window::OnLeftButtonUp(int x, int y, WPARAM params)
{

}

void Window::OnMenuItemClicked(int menuId)
{

}

LRESULT Window::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
   switch(message)
   {
   case WM_PAINT:
      {
         DeviceContext dc(hWnd);
         OnPaint(&dc);
      }
      break;

   case WM_LBUTTONUP:
      OnLeftButtonUp(LOWORD(lParam), HIWORD(lParam), lParam);
      break;

   case WM_DESTROY:
      ::PostQuitMessage(0);
      return 0;

   case WM_COMMAND:
      {
         WORD hi = HIWORD(wParam);
         WORD lo = LOWORD(wParam);

         if(hi == 0 || hi == 1)
            OnMenuItemClicked(lo);
      }
      break;
   }

   return ::DefWindowProc(hWnd, message, wParam, lParam);
}

Window* WindowFromHandler(HWND const hWnd)
{
   auto result = g_wndMap.find(hWnd);
   return result == g_wndMap.end() ? nullptr : result->second;
}

LRESULT CALLBACK CallWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   auto wnd = WindowFromHandler(hWnd);
   if(wnd == nullptr || (HWND)wnd == nullptr)
      return ::DefWindowProc(hWnd, message, wParam, lParam);

   return wnd->WndProc(message, wParam, lParam);
}