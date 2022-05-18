#include "stdafx.h"
#include "DarkIocp.h"



C_IOCP::C_IOCP(long maxConcurrency) : hIoCompletionPort(::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, maxConcurrency))
{
	if (hIoCompletionPort == 0)
	{
		//throw C_EXCEPTION(_T("C_IOCP::C_IOCP() - CreateIoCompletionPort"));
	}
}

C_IOCP::~C_IOCP() 
{ 
	::CloseHandle(hIoCompletionPort);
}

void C_IOCP::AssociateDevice(HANDLE hDevice, ULONG_PTR completionKey) 
{
	if (hIoCompletionPort != ::CreateIoCompletionPort(hDevice, hIoCompletionPort, completionKey, 0))
	{
		//throw C_EXCEPTION(_T("C_IOCP::AssociateDevice() - CreateIoCompletionPort"));
	}
}

void C_IOCP::PostStatus(ULONG_PTR completionKey, DWORD dwNumBytes, OVERLAPPED *pOverlapped) 
{
	if (0 == ::PostQueuedCompletionStatus(hIoCompletionPort, dwNumBytes, completionKey, pOverlapped))
	{
		//throw C_EXCEPTION(_T("C_IOCP::PostStatus() - PostQueuedCompletionStatus"));
	}
}

bool C_IOCP::GetStatus(ULONG_PTR *pCompletionKey, PDWORD pdwNumBytes, OVERLAPPED **ppOverlapped, DWORD dwMilliseconds)
{
	if (FALSE == ::GetQueuedCompletionStatus(hIoCompletionPort, pdwNumBytes, pCompletionKey, ppOverlapped, dwMilliseconds))
	{
		if (WAIT_TIMEOUT != GetLastError())
		{
			//throw C_EXCEPTION(_T("C_IOCP::GetStatus() - GetQueuedCompletionStatus"));
		}
		return(false);
	}
	return(true);
}
