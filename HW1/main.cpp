//#include "task_1.h"
//#include "task_2.h"
//#include "task_3.h"
#include "task_4.h"

/*Задание №1*/
/*
void main() {
	int stop_print;
	max_allocating_memory();
	scanf_s("%d", &stop_print);
}
*/
/*Задание №2*/
/*
void main() {
	int stop_print;
	random_allocating_memory();
	scanf_s("%d", &stop_print);
}
*/

/*Задание №3*/
/*
void main() {
	int stop_print;
	HANDLE event = max_create_handle();
	scanf_s("%d", &stop_print);
	CloseHandle(event);
}
*/

/*Задание №4*/
/*
void main() {
	int stop_print;
	create_pens();
	scanf_s("%d", &stop_print);
}
*/

/*Задание №5*/
/*
int counter = 0;
int SIZE_STACK = 1024 * 1024;

void f() {
	counter++;
	f();
}

void main() {
	int stop_print;
	__try {
		f();
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		std::cout << SIZE_STACK / counter << std::endl;
	}
	scanf_s("%d", &stop_print);
}
// Ответ можно посмотреть через программу VMMap. Ответ: 
//    Private Data: 2180 К; Working Set: 2480 K
*/

/*Задание №6*/

int counter = 0;
int SIZE_STACK = 1024 * 1024;
void f() {
	int a = 100;
	int b = 200;
	int c = 300;
	counter++;
	f();
}

void main() {
	int stop_print;
	__try {
		f();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		std::cout << SIZE_STACK / counter << std::endl;
	}
	scanf_s("%d", &stop_print);
}
// Ответ можно посмотреть через программу VMMap. Ответ: 
//    Private Data: 2180 К; Working Set: 2508 K


/*Задание №8*/
// Метод руками: 247
// Фактически: 32767 (пруф: https://habrahabr.ru/post/307186/ )