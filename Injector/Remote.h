// remote.h : works with remote memory
//


template <typename DataType>
DataType ReadRemoteDataType(HANDLE hProcess, DataType* pValue) {
	DataType value = 0;
	SIZE_T bytesToRead = sizeof(DataType);
	SIZE_T bytesRead;

	BOOL res = ReadProcessMemory(hProcess, pValue, &value, bytesToRead, &bytesRead);
	assert(0 != res);
	assert(bytesRead == bytesToRead);
	return value;
}

template <typename DataType>
void CopyRemoteDataType(HANDLE hProcess, DataType* pValue, DataType* pLocal) {
	SIZE_T bytesToRead = sizeof(DataType);
	SIZE_T bytesRead;

	BOOL res = ReadProcessMemory(hProcess, pValue, pLocal, bytesToRead, &bytesRead);
	assert(0 != res);
	assert(bytesRead == bytesToRead);
	return;
}

template <typename DataType>
void WriteRemoteDataType(HANDLE hProcess, DataType* pValue, DataType value) {
	SIZE_T bytesToWrite = sizeof(DataType);
	SIZE_T bytesWritten;

	BOOL res = WriteProcessMemory(hProcess, pValue, &value, bytesToWrite, &bytesWritten);
	assert(0 != res);
	assert(bytesWritten == bytesToWrite);
	return;
}

#pragma pack(push, 1)
template <typename T>
class REMOTE_WRAPPER
{
	T* pLocalData = (T*)malloc(sizeof(T));

public:
	T * operator()(HANDLE hProcess, T* pRemoteData)
	{
		CopyRemoteDataType(hProcess, pRemoteData, pLocalData);
		return pLocalData;
	}

	~REMOTE_WRAPPER()
	{
		free(pLocalData);
	}

	static REMOTE_WRAPPER RemoteFactory()
	{
		//TODO !!! this leaks, create singleton
		return *(new REMOTE_WRAPPER<T>);
	}
};
#pragma pack(pop)

//used hProcess variable implicitly
#define REMOTE(TYPE, p) (REMOTE_WRAPPER<TYPE>::RemoteFactory()(hProcess, ((TYPE*)p)))