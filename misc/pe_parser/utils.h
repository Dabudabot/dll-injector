#pragma once

#include "stdafx.h"

template <typename Type>
Type rvaToVa(const DWORD_PTR base, const DWORD offset)
{
	return Type(base + offset);
}