#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>

void create_pens(void) {
	DWORD counter = 0;
	HPEN pen;
	for (;;) {
		counter += 1;
		pen = CreatePen(
			PS_DASH,  //Стиль пера
			1,         //Ширина пера в логических единицах
			RGB(100, 100, 100) //цвет пера
		);
		if (pen == NULL) {
			_tprintf(TEXT("Error code: %d \n"), GetLastError());
			_tprintf(TEXT("Count pens: %d. \n"), counter);
			break;
		}
	}
}