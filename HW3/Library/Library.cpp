#include "Library.h"

int isPalindrom(const char* text, int left, int right) {
	while (left <= right) {
		if (text[left] != text[right]) {
			return 0;
		}
		left++;
		right--;
	}
	return 1;
}

int PalindromeCounter(const char* text) {
	int i = 0;
	int left = -1;
	int right = -1;
	int counter = 0;
	int size = strlen(text);

	while (i < size) {
		if (text[i] == ' ') {
			if (left == right) {
				i++; 
			}
			else {
				right = i;
				counter += isPalindrom(text, left, right - 1);
				left = right;
				i++;
			}
		}
		else {
			if (left == right) {
				left = i; 
				i++;
			}
			else {
				i++; 
			}
		}
	}
	if (left != right) {
		right = i;
		counter += isPalindrom(text, left, right - 1);
		left = right;
	}
	return counter;
}