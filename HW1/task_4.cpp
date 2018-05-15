#include "task_4.h"

void CreatePens() {
	DWORD i;
	HPEN pen;
	for (i = 0; ; i++) {
		pen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 255, 255));
		if (pen == NULL) {
			printf("CreatePen failed (%d)\n", GetLastError());
			printf("Create %d pen\n", i+1);
			break;
		}
	}
}