#pragma once  

#ifdef POLINDROM_EXPORTS
#define POLINDROM_API extern "C" __declspec(dllexport)
#else
#define POLINDROM_API extern "C" __declspec(dllimport)
#endif

#include <string>

POLINDROM_API int PalindromeCounter(const char* text);
