#pragma once

#include <Windows.h>

ULONG_PTR RvaToVa(const ULONG_PTR base, const ULONG_PTR offset)
{
  return base + offset;
}

template <class T>
DWORD ReadRemote(
  _In_ HANDLE hProcess,
  _In_ ULONG_PTR offset,
  _Out_ T& value)
{
  SIZE_T readLen = 0;
  ReadProcessMemory(hProcess, (LPCVOID)offset, &value, sizeof(T), &readLen);
  return 0;
}

template <class T>
DWORD ReadRemote(
  _In_ HANDLE hProcess,
  _In_ ULONG_PTR offset,
  _Out_ T* value,
  _Out_ DWORD& amount)
{
  ULONG_PTR p = offset;
  DWORD counter = 0;
  T zero = {};

  for (;;)
  {
    T current;

    ReadRemote<T>(hProcess, p, current);
    value[counter] = current;
    counter++;

    if (0 != amount && counter == amount) break;

    p += sizeof(T);
    if (0 == amount && 0 == memcmp(&current, &zero, sizeof(T))) break;
  }

  counter--;
  amount = counter;
  return 0;
}



template <class T>
DWORD WriteRemote(
  _In_ HANDLE hProcess,
  _In_ ULONG_PTR offset,
  _Out_ T& value)
{
  SIZE_T readLen = 0;
  WriteProcessMemory(hProcess, (LPVOID)offset, &value, sizeof(T), &readLen);
  return 0;
}