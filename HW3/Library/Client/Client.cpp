#include <iostream>
#include <Windows.h>
#include <string>
#pragma comment(lib, "Library.lib")

extern "C" {
	int PalindromeCounter(const char* text);
}

int wmain(int argc, wchar_t *argv[]) {
	HANDLE file = CreateFileW(argv[1], GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	LARGE_INTEGER size;

	GetFileSizeEx(file, &size);

	char* text = new char[size.LowPart + 1];

	DWORD len = 0;
	ReadFile(file, text, size.LowPart + 1, &len, 0);

	text[size.LowPart] = 0;

	int counter = PalindromeCounter(text);
	MessageBox(NULL, std::to_wstring(counter).c_str(), L"A number of palindrome", MB_OK);


	CloseHandle(file);
	free(text);

	return 0;
}