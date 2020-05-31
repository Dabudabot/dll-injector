// header.h - Example header
#pragma once

#ifdef MY_DLL_EXPORTS
#define MY_DLL_API __declspec(dllexport)
#else
#define MY_DLL_API __declspec(dllimport)
#endif

// just test function
extern "C" MY_DLL_API int my_function();