#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>

HANDLE max_create_handle(void) {
	HANDLE event = CreateEvent(
		NULL,               // дескриптор безопасности
		TRUE,               // Ручной сброс - да, автоматический сброс - нет
		TRUE,              // начальное состояние объекта signaled - да, nonsignaled - нет
		NULL);               // имя объекта
	DWORD size = 0;
	HANDLE handleEvent;
	for (;;) {
		BOOL success = DuplicateHandle(
			GetCurrentProcess(), //дискриптор процессора
			event, // открытый дескриптор объекта, который является допустимым
			GetCurrentProcess(), //Указатель на переменную, которая получает дубликат
			&handleEvent, // Указатель на переменную, которая получает дубликат
			0, // доступ нового хендла(флаг)
			FALSE, //наседование дискриптора 
			DUPLICATE_SAME_ACCESS); //Игнорирует 5 параметр. Дубликат хендл имеет такой же доступ, как источник хендл.
		if (success) {
			size += 1;
		}
		else {
			_tprintf(TEXT("Error code: %d \n"), GetLastError());
			_tprintf(TEXT("Count handles: %d\n"), size);
			break;
		}
	}
	return event;
}