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
DataType* ReadRemoteDataType(const HANDLE hProcess, const ULONG_PTR pRemoteValue, DataType* pLocalValue) {
	const auto bytes_to_read = sizeof(DataType);
	SIZE_T bytes_read;

	if (pRemoteValue == 0) return nullptr;

	const BOOL res = ReadProcessMemory(hProcess, PVOID(pRemoteValue), pLocalValue, bytes_to_read, &bytes_read);
	assert(0 != res);
	assert(bytes_read == bytes_to_read);
	return pLocalValue;
}

template <typename DataType>
void WriteRemoteDataType(const HANDLE hProcess, const ULONG_PTR pRemoteValue, DataType* pLocalValue) {
	const auto bytes_to_write = sizeof(DataType);
	SIZE_T bytes_written;

	const BOOL res = WriteProcessMemory(hProcess, PVOID(pRemoteValue), pLocalValue, bytes_to_write, &bytes_written);
	assert(0 != res);	
	assert(bytes_written == bytes_to_write);
	return;
}

template <typename DataType>
DataType* CopyRemoteDataType(const HANDLE hProcess, const ULONG_PTR pRemoteValue) {
	const auto bytes_to_read = sizeof(DataType);
	SIZE_T bytes_read;

	if (pRemoteValue == 0) return nullptr;

	DataType* p_local_value = static_cast<DataType*>(malloc(bytes_to_read));
	const bool res = ReadProcessMemory(hProcess, PVOID(pRemoteValue), p_local_value, bytes_to_read, &bytes_read);
	assert(0 != res);
	assert(bytes_read == bytes_to_read);
	return p_local_value;
}

template <typename DataType>
DataType* CopyRemoteArrayFixedLength(const HANDLE hProcess, const ULONG_PTR pRemoteValue, const DWORD nElements) {
	auto bytes_to_read = sizeof(DataType)*nElements;
	SIZE_T bytes_read;

	if (pRemoteValue == 0) return nullptr;

	DataType* p_local_value = static_cast<DataType*>(malloc(bytes_to_read));
	const bool res = ReadProcessMemory(hProcess, PVOID(pRemoteValue), p_local_value, bytes_to_read, &bytes_read);
	assert(0 != res);
	assert(bytes_read == bytes_to_read);
	return p_local_value;
}

//reads not more than 1OK of elements (to handle possible errors if bad pointer)
//nElements counts zeroed element
template <typename DataType>
DataType* CopyRemoteArrayZeroEnded(const HANDLE hProcess, const ULONG_PTR pRemoteValue, /*out opt*/ DWORD* pnElements) {
	auto p_remote_last_element = pRemoteValue;
	DataType zero_element;
	memset(&zero_element, 0, sizeof(DataType));
	DataType curr_element; //placeholder

	if (pRemoteValue == 0)
	{
		if (nullptr != pnElements) *pnElements = 0;
		return nullptr;
	}

	for (;;)
	{
		//we use ReadRemoteDataType into &CurrElement to reduce need to malloc/free it
		DataType* p_curr_element = ReadRemoteDataType<DataType>(hProcess, p_remote_last_element, &curr_element);
		p_remote_last_element += sizeof(DataType);
		if (0 == memcmp(p_curr_element, &zero_element, sizeof(DataType))) break;
	}

	const auto sz_elements = (p_remote_last_element - pRemoteValue) / sizeof(DataType);
	assert(sz_elements < 10000);
	const DWORD n_elements = DWORD(sz_elements);

	DataType* p_local_value = CopyRemoteArrayFixedLength<DataType>(hProcess, pRemoteValue, n_elements);
	if (nullptr != pnElements) *pnElements = n_elements;
	return p_local_value;
}

//using hProcess variable implicitly
#define REMOTE(TYPE, p) (CopyRemoteDataType<TYPE>(h_process, (p)))
#define REMOTE_ARRAY_FIXED(TYPE, p, n) (CopyRemoteArrayFixedLength<TYPE>(h_process, (p), (n)))
#define REMOTE_ARRAY_ZEROENDED(TYPE, p, n) (CopyRemoteArrayZeroEnded<TYPE>(h_process, (p), &(n)))
#define REMOTE_ARRAY_ZEROENDED_NOLEN(TYPE, p) (CopyRemoteArrayZeroEnded<TYPE>(h_process, (p), nullptr))

//using pRemoteImageBase variable implicitly
#define RVA_TO_REMOTE_VA(p_type, offset) \
 ((offset) ? (ULONG_PTR)(RVA_TO_VA(p_type, p_remote_image_base, offset)) : 0)

//use this in function prototype
#define REMOTE_ARGS_DEFS HANDLE h_process, ULONG_PTR p_remote_image_base
//use this in function call
#define REMOTE_ARGS_CALL h_process, p_remote_image_base