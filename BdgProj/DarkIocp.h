#pragma once



#include <windows.h>


class C_IOCP
{
private:
	HANDLE hIoCompletionPort;

public:
	explicit C_IOCP(long maxConcurrency);
	~C_IOCP();

	void AssociateDevice(HANDLE hDevice, ULONG_PTR completionKey);
	void PostStatus(ULONG_PTR completionKey, DWORD dwNumBytes = 0, OVERLAPPED *pOverlapped = 0);
	bool GetStatus(ULONG_PTR *pCompletionKey, PDWORD pdwNumBytes, OVERLAPPED **ppOverlapped, DWORD dwMilliseconds = INFINITE);
};

template <typename ObjectType>
class C_IOCP_QUEUE
{
private:
	HANDLE hIoCompletionPort;

public:
	explicit C_IOCP_QUEUE()
	{
		hIoCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	}
	virtual ~C_IOCP_QUEUE()
	{
		CloseHandle(hIoCompletionPort);
	}

	BOOL push(ObjectType pMsg) 
	{
		return ::PostQueuedCompletionStatus(hIoCompletionPort, 0, (ULONG_PTR)pMsg, NULL);
	}
	ObjectType pop(int Wait = INFINITE)
	{
		ULONG nBytesTransferred = 0;
		ULONG_PTR nCompletionKey = 0;
		LPOVERLAPPED pOverlapped = NULL;

		if(FALSE != ::GetQueuedCompletionStatus(hIoCompletionPort, &nBytesTransferred, &nCompletionKey, &pOverlapped, Wait ? Wait : INFINITE))
		{
			return (ObjectType)(nCompletionKey);
		}
		return(NULL);
	}
};
