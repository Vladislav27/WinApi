#include <iostream>
#include <Windows.h>
#include <string>

typedef int(*pal_counter) (const char*);

int wmain(int argc, wchar_t *argv[]) {
	void* file = CreateFileW(argv[1], GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	LARGE_INTEGER size;

	GetFileSizeEx(file, &size);

	char* text = new char[size.LowPart + 1];

	DWORD len = 0;
	ReadFile(file, text, size.LowPart + 1, &len, 0);

	text[size.LowPart] = 0;
	
	HINSTANCE dll = LoadLibrary(TEXT("Library2.dll"));
	pal_counter PalindromeCounter = reinterpret_cast<pal_counter>(GetProcAddress(dll, "PalindromeCounter"));

	int counter = PalindromeCounter(text);
	MessageBox(NULL, std::to_wstring(counter).c_str(), L"A namber of palindrome", MB_OK);

	FreeLibrary(dll);
	CloseHandle(file);
	free(text);
	return 0;
}