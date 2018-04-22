#pragma once

/**
* \brief remote.h : works with remote memory 
* pRemote pointers are expected to be ULONG_PTR to disable their dereference
* we incorporate malloc into Copy... functions to eliminate the need of smart pointers
* just use free() on returned pLocal pointers when you don't need them anymore
 */
class Remote
{
public:
	/**
	 * \brief Reads data from remote process into variable
	 * this function is the same as CopyRemoteDataType 
	 * but receive storage
	 * for value in order to reduce amount of mallocs
	 * \tparam DataType type of data to read
	 * \param hProcess remote process to read from
	 * \param pRemoteValue addr to read
	 * \param pLocalValue storage variable for readed value
	 */
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

	/**
	 * \brief Writes data to the remote process
	 * \tparam DataType type of data to write
	 * \param hProcess remote process to write to
	 * \param pRemoteValue addr to write
	 * \param pLocalValue what to write
	 */
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

	/**
	 * \brief Copies data from remote process and return as a pointer
	 * DO NOT FORGET TO FREE THIS POINTER!
	 * \tparam DataType type of data to read
	 * \param hProcess remote process to read from
	 * \param pRemoteValue addr to read
	 * \return readed data NEED TO FREE THIS POINTER
	 */
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

	/**
	 * \brief Copies array if data from remote 
	 * process ad return as a pointer
	 * DO NOT FORGET TO FREE THIS POINTER!
	 * \tparam DataType type of data to read
	 * \param hProcess remote process to read from
	 * \param pRemoteValue addr to read
	 * \param nElements number of elements to read
	 * \return readed data NEED TO FREE THIS POINTER
	 */
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

	/**
	 * \brief Copies array if data from remote 
	 * process ad return as a pointer
	 * DO NOT FORGET TO FREE THIS POINTER!
	 * reads not more than 1OK of elements 
	 * (to handle possible errors if bad pointer)
	 * \tparam DataType type of data to read
	 * \param hProcess remote process to read from
	 * \param pRemoteValue addr to read
	 * \param pnElements counts zeroed element
	 * \return 
	 */
	template <typename DataType>
	static DataType* copyRemoteArrayZeroEnded(
		_In_ const HANDLE hProcess,
		_In_ const ULONG_PTR pRemoteValue,
		_Out_opt_ DWORD* pnElements) {

		auto pRemoteLastElement = pRemoteValue;
		DataType zeroElement;
		memset(&zeroElement, 0, sizeof(DataType));
		

		if (pRemoteValue == 0)
		{
			if (nullptr != pnElements) *pnElements = 0;
			return nullptr;
		}

		for (;;)
		{
			DataType currElement; //placeholder
			//we use ReadRemoteDataType into &CurrElement to reduce need to malloc/free it
			readRemoteDataType<DataType>(
				hProcess, 
				pRemoteLastElement, 
				&currElement);

			pRemoteLastElement += sizeof(DataType);
			if (0 == memcmp(&currElement, &zeroElement, sizeof(DataType))) break;
		}

		const auto szElements = (pRemoteLastElement - pRemoteValue) / sizeof(DataType);
		assert(szElements < 10000);
		const DWORD nElements = DWORD(szElements);

		DataType* pLocalValue = copyRemoteArrayFixedLength<DataType>(hProcess, pRemoteValue, nElements);
		if (nullptr != pnElements) *pnElements = nElements;
		return pLocalValue;
	}
};