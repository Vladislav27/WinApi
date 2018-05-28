#pragma 

class WindowClass
{
   WNDCLASSEX wndcls;

public:
   WindowClass(HINSTANCE hInstance, LPCTSTR lpszName, LPCTSTR lpszMenu, WNDPROC winProc);
   ~WindowClass();

   LPCTSTR Name() const {return wndcls.lpszClassName;}
};

class DeviceContext 
{
   HWND hWnd;
   HDC hDC;
   PAINTSTRUCT ps;

public:
   DeviceContext(HWND const wnd):hWnd(wnd)
   {
      hDC = ::BeginPaint(hWnd, &ps);
   }

   ~DeviceContext()
   {
      ::EndPaint(hWnd, &ps);
   }

   operator HDC() const {return hDC;}
};

class Window
{
protected:
   HWND hWnd;

   virtual void OnPaint(DeviceContext* dc);
   virtual void OnLeftButtonUp(int x, int y, WPARAM params);
   virtual void OnMenuItemClicked(int menuId);

public:
   virtual ~Window() {}
   bool Create(LPCTSTR lpszClassName, LPCTSTR lpszText, DWORD style, int x, int y, int cx, int cy, HINSTANCE hInstance);
   void ShowWindow(int nCmdShow);
   void Invalidate(BOOL erase = TRUE) {::InvalidateRect(hWnd, nullptr, erase);}

   LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);   

   operator HWND() const {return hWnd;}
};

Window* WindowFromHandler(HWND const hWnd);
LRESULT CALLBACK CallWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
