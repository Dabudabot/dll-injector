
#include "pch.h"

#define MY_DLL_EXPORTS

#include "header.h"
#include <iostream>

int my_function()
{
  std::cout << "my function call" << std::endl;
  return 42;
}