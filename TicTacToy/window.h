#pragma once

#include "framework.h"
#include "..\Common\game.h"

class TicTacToeWindow : public Window
{
   HANDLE hBmp0;
   HANDLE hBmpX;
   BITMAP bmp0;
   BITMAP bmpX;

   tictactoe_game game;

   void DrawBackground(HDC dc, RECT rc);
   void DrawGrid(HDC dc, RECT rc);
   void DrawMarks(HDC dc, RECT rc);
   void DrawCut(HDC dc, RECT rc);

   virtual void OnPaint(DeviceContext* dc) override;
   virtual void OnLeftButtonUp(int x, int y, WPARAM params) override;
   virtual void OnMenuItemClicked(int menuId) override;

public:
   TicTacToeWindow();
   virtual ~TicTacToeWindow() override;
};

