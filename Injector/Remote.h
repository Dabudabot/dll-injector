// remote.h : works with remote memory
//

#pragma once

/*
* pRemote pointers are expected to be ULONG_PTR to disable their dereference
* we incorporate malloc into Copy... functions to eliminate the need of smart pointers
*  just use free() on returned pLocal pointers when you don't need them anymore
*/
class Remote
{
public:
	//this function is the same as CopyRemoteDataType but receive storage 
	//for value in order to reduce amount of mallocs
	template <typename DataType>
	static void readRemoteDataType(
		_In_ const HANDLE hProcess, 
		_In_ const ULONG_PTR pRemoteValue, 
		_Out_ DataType* pLocalValue) {

		const auto bytesToRead = sizeof(DataType);
		SIZE_T bytesRead;

		if (pRemoteValue == 0) return;

		const BOOL res = ReadProcessMemory(
			hProcess,
			PVOID(pRemoteValue), 
			pLocalValue, 
			bytesToRead,
			&bytesRead);

		assert(0 != res);
		assert(bytesRead == bytesToRead);
	}

	template <typename DataType>
	static void writeRemoteDataType(
		_In_ const HANDLE hProcess, 
		_In_ const ULONG_PTR pRemoteValue, 
		_Out_ DataType* pLocalValue) {

		const auto byteToWrite = sizeof(DataType);
		SIZE_T bytesWritten;

		const BOOL res = WriteProcessMemory(
			hProcess, 
			PVOID(pRemoteValue), 
			pLocalValue, 
			byteToWrite,
			&bytesWritten);

		assert(0 != res);
		assert(bytesWritten == byteToWrite);
	}

	template <typename DataType>
	static DataType* copyRemoteDataType(
		_In_ const HANDLE hProcess, 
		_In_ const ULONG_PTR pRemoteValue) {

		const auto bytesToRead = sizeof(DataType);
		SIZE_T bytesRead;

		if (pRemoteValue == 0) return nullptr;

		DataType* pLocalValue = static_cast<DataType*>(malloc(bytesToRead));
		const bool res = ReadProcessMemory(
			hProcess, 
			PVOID(pRemoteValue), 
			pLocalValue,
			bytesToRead,
			&bytesRead);

		assert(0 != res);
		assert(bytesRead == bytesToRead);
		return pLocalValue;
	}

	template <typename DataType>
	static DataType* copyRemoteArrayFixedLength(
		_In_ const HANDLE hProcess, 
		_In_ const ULONG_PTR pRemoteValue,
		_In_ const DWORD nElements) {

		auto bytesToRead = sizeof(DataType)*nElements;
		SIZE_T bytesRead;

		if (pRemoteValue == 0) return nullptr;

		DataType* pLocalValue = static_cast<DataType*>(malloc(bytesToRead));
		const bool res = ReadProcessMemory(
			hProcess, 
			PVOID(pRemoteValue),
			pLocalValue,
			bytesToRead,
			&bytesRead);

		assert(0 != res);
		assert(bytesRead == bytesToRead);
		return pLocalValue;
	}

	//reads not more than 1OK of elements (to handle possible errors if bad pointer)
	//nElements counts zeroed element
	template <typename DataType>
	static DataType* copyRemoteArrayZeroEnded(
		_In_ const HANDLE hProcess,
		_In_ const ULONG_PTR pRemoteValue,
		_Out_opt_ DWORD* pnElements) {

		auto pRemoteLastElement = pRemoteValue;
		DataType zeroElement;
		memset(&zeroElement, 0, sizeof(DataType));
		DataType currElement; //placeholder

		if (pRemoteValue == 0)
		{
			if (nullptr != pnElements) *pnElements = 0;
			return nullptr;
		}

		for (;;)
		{
			//we use ReadRemoteDataType into &CurrElement to reduce need to malloc/free it
			DataType* pCurrElement = readRemoteDataType<DataType>(
				hProcess, 
				pRemoteLastElement,
				&currElement);

			pRemoteLastElement += sizeof(DataType);
			if (0 == memcmp(pCurrElement, &zeroElement, sizeof(DataType))) break;
		}

		const auto szElements = (pRemoteLastElement - pRemoteValue) / sizeof(DataType);
		assert(szElements < 10000);
		const DWORD nElements = DWORD(szElements);

		DataType* pLocalValue = copyRemoteArrayFixedLength<DataType>(hProcess, pRemoteValue, nElements);
		if (nullptr != pnElements) *pnElements = nElements;
		return pLocalValue;
	}
};