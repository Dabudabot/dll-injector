// remote.h : works with remote memory
//

//pRemote pointers are expected to be ulong_ptr to disable their dereference
//we incorporate malloc into copy functionas to eliminate the need of smart pointers
//	just use free on returned plocal pointers when you don`t need them anymore

#ifndef REMOTE_H
#define REMOTE_H

template <typename DataType>
DataType ReadRemoteDataType(const HANDLE hProcess, const ULONG_PTR pRemoteValue) {
	DataType value = 0;
	const SIZE_T bytes_to_read = sizeof(DataType);
	SIZE_T bytes_read;

	const BOOL res = ReadProcessMemory(hProcess, PVOID(pRemoteValue), &value, bytes_to_read, &bytes_read);
	assert(0 != res);
	assert(bytes_read == bytes_to_read);
	return value;
}
template <typename DataType>
void WriteRemoteDataType(const HANDLE hProcess, const ULONG_PTR pRemoteValue, DataType* pLocalValue) {
	const SIZE_T bytes_to_write = sizeof(DataType);
	SIZE_T bytes_written;

	const BOOL res = WriteProcessMemory(hProcess, PVOID(pRemoteValue), pLocalValue, bytes_to_write, &bytes_written);
	assert(0 != res);
	assert(bytes_written == bytes_to_write);
}
template <typename DataType>
DataType* CopyRemoteDataType(const HANDLE hProcess, const ULONG_PTR pRemoteValue) {
	const SIZE_T bytes_to_read = sizeof(DataType);
	SIZE_T bytes_read;

	auto p_local = static_cast<DataType*>(malloc(bytes_to_read * sizeof(DataType)));

	const BOOL res = ReadProcessMemory(hProcess, PVOID(pRemoteValue), p_local, bytes_to_read, &bytes_read);
	assert(0 != res);
	assert(bytes_read == bytes_to_read);
	return p_local;
}

template <typename DataType>
DataType* CopyRemoteArrayFixedLength(const HANDLE hProcess, const ULONG_PTR pRemoteValue, const DWORD nElements) {
	const auto bytes_to_read = sizeof(DataType)*nElements;
	SIZE_T bytes_read;

	auto p_local = static_cast<DataType*>(malloc(bytes_to_read * sizeof(DataType)));

	const BOOL res = ReadProcessMemory(hProcess, PVOID(pRemoteValue), p_local, bytes_to_read, &bytes_read);
	assert(0 != res);
	assert(bytes_read == bytes_to_read);
	return p_local;
}

//reads not more than 1M of elements (to handle possible errors if bad pointer)
//n_elements follows strlen semantics, do not forget to include +1 for zero element
template <typename DataType>
DataType* ReadRemoteArrayZeroEnded(const HANDLE hProcess, const ULONG_PTR pRemoteValue, /*out opt*/ DWORD* pnElements) {
	auto p_remote_last_element = pRemoteValue;
	DataType zero_element;
	memset(&zero_element, 0, sizeof(DataType));
	for (;;)
	{
		auto curr_element = ReadRemoteDataType<DataType>(hProcess, p_remote_last_element);
		if (0 == memcmp(&curr_element, &zero_element, sizeof(DataType))) break;
		p_remote_last_element += sizeof(DataType);
	}

	const auto sz_elements = (p_remote_last_element - pRemoteValue) / sizeof(DataType);
	assert(sz_elements < 10000);
	auto n_elements = DWORD(sz_elements);

	auto p_local = CopyRemoteArrayFixedLength(hProcess, pRemoteValue, p_local, n_elements);
	if (nullptr != pnElements) *pnElements = n_elements;
	return p_local;
}


//used hProcess variable implicitly
#define REMOTE(TYPE, p)	(CopyRemoteDataType<TYPE>(hProcess, (p)))
#define REMOTE_ARRAY_FIXED(TYPE, p, n) (CopyRemoteArrayFixedLength<TYPE>(hProcess, (p), (n)))
#define REMOTE_ARRAY_ZEROENDED(TYPE, p, n) (ReadRemoteArrayZeroEnded<TYPE>(hProcess, (p), &(n)))
#define RVA_TO_VA(ptype, base, offset) ((ptype)(((DWORD_PTR)(base)) + (offset)));
#define RVA_TO_REMOTE_VA(TYPE, base, offset) ((ULONG_PTR)RVA_TO_VA(TYPE, base, offset))
#endif // REMOTE_H
