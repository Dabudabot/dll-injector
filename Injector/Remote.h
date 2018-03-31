// remote.h : works with remote memory
//

#pragma once

/*
* pRemote pointers are expected to be ULONG_PTR to disable their dereference
* we incorporate malloc into Copy... functions to eliminate the need of smart pointers
*  just use free() on returned pLocal pointers when you don't need them anymore
*/

//this function is the same as CopyRemoteDataType but receive storage for value in order to reduce amount of mallocs
template <typename DataType>
DataType* ReadRemoteDataType(HANDLE hProcess, ULONG_PTR pRemoteValue, DataType* pLocalValue) {
	SIZE_T bytesToRead = sizeof(DataType);
	SIZE_T bytesRead;

	if (pRemoteValue == 0) return nullptr;

	BOOL res = ReadProcessMemory(hProcess, (PVOID)pRemoteValue, pLocalValue, bytesToRead, &bytesRead);
	assert(0 != res);
	assert(bytesRead == bytesToRead);
	return pLocalValue;
}

template <typename DataType>
void WriteRemoteDataType(HANDLE hProcess, ULONG_PTR pRemoteValue, DataType* pLocalValue) {
	SIZE_T bytesToWrite = sizeof(DataType);
	SIZE_T bytesWritten;

	BOOL res = WriteProcessMemory(hProcess, (PVOID)pRemoteValue, pLocalValue, bytesToWrite, &bytesWritten);
	assert(0 != res);
	assert(bytesWritten == bytesToWrite);
	return;
}

template <typename DataType>
DataType* CopyRemoteDataType(HANDLE hProcess, ULONG_PTR pRemoteValue) {
	SIZE_T bytesToRead = sizeof(DataType);
	SIZE_T bytesRead;

	if (pRemoteValue == 0) return nullptr;

	DataType* pLocalValue = (DataType*)malloc(bytesToRead);
	BOOL res = ReadProcessMemory(hProcess, (PVOID)pRemoteValue, pLocalValue, bytesToRead, &bytesRead);
	assert(0 != res);
	assert(bytesRead == bytesToRead);
	return pLocalValue;
}

template <typename DataType>
DataType* CopyRemoteArrayFixedLength(HANDLE hProcess, ULONG_PTR pRemoteValue, DWORD nElements) {
	SIZE_T bytesToRead = sizeof(DataType)*nElements;
	SIZE_T bytesRead;

	if (pRemoteValue == 0) return nullptr;

	DataType* pLocalValue = (DataType*)malloc(bytesToRead);
	BOOL res = ReadProcessMemory(hProcess, (PVOID)pRemoteValue, pLocalValue, bytesToRead, &bytesRead);
	assert(0 != res);
	assert(bytesRead == bytesToRead);
	return pLocalValue;
}

//reads not more than 1OK of elements (to handle possible errors if bad pointer)
//nElements counts zeroed element
template <typename DataType>
DataType* CopyRemoteArrayZeroEnded(HANDLE hProcess, ULONG_PTR pRemoteValue, /*out opt*/ DWORD* pnElements) {
	ULONG_PTR pRemoteLastElement = pRemoteValue;
	DataType ZeroElement;
	memset(&ZeroElement, 0, sizeof(DataType));
	DataType CurrElement; //placeholder

	if (pRemoteValue == 0)
	{
		if (nullptr != pnElements) *pnElements = 0;
		return nullptr;
	}

	for (;;)
	{
		//we use ReadRemoteDataType into &CurrElement to reduce need to malloc/free it
		DataType* pCurrElement = ReadRemoteDataType<DataType>(hProcess, pRemoteLastElement, &CurrElement);
		pRemoteLastElement += sizeof(DataType);
		if (0 == memcmp(pCurrElement, &ZeroElement, sizeof(DataType))) break;
	}

	SIZE_T szElements = (pRemoteLastElement - pRemoteValue) / sizeof(DataType);
	assert(szElements < 10000);
	DWORD nElements = (DWORD)szElements;

	DataType* pLocalValue = CopyRemoteArrayFixedLength<DataType>(hProcess, pRemoteValue, nElements);
	if (nullptr != pnElements) *pnElements = nElements;
	return pLocalValue;
}

//using hProcess variable implicitly
#define REMOTE(TYPE, p) (CopyRemoteDataType<TYPE>(hProcess, (p)))
#define REMOTE_ARRAY_FIXED(TYPE, p, n) (CopyRemoteArrayFixedLength<TYPE>(hProcess, (p), (n)))
#define REMOTE_ARRAY_ZEROENDED(TYPE, p, n) (CopyRemoteArrayZeroEnded<TYPE>(hProcess, (p), &(n)))
#define REMOTE_ARRAY_ZEROENDED_NOLEN(TYPE, p) (CopyRemoteArrayZeroEnded<TYPE>(hProcess, (p), nullptr))

//using pRemoteImageBase variable implicitly
#define RVA_TO_REMOTE_VA(ptype, offset) \
 ((offset) ? (ULONG_PTR)(RVA_TO_VA(ptype, pRemoteImageBase, offset)) : 0)

//use this in function prototype
#define REMOTE_ARGS_DEFS HANDLE hProcess, ULONG_PTR pRemoteImageBase
//use this in function call
#define REMOTE_ARGS_CALL hProcess, pRemoteImageBase