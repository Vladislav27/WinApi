#include "pch.h"
#include "window.h"
#include "resource.h"
#include <tchar.h>

TicTacToeWindow::TicTacToeWindow()
{
   auto hModule = ::GetModuleHandle(nullptr);
   hBmp0 = ::LoadBitmap(hModule, MAKEINTRESOURCE(IDB_BITMAP_0));
   hBmpX = ::LoadBitmap(hModule, MAKEINTRESOURCE(IDB_BITMAP_X));

   ::GetObject(hBmp0, sizeof(bmp0), &bmp0);
   ::GetObject(hBmpX, sizeof(bmpX), &bmpX);  
}

TicTacToeWindow::~TicTacToeWindow()
{
   ::DeleteObject(hBmp0);
   ::DeleteObject(hBmpX);
}

void TicTacToeWindow::OnLeftButtonUp(int x, int y, WPARAM params)
{
   if(game.is_started() && !game.is_finished())
   {
      RECT rcClient;
      ::GetClientRect(hWnd, &rcClient);

      int cellw = (rcClient.right - rcClient.left) / 3;
      int cellh = (rcClient.bottom - rcClient.top) / 3;

      int col = x / cellw;
      int row = y / cellh;

      if(game.move(tictactoe_cell(row, col), tictactoe_player::user))
      {
         if(!game.is_finished())
            game.move(tictactoe_player::computer);

         Invalidate(FALSE);
      }
   }
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND hwndMain;

void game_2players(HWND hWnd, HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
	PostMessage(hWnd, WM_CLOSE, 0, 0);
	static TCHAR szAppName[] = _T("TicTacToe for 2 Players");
	MSG msg;
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwndMain = CreateWindowEx(0,
		szAppName,
		_T("TicTacToe for 2 Players"),
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		366,
		382,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwndMain, iCmdShow);
	UpdateWindow(hwndMain);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

#define EX 1
#define OH 2

const RECT m_rcSquares[9] = {
	{ 16, 16,112,112 },
	{ 128, 16,224,112 },
	{ 240, 16,336,112 },
	{ 16,128,112,224 },
	{ 128,128,224,224 },
	{ 240,128,336,224 },
	{ 16,240,112,336 },
	{ 128,240,224,336 },
	{ 240,240,336,336 }
};

int m_nNextChar = EX, m_nGameGrid[9];

int GetRectID(int x, int y) {
	for (int i = 0; i < 9; i++) {
		if (x > m_rcSquares[i].left && x < m_rcSquares[i].right &&
			y > m_rcSquares[i].top && y < m_rcSquares[i].bottom)
			return i;
	}
	return -1;
}

void DrawX(HDC hdc, int nPos) {
	HPEN pen = CreatePen(PS_SOLID, 16, RGB(255, 0, 0));
	void *oldPen = SelectObject(hdc, pen);

	RECT rect = m_rcSquares[nPos];

	rect.top += 10;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;

	MoveToEx(hdc, rect.left, rect.top, NULL);
	LineTo(hdc, rect.right, rect.bottom);
	MoveToEx(hdc, rect.left, rect.bottom, NULL);
	LineTo(hdc, rect.right, rect.top);

	SelectObject(hdc, (HGDIOBJ)oldPen);
	DeleteObject(pen);
}

void DrawO(HDC hdc, int nPos) {
	HPEN pen = ::CreatePen(PS_SOLID, 16, RGB(0, 0, 255));
	void *oldPen = ::SelectObject(hdc, pen);
	void *oldBr = ::SelectObject(hdc, GetStockObject(NULL_BRUSH));

	RECT rect = m_rcSquares[nPos];

	rect.top += 10;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;

	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	SelectObject(hdc, (HGDIOBJ)oldBr);
	SelectObject(hdc, (HGDIOBJ)oldPen);
	DeleteObject(pen);
}

void DrawBoard(HDC hdc)
{
	HPEN pen = ::CreatePen(PS_SOLID, 16, RGB(0, 0, 0));
	void *oldPen = ::SelectObject(hdc, pen);

	MoveToEx(hdc, 120, 16, NULL);
	LineTo(hdc, 120, 336);

	MoveToEx(hdc, 232, 16, NULL);
	LineTo(hdc, 232, 336);

	MoveToEx(hdc, 16, 120, NULL);
	LineTo(hdc, 336, 120);

	MoveToEx(hdc, 16, 232, NULL);
	LineTo(hdc, 336, 232);

	for (int i = 0; i < 9; i++)
	{
		if (m_nGameGrid[i] == EX)
			DrawX(hdc, i);
		else if (m_nGameGrid[i] == OH)
			DrawO(hdc, i);
	}
	SelectObject(hdc, (HGDIOBJ)oldPen);
	DeleteObject(pen);
}

int IsWinner()
{
	static int nPattern[8][3] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		0, 3, 6,
		1, 4, 7,
		2, 5, 8,
		0, 4, 8,
		2, 4, 6
	};

	for (int i = 0; i < 8; i++)
	{
		if ((m_nGameGrid[nPattern[i][0]] == EX) &&
			(m_nGameGrid[nPattern[i][1]] == EX) &&
			(m_nGameGrid[nPattern[i][2]] == EX))
			return EX;
		if ((m_nGameGrid[nPattern[i][0]] == OH) &&
			(m_nGameGrid[nPattern[i][1]] == OH) &&
			(m_nGameGrid[nPattern[i][2]] == OH))
			return OH;
	}
	return 0;
}

BOOL IsDraw() {
	for (int i = 0; i < 9; i++)
	{
		if (m_nGameGrid[i] == 0)
			return FALSE;
	}
	return TRUE;
}

void ResetGame() {
	RECT rect;

	m_nNextChar = EX;
	::ZeroMemory(m_nGameGrid, 9 * sizeof(int));
	::GetClientRect(hwndMain, &rect);
	::InvalidateRect(hwndMain, &rect, TRUE);
}

void CheckForGameOver() {
	int nWinner;

	if (nWinner = IsWinner())
	{
		MessageBox(hwndMain, ((nWinner == EX) ? _T("X Won!") : _T("O Won!")),
			_T("Game over!"), MB_ICONEXCLAMATION | MB_OK);
		ResetGame();
	}
	else if (IsDraw())
	{
		MessageBox(hwndMain, _T("It's a draw!"), _T("Game over!"), MB_ICONEXCLAMATION | MB_OK);
		ResetGame();
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int nPos;

	switch (iMsg)
	{
	case WM_LBUTTONDOWN:
		if (m_nNextChar != EX)
			return 0;
		nPos = GetRectID(LOWORD(lParam), HIWORD(lParam));
		if ((nPos == -1) || (m_nGameGrid[nPos] != 0))
			return 0;
		m_nGameGrid[nPos] = EX;
		m_nNextChar = OH;
		hdc = ::GetDC(hwndMain);
		DrawX(hdc, nPos);
		CheckForGameOver();
		return 0;

	case WM_RBUTTONDOWN:
		if (m_nNextChar != OH)
			return 0;
		nPos = GetRectID(LOWORD(lParam), HIWORD(lParam));
		if ((nPos == -1) || (m_nGameGrid[nPos] != 0))
			return 0;
		m_nGameGrid[nPos] = OH;
		m_nNextChar = EX;
		hdc = ::GetDC(hwndMain);
		DrawO(hdc, nPos);
		CheckForGameOver();
		return 0;

	case WM_MBUTTONDOWN:
		ResetGame();
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SetBkMode(hdc, TRANSPARENT);
		DrawBoard(hdc);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void TicTacToeWindow::OnMenuItemClicked(int menuId)
{
   switch(menuId)
   {
   case ID_GAME_EXIT:
      ::PostMessage(hWnd, WM_CLOSE, 0, 0);
      break;

   case ID_GAME_STARTUSER:
      game.start(tictactoe_player::user);
      Invalidate(FALSE);
      break;

   case ID_GAME_STARTCOMPUTER:
      game.start(tictactoe_player::computer);
      game.move(tictactoe_player::computer);
      Invalidate(FALSE);
      break;

   case ID_GAME_2PLAYER:
	   game_2players(hWnd, 0, 0, 0, 10);
	   break;
   }
}

void TicTacToeWindow::DrawBackground(HDC dc, RECT rc)
{
   auto brWhite = ::CreateSolidBrush(COLORREF(0xffffff));
   auto brOld = ::SelectObject(dc, brWhite);

   ::FillRect(dc, &rc, static_cast<HBRUSH>(brOld));

   ::SelectObject(dc, brOld);
}

void TicTacToeWindow::DrawGrid(HDC dc, RECT rc)
{
   auto penLine = ::CreatePen(PS_SOLID, 1, COLORREF(0x111111));
   auto penOld = ::SelectObject(dc, static_cast<HPEN>(penLine));

   ::Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);

   int cellw = (rc.right - rc.left) / 3;
   int cellh = (rc.bottom - rc.top) / 3;

   for(int i = 0; i < 2; ++i)
   {
      ::MoveToEx(dc, rc.left, rc.top + (i+1)*cellh, nullptr);
      ::LineTo(dc, rc.right, rc.top + (i+1)*cellh);
   }

   for(int j = 0; j < 2; ++j)
   {
      ::MoveToEx(dc, rc.left + (j+1)*cellw, rc.top, nullptr);
      ::LineTo(dc, rc.left + (j+1)*cellw, rc.bottom);
   }

   ::SelectObject(dc, penOld);
}

void TicTacToeWindow::DrawMarks(HDC dc, RECT rc)
{
   auto status = game.get_status();

   int cellw = (rc.right - rc.left) / 3;
   int cellh = (rc.bottom - rc.top) / 3;

   for(int i = 0; i < 3; ++i)
   {
      for(int j = 0; j < 3; j++)
      {
         HANDLE hbmp = nullptr;
         int width = 0;
         int height = 0;

         switch(status[i*3+j])
         {
         case 1:
            hbmp = hBmpX;
            width = bmpX.bmWidth;
            height = bmpX.bmHeight;
            break;
         case 2:
            hbmp = hBmp0;
            width = bmp0.bmWidth;
            height = bmp0.bmHeight;
            break;
         }

         auto memdc2 = ::CreateCompatibleDC(dc);
         auto bmpOld2 = ::SelectObject(memdc2, hbmp);

         if(hbmp != nullptr)
            ::BitBlt(
            dc, 
            rc.left + j * cellw + (cellw - width) / 2,
            rc.top + i * cellh + (cellh - height) / 2,
            width,
            height,
            memdc2,
            0,
            0,
            SRCCOPY);

         ::SelectObject(memdc2, bmpOld2);
         ::DeleteDC(memdc2);
      }
   }
}

void TicTacToeWindow::DrawCut(HDC dc, RECT rc)
{
   if(game.is_finished())
   {
      auto streak = game.get_winning_line();

      if(streak.first.is_valid() && streak.second.is_valid())
      {
         int cellw = (rc.right - rc.left) / 3;
         int cellh = (rc.bottom - rc.top) / 3;

         auto penLine = ::CreatePen(PS_SOLID, 15, COLORREF(0x2222ff));
         auto penOld = ::SelectObject(dc, static_cast<HPEN>(penLine));

         ::MoveToEx(
            dc, 
            rc.left + streak.first.col * cellw + cellw/2, 
            rc.top + streak.first.row * cellh + cellh/2,
            nullptr);

         ::LineTo(dc,
            rc.left + streak.second.col * cellw + cellw/2,
            rc.top + streak.second.row * cellh + cellh/2);

         ::SelectObject(dc, penOld);
      }
   }
}

void TicTacToeWindow::OnPaint(DeviceContext* dc)
{
   RECT rcClient;
   ::GetClientRect(hWnd, &rcClient);

   auto memdc = ::CreateCompatibleDC(*dc);
   auto membmp = ::CreateCompatibleBitmap(*dc, rcClient.right - rcClient.left, rcClient.bottom-rcClient.top);
   auto bmpOld = ::SelectObject(memdc, membmp);

   DrawBackground(memdc, rcClient);

   DrawGrid(memdc, rcClient);

   DrawMarks(memdc, rcClient);

   DrawCut(memdc, rcClient);

   ::BitBlt(*dc, 
      rcClient.left, 
      rcClient.top, 
      rcClient.right - rcClient.left, 
      rcClient.bottom-rcClient.top,
      memdc, 
      0, 
      0, 
      SRCCOPY);

   ::SelectObject(memdc, bmpOld);
   ::DeleteObject(membmp);
   ::DeleteDC(memdc);
}