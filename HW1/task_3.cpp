#include "task_3.h"

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
			GetCurrentProcess(),
			event,
			GetCurrentProcess(),
			&handleEvent,
			0,
			FALSE,
			DUPLICATE_SAME_ACCESS);
		if (success) {
			size += 1;
		} else {
			printf("Count handles: %d\n", size);
			break;
		}
	}
	return event;
}