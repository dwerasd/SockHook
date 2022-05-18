#include "stdafx.h"
#include "sockhook.h"



taccept oaccept;
tbind obind;
tclosesocket oclosesocket;
tconnect oconnect;
tgetpeername ogetpeername;
tgetsockname ogetsockname;
tgetsockopt ogetsockopt;
thtonl ohtonl;
thtons ohtons;
tioctlsocket oioctlsocket;
tinet_addr oinet_addr;
tinet_ntoa oinet_ntoa;
tlisten olisten;
tntohl ontohl;
tntohs ontohs;
trecv orecv;
trecvfrom orecvfrom;
tselect oselect;
tsend osend;
tsendto osendto;
tsetsockopt osetsockopt;
tshutdown oshutdown;
tsocket osocket;
tGetAddrInfoW oGetAddrInfoW;
tGetNameInfoW oGetNameInfoW;
tWSApSetPostRoutine oWSApSetPostRoutine;
tFreeAddrInfoW oFreeAddrInfoW;
tWPUCompleteOverlappedRequest oWPUCompleteOverlappedRequest;
tWSAAccept oWSAAccept;
tWSAAddressToStringA oWSAAddressToStringA;
tWSAAddressToStringW oWSAAddressToStringW;
tWSACloseEvent oWSACloseEvent;
tWSAConnect oWSAConnect;
tWSACreateEvent oWSACreateEvent;
tWSADuplicateSocketA oWSADuplicateSocketA;
tWSADuplicateSocketW oWSADuplicateSocketW;
tWSAEnumNameSpaceProvidersA oWSAEnumNameSpaceProvidersA;
tWSAEnumNameSpaceProvidersW oWSAEnumNameSpaceProvidersW;
tWSAEnumNetworkEvents oWSAEnumNetworkEvents;
tWSAEnumProtocolsA oWSAEnumProtocolsA;
tWSAEnumProtocolsW oWSAEnumProtocolsW;
tWSAEventSelect oWSAEventSelect;
tWSAGetOverlappedResult oWSAGetOverlappedResult;
tWSAGetQOSByName oWSAGetQOSByName;
tWSAGetServiceClassInfoA oWSAGetServiceClassInfoA;
tWSAGetServiceClassInfoW oWSAGetServiceClassInfoW;
tWSAGetServiceClassNameByClassIdA oWSAGetServiceClassNameByClassIdA;
tWSAGetServiceClassNameByClassIdW oWSAGetServiceClassNameByClassIdW;
tWSAHtonl oWSAHtonl;
tWSAHtons oWSAHtons;
tgethostbyaddr ogethostbyaddr;
tgethostbyname ogethostbyname;
tgetprotobyname ogetprotobyname;
tgetprotobynumber ogetprotobynumber;
tgetservbyname ogetservbyname;
tgetservbyport ogetservbyport;
tgethostname ogethostname;
tWSAInstallServiceClassA oWSAInstallServiceClassA;
tWSAInstallServiceClassW oWSAInstallServiceClassW;
tWSAIoctl oWSAIoctl;
tWSAJoinLeaf oWSAJoinLeaf;
tWSALookupServiceBeginA oWSALookupServiceBeginA;
tWSALookupServiceBeginW oWSALookupServiceBeginW;
tWSALookupServiceEnd oWSALookupServiceEnd;
tWSALookupServiceNextA oWSALookupServiceNextA;
tWSALookupServiceNextW oWSALookupServiceNextW;
tWSANSPIoctl oWSANSPIoctl;
tWSANtohl oWSANtohl;
tWSANtohs oWSANtohs;
tWSAProviderConfigChange oWSAProviderConfigChange;
tWSARecv oWSARecv;
tWSARecvDisconnect oWSARecvDisconnect;
tWSARecvFrom oWSARecvFrom;
tWSARemoveServiceClass oWSARemoveServiceClass;
tWSAResetEvent oWSAResetEvent;
tWSASend oWSASend;
tWSASendDisconnect oWSASendDisconnect;
tWSASendTo oWSASendTo;
tWSASetEvent oWSASetEvent;
tWSASetServiceA oWSASetServiceA;
tWSASetServiceW oWSASetServiceW;
tWSASocketA oWSASocketA;
tWSASocketW oWSASocketW;
tWSAStringToAddressA oWSAStringToAddressA;
tWSAStringToAddressW oWSAStringToAddressW;
tWSAWaitForMultipleEvents oWSAWaitForMultipleEvents;
tWSCDeinstallProvider oWSCDeinstallProvider;
tWSCEnableNSProvider oWSCEnableNSProvider;
tWSCEnumProtocols oWSCEnumProtocols;
tWSCGetProviderPath oWSCGetProviderPath;
tWSCInstallNameSpace oWSCInstallNameSpace;
tWSCInstallProvider oWSCInstallProvider;
tWSCUnInstallNameSpace oWSCUnInstallNameSpace;
tWSCUpdateProvider oWSCUpdateProvider;
tWSCWriteNameSpaceOrder oWSCWriteNameSpaceOrder;
tWSCWriteProviderOrder oWSCWriteProviderOrder;
tfreeaddrinfo ofreeaddrinfo;
tgetaddrinfo ogetaddrinfo;
tgetnameinfo ogetnameinfo;
tWSAAsyncSelect oWSAAsyncSelect;
tWSAAsyncGetHostByAddr oWSAAsyncGetHostByAddr;
tWSAAsyncGetHostByName oWSAAsyncGetHostByName;
tWSAAsyncGetProtoByNumber oWSAAsyncGetProtoByNumber;
tWSAAsyncGetProtoByName oWSAAsyncGetProtoByName;
tWSAAsyncGetServByPort oWSAAsyncGetServByPort;
tWSAAsyncGetServByName oWSAAsyncGetServByName;
tWSACancelAsyncRequest oWSACancelAsyncRequest;
tWSASetBlockingHook oWSASetBlockingHook;
tWSAUnhookBlockingHook oWSAUnhookBlockingHook;
tWSAGetLastError oWSAGetLastError;
tWSASetLastError oWSASetLastError;
tWSACancelBlockingCall oWSACancelBlockingCall;
tWSAIsBlocking oWSAIsBlocking;
tWSAStartup oWSAStartup;
tWSACleanup oWSACleanup;
t__WSAFDIsSet o__WSAFDIsSet;
tWEP oWEP;

typedef struct _SOCK_DATA
{
	wchar_t wszTag[1 << 4];
	SOCKET s;
	char szBuf[(1 << 15)];
	int nSize;
	int nFlag;
} SOCK_DATA, *LPSOCK_DATA;


C_IOCP_QUEUE<LPSOCK_DATA> *pReciveQueue = 0;
C_OBJECT_POOL_DLF<SOCK_DATA> *pReciveDataPool = 0;

void __stdcall ReciveLogThread()
{
	pReciveQueue = new C_IOCP_QUEUE<LPSOCK_DATA>();
	pReciveDataPool = new C_OBJECT_POOL_DLF<SOCK_DATA>((ULONG_PTR)1000);
	do 
	{
		LPSOCK_DATA pSockData = pReciveQueue->pop();

		wchar_t wszBuffer[(1 << 15)];
		swprintf_s(wszBuffer, L"[%s/%08x/%04d] ", pSockData->wszTag, pSockData->s, pSockData->nSize);
		for (int i = 0 ; i < pSockData->nSize ; i++ )
		{
			wchar_t wszBuf[4] = { 0 };
			swprintf_s(wszBuf, L"%02x ", *(LPBYTE)(pSockData->szBuf+i) & 0xff);
			if (0 < i && 0 == (i % 256) )
			{
				wcscat(wszBuffer, L"\r\n");
			}
			wcscat(wszBuffer, wszBuf);
		}
		pLog->Write(wszBuffer);
		pReciveDataPool->Deallocate(pSockData);
	} while (TRUE);
}

DWORD DetourMaker(HMODULE hMod, LPSTR pszFunc, LPBYTE NewFunc, DWORD dwSize)
{
	LPBYTE pbytHookAddr = (LPBYTE)GetProcAddress(hMod, pszFunc);
	if ( 0 < pbytHookAddr )
	{
		DWORD dwProtect = 0;
		LPBYTE tmp = (LPBYTE)malloc(dwSize+5);			// 원래코드를 복사할만큼+점프할5바이트 할당
		memcpy(tmp, (LPBYTE)pbytHookAddr, dwSize);			// 원래코드를 복사함
		*(LPBYTE)(tmp+dwSize) = 0xE9;
		*(LPDWORD)(tmp+dwSize+1) = (DWORD)((LPBYTE)pbytHookAddr-tmp)-5;	// 할당받은 함수에서 새로운 함수로 가야함.
		VirtualProtect((LPVOID)pbytHookAddr, 5, PAGE_READWRITE, &dwProtect );
		*(LPBYTE)pbytHookAddr = 0xE9;
		*(LPDWORD)(pbytHookAddr+1) = (DWORD)(NewFunc-pbytHookAddr)-5;				// 원래 함수에서 할당받은 함수로 가야함.
		VirtualProtect((LPVOID)pbytHookAddr, 5, dwProtect, NULL );

		pLog->Write("DetourMaker(): Hook=%s", pszFunc);
		return (DWORD)tmp;
	}
	else
	{
		pLog->Write("DetourMaker(): NotFound=%s", pszFunc);
	}
	return(0);
}

int __stdcall hksend(SOCKET s, const char *buf, int len, int flags)
{
	int nRet = osend(s, buf, len, flags);

	if(0 < nRet)
	{
		LPSOCK_DATA pSockData = pReciveDataPool->Allocate();
		swprintf_s(pSockData->wszTag, L"send");
		pSockData->s = s;
		pSockData->nSize = nRet;
		pSockData->nFlag = flags;
		ZeroMemory(pSockData->szBuf, (1 << 15));
		for (int i = 0 ; i < pSockData->nSize ; i++ )
		{
			pSockData->szBuf[i] = buf[i];
		}
		pReciveQueue->push(pSockData);
	}

	return nRet;
}

int __stdcall hksendto(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen)
{
	int nRet = osendto(s, buf, len, flags, to, tolen);

	if(0 < nRet)
	{
		LPSOCK_DATA pSockData = pReciveDataPool->Allocate();
		swprintf_s(pSockData->wszTag, L"sendto");
		pSockData->s = s;
		pSockData->nSize = nRet;
		pSockData->nFlag = flags;
		ZeroMemory(pSockData->szBuf, (1 << 15));
		for (int i = 0 ; i < pSockData->nSize ; i++ )
		{
			pSockData->szBuf[i] = buf[i];
		}
		pReciveQueue->push(pSockData);
	}

	return nRet;
}

int __stdcall hkrecv(SOCKET s, char *buf, int len, int flags)
{
	int nRet = orecv(s, buf, len, flags);
	if(0 < nRet)
	{
		LPSOCK_DATA pSockData = pReciveDataPool->Allocate();
		swprintf_s(pSockData->wszTag, L"recv");
		pSockData->s = s;
		pSockData->nSize = nRet;
		ZeroMemory(pSockData->szBuf, (1 << 15));
		for (int i = 0 ; i < pSockData->nSize ; i++ )	// 버퍼 복사.
		{
			pSockData->szBuf[i] = buf[i];
		}
		pReciveQueue->push(pSockData);
	}
	return nRet;
}

int __stdcall hkrecvfrom(SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen)
{
	//return orecvfrom(s, buf, len, flags, from, fromlen);

	int nRet = orecvfrom(s, buf, len, flags, from, fromlen);

	if(0 < nRet)
	{
		LPSOCK_DATA pSockData = pReciveDataPool->Allocate();
		swprintf_s(pSockData->wszTag, L"recvfrom");
		pSockData->s = s;
		pSockData->nSize = nRet;
		ZeroMemory(pSockData->szBuf, (1 << 15));
		for (int i = 0 ; i < pSockData->nSize ; i++)
		{
			pSockData->szBuf[i] = buf[i];
		}
		pReciveQueue->push(pSockData);
	}
	return nRet;
}

int __stdcall hkWSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
					 LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	int nRet = oWSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, 
		lpCompletionRoutine);

	if(NOERROR == nRet)
	{
		LPSOCK_DATA pSockData = pReciveDataPool->Allocate();
		swprintf_s(pSockData->wszTag, L"WSASend");
		pSockData->s = s;
		pSockData->nSize = *lpNumberOfBytesSent;
		ZeroMemory(pSockData->szBuf, (1 << 15));
		for (int i = 0 ; i < pSockData->nSize ; i++ )
		{
			pSockData->szBuf[i] = lpBuffers->buf[i];
		}
		pReciveQueue->push(pSockData);
	}

	return nRet;
}

int __stdcall hkWSASendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
					   const struct sockaddr *lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	int nRet = oWSASendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iToLen,
		lpOverlapped, lpCompletionRoutine);

	if(NOERROR == nRet)
	{
		LPSOCK_DATA pSockData = pReciveDataPool->Allocate();
		swprintf_s(pSockData->wszTag, L"WSASendTo");
		pSockData->s = s;
		pSockData->nSize = *lpNumberOfBytesSent;
		ZeroMemory(pSockData->szBuf, (1 << 15));
		for (int i = 0 ; i < pSockData->nSize ; i++ )
		{
			pSockData->szBuf[i] = lpBuffers->buf[i];
		}
		pReciveQueue->push(pSockData);
	}
	return nRet;
}

int __stdcall hkWSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags,
					 LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	int nRet = oWSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);

	if(NOERROR == nRet)
	{
		LPSOCK_DATA pSockData = pReciveDataPool->Allocate();
		swprintf_s(pSockData->wszTag, L"WSARecv");
		pSockData->s = s;
		pSockData->nSize = *lpNumberOfBytesRecvd;
		ZeroMemory(pSockData->szBuf, (1 << 15));
		for (int i = 0 ; i < pSockData->nSize ; i++ )
		{
			pSockData->szBuf[i] = lpBuffers->buf[i];
		}
		pReciveQueue->push(pSockData);
	}

	return nRet;
}

int __stdcall hkWSARecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, struct sockaddr *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	int nRet = oWSARecvFrom(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpFrom, lpFromlen, lpOverlapped, lpCompletionRoutine);

	if(NOERROR == nRet)
	{
		LPSOCK_DATA pSockData = pReciveDataPool->Allocate();
		swprintf_s(pSockData->wszTag, L"WSARecvFrom");
		pSockData->s = s;
		pSockData->nSize = *lpNumberOfBytesRecvd;
		ZeroMemory(pSockData->szBuf, (1 << 15));
		for (int i = 0 ; i < pSockData->nSize ; i++ )
		{
			pSockData->szBuf[i] = lpBuffers->buf[i];
		}
		pReciveQueue->push(pSockData);
	}


	return nRet;
}

SOCKET __stdcall hkaccept(SOCKET s, struct sockaddr *addr, int *addrlen)
{
	return oaccept(s, addr, addrlen);
}

int __stdcall hkbind(SOCKET s, const struct sockaddr *name, int namelen)
{
	return obind(s, name, namelen);
}

int __stdcall hkclosesocket(SOCKET s)
{
	return oclosesocket(s);
}

int __stdcall hkconnect(SOCKET s, const struct sockaddr *name, int namelen)
{
	sockaddr_in *p = (sockaddr_in *)name;
	pLog->Write("hkconnect : %s / %d", inet_ntoa(p->sin_addr), p->sin_port);
	return oconnect(s, name, namelen);
}

int __stdcall hkgetpeername(SOCKET s, struct sockaddr *name, int *namelen)
{
	return ogetpeername(s, name, namelen);
}

int __stdcall hkgetsockname(SOCKET s, struct sockaddr *name, int *namelen)
{
	return ogetsockname(s, name, namelen);
}

int __stdcall hkgetsockopt(SOCKET s, int level, int optname, char *optval, int *optlen)
{
	return ogetsockopt(s, level, optname, optval, optlen);
}

u_long __stdcall hkhtonl(u_long hostlong)
{
	return ohtonl(hostlong);
}

u_short __stdcall hkhtons(u_short hostshort)
{
	return ohtons(hostshort);
}

int __stdcall hkioctlsocket(SOCKET s, long cmd, u_long *argp)
{
	return oioctlsocket(s, cmd, argp);
}

unsigned long __stdcall hkinet_addr(const char *cp)
{
	return oinet_addr(cp);
}

char* __stdcall hkinet_ntoa(struct in_addr in)
{
	return oinet_ntoa(in);
}

int __stdcall hklisten(SOCKET s, int backlog)
{
	return olisten(s, backlog);
}

u_long __stdcall hkntohl(u_long netlong)
{
	return ontohl(netlong);
}

u_short __stdcall hkntohs(u_short netshort)
{
	return ontohs(netshort);
}

int __stdcall hkselect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout)
{
	return oselect(nfds, readfds, writefds, exceptfds, timeout);
}

int __stdcall hksetsockopt(SOCKET s, int level, int optname, const char *optval, int optlen)
{
	return osetsockopt(s, level, optname, optval, optlen);
}

int __stdcall hkshutdown(SOCKET s, int how)
{
	return oshutdown(s, how);
}

SOCKET __stdcall hksocket(int af, int type, int protocol)
{
	return osocket(af, type, protocol);
}

int __stdcall hkGetAddrInfoW(PCWSTR pNodeName, PCWSTR pServiceName, const ADDRINFOW *pHints, PADDRINFOW *ppResult)
{
	return oGetAddrInfoW(pNodeName, pServiceName, pHints, ppResult);
}

int __stdcall hkGetNameInfoW(const SOCKADDR *pSockaddr, socklen_t SockaddrLength, PWCHAR pNodeBuffer, DWORD NodeBufferSize,
						  PWCHAR pServiceBuffer, DWORD ServiceBufferSize, INT Flags)
{
	return oGetNameInfoW(pSockaddr, SockaddrLength, pNodeBuffer, NodeBufferSize, pServiceBuffer, 
		ServiceBufferSize, Flags);
}

INT __stdcall hkWSApSetPostRoutine(LPVOID lpPostRoutine)
{
	return oWSApSetPostRoutine(lpPostRoutine);
}

void __stdcall hkFreeAddrInfoW(PADDRINFOW pAddrInfo)
{
	oFreeAddrInfoW(pAddrInfo);
}

WSAEVENT __stdcall hkWPUCompleteOverlappedRequest(SOCKET s, LPWSAOVERLAPPED lpOverlapped, DWORD dwError, DWORD cbTransferred,
											   LPINT lpErrno)
{
	return oWPUCompleteOverlappedRequest(s, lpOverlapped, dwError, cbTransferred, lpErrno);
}

SOCKET __stdcall hkWSAAccept(SOCKET s, struct sockaddr *addr, LPINT addrlen, LPCONDITIONPROC lpfnCondition, DWORD dwCallbackData)
{
	return oWSAAccept(s, addr, addrlen, lpfnCondition, dwCallbackData);
}

INT __stdcall hkWSAAddressToStringA(LPSOCKADDR lpsaAddress, DWORD dwAddressLength, LPWSAPROTOCOL_INFO lpProtocolInfo,
								 LPTSTR lpszAddressString, LPDWORD lpdwAddressStringLength)
{
	return oWSAAddressToStringA(lpsaAddress, dwAddressLength, lpProtocolInfo, lpszAddressString, 
		lpdwAddressStringLength);
}

INT __stdcall hkWSAAddressToStringW(LPSOCKADDR lpsaAddress, DWORD dwAddressLength, LPWSAPROTOCOL_INFO lpProtocolInfo,
								 LPTSTR lpszAddressString, LPDWORD lpdwAddressStringLength)
{
	return oWSAAddressToStringW(lpsaAddress, dwAddressLength, lpProtocolInfo, lpszAddressString, 
		lpdwAddressStringLength);
}

BOOL __stdcall hkWSACloseEvent(WSAEVENT hEvent)
{
	return oWSACloseEvent(hEvent);
}

int __stdcall hkWSAConnect(SOCKET s, const struct sockaddr *name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData,
						LPQOS lpSQOS, LPQOS lpGQOS)
{
	return oWSAConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS);
}

WSAEVENT __stdcall hkWSACreateEvent(void)
{
	return oWSACreateEvent();
}

int __stdcall hkWSADuplicateSocketA(SOCKET s, DWORD dwProcessId, LPWSAPROTOCOL_INFO lpProtocolInfo)
{
	return oWSADuplicateSocketA(s, dwProcessId, lpProtocolInfo);
}

int __stdcall hkWSADuplicateSocketW(SOCKET s, DWORD dwProcessId, LPWSAPROTOCOL_INFO lpProtocolInfo)
{
	return oWSADuplicateSocketW(s, dwProcessId, lpProtocolInfo);
}

INT __stdcall hkWSAEnumNameSpaceProvidersA(LPDWORD lpdwBufferLength, LPWSANAMESPACE_INFO lpnspBuffer)
{
	return oWSAEnumNameSpaceProvidersA(lpdwBufferLength, lpnspBuffer);
}

INT __stdcall hkWSAEnumNameSpaceProvidersW(LPDWORD lpdwBufferLength, LPWSANAMESPACE_INFO lpnspBuffer)
{
	return oWSAEnumNameSpaceProvidersW(lpdwBufferLength, lpnspBuffer);
}

int __stdcall hkWSAEnumNetworkEvents(SOCKET s, WSAEVENT hEventObject, LPWSANETWORKEVENTS lpNetworkEvents)
{
	return oWSAEnumNetworkEvents(s, hEventObject, lpNetworkEvents);
}

int __stdcall hkWSAEnumProtocolsA(LPINT lpiProtocols, LPWSAPROTOCOL_INFO lpProtocolBuffer, LPDWORD lpdwBufferLength)
{
	return oWSAEnumProtocolsA(lpiProtocols, lpProtocolBuffer, lpdwBufferLength);
}

int __stdcall hkWSAEnumProtocolsW(LPINT lpiProtocols, LPWSAPROTOCOL_INFO lpProtocolBuffer, LPDWORD lpdwBufferLength)
{
	return oWSAEnumProtocolsW(lpiProtocols, lpProtocolBuffer, lpdwBufferLength);
}

int __stdcall hkWSAEventSelect(SOCKET s, WSAEVENT hEventObject, long lNetworkEvents)
{
	return oWSAEventSelect(s, hEventObject, lNetworkEvents);
}

BOOL __stdcall hkWSAGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait,
									 LPDWORD lpdwFlags)
{
	return oWSAGetOverlappedResult(s, lpOverlapped, lpcbTransfer, fWait, lpdwFlags);
}

BOOL __stdcall hkWSAGetQOSByName(SOCKET s, LPWSABUF lpQOSName, LPQOS lpQOS)
{
	return oWSAGetQOSByName(s, lpQOSName, lpQOS);
}

INT __stdcall hkWSAGetServiceClassInfoA(LPGUID lpProviderId, LPGUID lpServiceClassId, LPDWORD lpdwBufferLength,
									 LPWSASERVICECLASSINFO lpServiceClassInfo)
{
	return oWSAGetServiceClassInfoA(lpProviderId, lpServiceClassId, lpdwBufferLength, lpServiceClassInfo);
}

INT __stdcall hkWSAGetServiceClassInfoW(LPGUID lpProviderId, LPGUID lpServiceClassId, LPDWORD lpdwBufferLength,
									 LPWSASERVICECLASSINFO lpServiceClassInfo)
{
	return oWSAGetServiceClassInfoW(lpProviderId, lpServiceClassId, lpdwBufferLength, lpServiceClassInfo);
}

INT __stdcall hkWSAGetServiceClassNameByClassIdA(LPGUID lpServiceClassId, LPTSTR lpszServiceClassName, LPDWORD lpdwBufferLength)
{
	return oWSAGetServiceClassNameByClassIdA(lpServiceClassId, lpszServiceClassName, lpdwBufferLength);
}

INT __stdcall hkWSAGetServiceClassNameByClassIdW(LPGUID lpServiceClassId, LPTSTR lpszServiceClassName, LPDWORD lpdwBufferLength)
{
	return oWSAGetServiceClassNameByClassIdW(lpServiceClassId, lpszServiceClassName, lpdwBufferLength);
}

int __stdcall hkWSAHtonl(SOCKET s, u_long hostlong, u_long *lpnetlong)
{
	return oWSAHtonl(s, hostlong, lpnetlong);
}

int __stdcall hkWSAHtons(SOCKET s, u_short hostshort, u_short *lpnetshort)
{
	return oWSAHtons(s, hostshort, lpnetshort);
}

struct hostent* __stdcall hkgethostbyaddr(const char *addr, int len, int type)
{
	return ogethostbyaddr(addr, len, type);
}

struct hostent* __stdcall hkgethostbyname(const char *name)
{
	return ogethostbyname(name);
}

struct protoent* __stdcall hkgetprotobyname(const char *name)
{
	return ogetprotobyname(name);
}

struct protoent* __stdcall hkgetprotobynumber(int number)
{
	return ogetprotobynumber(number);
}

struct servent* __stdcall hkgetservbyname(const char *name, const char *proto)
{
	return ogetservbyname(name, proto);
}

struct servent* __stdcall hkgetservbyport(int port, const char *proto)
{
	return ogetservbyport(port, proto);
}

int __stdcall hkgethostname(char *name, int namelen)
{
	return ogethostname(name, namelen);
}

INT __stdcall hkWSAInstallServiceClassA(LPWSASERVICECLASSINFO lpServiceClassInfo)
{
	return oWSAInstallServiceClassA(lpServiceClassInfo);
}

INT __stdcall hkWSAInstallServiceClassW(LPWSASERVICECLASSINFO lpServiceClassInfo)
{
	return oWSAInstallServiceClassW(lpServiceClassInfo);
}

int __stdcall hkWSAIoctl(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer,
					  DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	return oWSAIoctl(s, dwIoControlCode, lpvInBuffer, cbInBuffer, lpvOutBuffer, cbOutBuffer, 
		lpcbBytesReturned, lpOverlapped, lpCompletionRoutine);
}

SOCKET __stdcall hkWSAJoinLeaf(SOCKET s, const struct sockaddr *name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData,
							LPQOS lpSQOS, LPQOS lpGQOS, DWORD dwFlags)
{
	return oWSAJoinLeaf(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS, dwFlags);
}

INT __stdcall hkWSALookupServiceBeginA(LPWSAQUERYSET lpqsRestrictions, DWORD dwControlFlags, LPHANDLE lphLookup)
{
	return oWSALookupServiceBeginA(lpqsRestrictions, dwControlFlags, lphLookup);
}

INT __stdcall hkWSALookupServiceBeginW(LPWSAQUERYSET lpqsRestrictions, DWORD dwControlFlags, LPHANDLE lphLookup)
{
	return oWSALookupServiceBeginW(lpqsRestrictions, dwControlFlags, lphLookup);
}

INT __stdcall hkWSALookupServiceEnd(HANDLE hLookup)
{
	return oWSALookupServiceEnd(hLookup);
}

INT __stdcall hkWSALookupServiceNextA(HANDLE hLookup, DWORD dwControlFlags, LPDWORD lpdwBufferLength, LPWSAQUERYSET lpqsResults)
{
	return oWSALookupServiceNextA(hLookup, dwControlFlags, lpdwBufferLength, lpqsResults);
}

INT __stdcall hkWSALookupServiceNextW(HANDLE hLookup, DWORD dwControlFlags, LPDWORD lpdwBufferLength, LPWSAQUERYSET lpqsResults)
{
	return oWSALookupServiceNextW(hLookup, dwControlFlags, lpdwBufferLength, lpqsResults);
}

int __stdcall hkWSANSPIoctl(HANDLE hLookup, DWORD dwControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer,
						 DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSACOMPLETION lpCompletion)
{
	return oWSANSPIoctl(hLookup, dwControlCode, lpvInBuffer, cbInBuffer, lpvOutBuffer, cbOutBuffer, 
		lpcbBytesReturned, lpCompletion);
}

int __stdcall hkWSANtohl(SOCKET s, u_long netlong, u_long *lphostlong)
{
	return oWSANtohl(s, netlong, lphostlong);
}

int __stdcall hkWSANtohs(SOCKET s, u_short netshort, u_short *lphostshort)
{
	return oWSANtohs(s, netshort, lphostshort);
}

int __stdcall hkWSAProviderConfigChange(LPHANDLE lpNotificationHandle, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	return oWSAProviderConfigChange(lpNotificationHandle, lpOverlapped, lpCompletionRoutine);
}

int __stdcall hkWSARecvDisconnect(SOCKET s, LPWSABUF lpInboundDisconnectData)
{
	return oWSARecvDisconnect(s, lpInboundDisconnectData);
}

INT __stdcall hkWSARemoveServiceClass(LPGUID lpServiceClassId)
{
	return oWSARemoveServiceClass(lpServiceClassId);
}

BOOL __stdcall hkWSAResetEvent(WSAEVENT hEvent)
{
	return oWSAResetEvent(hEvent);
}

int __stdcall hkWSASendDisconnect(SOCKET s, LPWSABUF lpOutboundDisconnectData)
{
	return oWSASendDisconnect(s, lpOutboundDisconnectData);
}

BOOL __stdcall hkWSASetEvent(WSAEVENT hEvent)
{
	return oWSASetEvent(hEvent);
}

INT __stdcall hkWSASetServiceA(LPWSAQUERYSET lpqsRegInfo, WSAESETSERVICEOP essOperation, DWORD dwControlFlags)
{
	return oWSASetServiceA(lpqsRegInfo, essOperation, dwControlFlags);
}

INT __stdcall hkWSASetServiceW(LPWSAQUERYSET lpqsRegInfo, WSAESETSERVICEOP essOperation, DWORD dwControlFlags)
{
	return oWSASetServiceW(lpqsRegInfo, essOperation, dwControlFlags);
}

SOCKET __stdcall hkWSASocketA(int af, int type, int protocol, LPWSAPROTOCOL_INFO lpProtocolInfo, GROUP g, DWORD dwFlags)
{
	return oWSASocketA(af, type, protocol, lpProtocolInfo, g, dwFlags);
}

SOCKET __stdcall hkWSASocketW(int af, int type, int protocol, LPWSAPROTOCOL_INFO lpProtocolInfo, GROUP g, DWORD dwFlags)
{
	return oWSASocketW(af, type, protocol, lpProtocolInfo, g, dwFlags);
}

INT __stdcall hkWSAStringToAddressA(LPTSTR AddressString, INT AddressFamily, LPWSAPROTOCOL_INFO lpProtocolInfo,
								 LPSOCKADDR lpAddress, LPINT lpAddressLength)
{
	return oWSAStringToAddressA(AddressString, AddressFamily, lpProtocolInfo, lpAddress, lpAddressLength);
}

INT __stdcall hkWSAStringToAddressW(LPTSTR AddressString, INT AddressFamily, LPWSAPROTOCOL_INFO lpProtocolInfo,
								 LPSOCKADDR lpAddress, LPINT lpAddressLength)
{
	return oWSAStringToAddressW(AddressString, AddressFamily, lpProtocolInfo, lpAddress, lpAddressLength);
}

DWORD __stdcall hkWSAWaitForMultipleEvents(DWORD cEvents, const WSAEVENT *lphEvents, BOOL fWaitAll, DWORD dwTimeout,
										BOOL fAlertable)
{
	return oWSAWaitForMultipleEvents(cEvents, lphEvents, fWaitAll, dwTimeout, fAlertable);
}

int __stdcall hkWSCDeinstallProvider(LPGUID lpProviderId, LPINT lpErrno)
{
	return oWSCDeinstallProvider(lpProviderId, lpErrno);
}

int __stdcall hkWSCEnableNSProvider(LPGUID lpProviderId, BOOL fEnable)
{
	return oWSCEnableNSProvider(lpProviderId, fEnable);
}

int __stdcall hkWSCEnumProtocols(LPINT lpiProtocols, LPWSAPROTOCOL_INFOW lpProtocolBuffer, LPDWORD lpdwBufferLength,
							  LPINT lpErrno)
{
	return oWSCEnumProtocols(lpiProtocols, lpProtocolBuffer, lpdwBufferLength, lpErrno);
}

int __stdcall hkWSCGetProviderPath(LPGUID lpProviderId, LPWSTR lpszProviderDllPath, LPINT lpProviderDllPathLen,
								LPINT lpErrno)
{
	return oWSCGetProviderPath(lpProviderId, lpszProviderDllPath, lpProviderDllPathLen, lpErrno);
}

int __stdcall hkWSCInstallNameSpace(LPWSTR lpszIdentifier, LPWSTR lpszPathName, DWORD dwNameSpace, DWORD dwVersion,
								 LPGUID lpProviderId)
{
	return oWSCInstallNameSpace(lpszIdentifier, lpszPathName, dwNameSpace, dwVersion, lpProviderId);
}

int __stdcall hkWSCInstallProvider(const LPGUID lpProviderId, const LPWSTR lpszProviderDllPath, const LPWSAPROTOCOL_INFO lpProtocolInfoList,
								DWORD dwNumberOfEntries, LPINT lpErrno)
{
	return oWSCInstallProvider(lpProviderId, lpszProviderDllPath, lpProtocolInfoList, dwNumberOfEntries, 
		lpErrno);
}

int __stdcall hkWSCUnInstallNameSpace(LPGUID lpProviderId)
{
	return oWSCUnInstallNameSpace(lpProviderId);
}

int __stdcall hkWSCUpdateProvider(LPGUID lpProviderId, const WCHAR *lpszProviderDllPath, const LPWSAPROTOCOL_INFO lpProtocolInfoList,
							   DWORD dwNumberOfEntries, LPINT lpErrno)
{
	return oWSCUpdateProvider(lpProviderId, lpszProviderDllPath, lpProtocolInfoList, dwNumberOfEntries, 
		lpErrno);
}

int __stdcall hkWSCWriteNameSpaceOrder(LPGUID lpProviderId, DWORD dwNumberOfEntries)
{
	return oWSCWriteNameSpaceOrder(lpProviderId, dwNumberOfEntries);
}

int __stdcall hkWSCWriteProviderOrder(LPDWORD lpwdCatalogEntryId, DWORD dwNumberOfEntries)
{
	return oWSCWriteProviderOrder(lpwdCatalogEntryId, dwNumberOfEntries);
}

void __stdcall hkfreeaddrinfo(struct addrinfo *ai)
{
	ofreeaddrinfo(ai);
}

int __stdcall hkgetaddrinfo(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res)
{
	return ogetaddrinfo(nodename, servname, hints, res);
}

int __stdcall hkgetnameinfo(const struct sockaddr FAR *sa, socklen_t salen, char FAR *host, DWORD hostlen, char FAR *serv,
						 DWORD servlen, int flags)
{
	return ogetnameinfo(sa, salen, host, hostlen, serv, servlen, flags);
}

int __stdcall hkWSAAsyncSelect(SOCKET s, HWND hWnd, unsigned int wMsg, long lEvent)
{
	return oWSAAsyncSelect(s, hWnd, wMsg, lEvent);
}

HANDLE __stdcall hkWSAAsyncGetHostByAddr(HWND hWnd, unsigned int wMsg, const char *addr, int len, int type, char *buf,
									  int buflen)
{
	return oWSAAsyncGetHostByAddr(hWnd, wMsg, addr, len, type, buf, buflen);
}

HANDLE __stdcall hkWSAAsyncGetHostByName(HWND hWnd, unsigned int wMsg, const char *name, char *buf, int buflen)
{
	return oWSAAsyncGetHostByName(hWnd, wMsg, name, buf, buflen);
}

HANDLE __stdcall hkWSAAsyncGetProtoByNumber(HWND hWnd, unsigned int wMsg, int number, char *buf, int buflen)
{
	return oWSAAsyncGetProtoByNumber(hWnd, wMsg, number, buf, buflen);
}

HANDLE __stdcall hkWSAAsyncGetProtoByName(HWND hWnd, unsigned int wMsg, const char *name, char *buf, int buflen)
{
	return oWSAAsyncGetProtoByName(hWnd, wMsg, name, buf, buflen);
}

HANDLE __stdcall hkWSAAsyncGetServByPort(HWND hWnd, unsigned int wMsg, int port, const char *proto, char *buf,
									  int buflen)
{
	return oWSAAsyncGetServByPort(hWnd, wMsg, port, proto, buf, buflen);
}

HANDLE __stdcall hkWSAAsyncGetServByName(HWND hWnd, unsigned int wMsg, const char *name, const char *proto, char *buf,
									  int buflen)
{
	return oWSAAsyncGetServByName(hWnd, wMsg, name, proto, buf, buflen);
}

int __stdcall hkWSACancelAsyncRequest(HANDLE hAsyncTaskHandle)
{
	return oWSACancelAsyncRequest(hAsyncTaskHandle);
}

FARPROC __stdcall hkWSASetBlockingHook(FARPROC lpBlockingFunc)
{
	return oWSASetBlockingHook(lpBlockingFunc);
}

int __stdcall hkWSAUnhookBlockingHook(void)
{
	return oWSAUnhookBlockingHook();
}

int __stdcall hkWSAGetLastError(void)
{
	return oWSAGetLastError();
}

void __stdcall hkWSASetLastError(int iError)
{
	oWSASetLastError(iError);
}

int __stdcall hkWSACancelBlockingCall(void)
{
	return oWSACancelBlockingCall();
}

BOOL __stdcall hkWSAIsBlocking(void)
{
	return oWSAIsBlocking();
}

int __stdcall hkWSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
{
	return oWSAStartup(wVersionRequested, lpWSAData);
}

int __stdcall hkWSACleanup(void)
{
	return oWSACleanup();
}

int __stdcall hk__WSAFDIsSet(SOCKET fd, fd_set *set)
{
	return o__WSAFDIsSet(fd, set);
}

int __stdcall hkWEP(void)
{
	return oWEP();
}


void recvHook()
{
	HMODULE hMod = GetModuleHandle(_T("ws2_32.dll"));
	if (0 == hMod)
	{
		hMod = LoadLibrary(_T("ws2_32.dll"));
	}
	if (0 != hMod)
	{
		/*
		oaccept = (taccept)DetourMaker(hMod, "accept", (LPBYTE)hkaccept, 5);
		obind = (tbind)DetourMaker(hMod, "bind", (LPBYTE)hkbind, 5);
		oclosesocket = (tclosesocket)DetourMaker(hMod, "closesocket", (LPBYTE)hkclosesocket, 5);
		oconnect = (tconnect)DetourMaker(hMod, "connect", (LPBYTE)hkconnect, 5);
		*/
		/*
		ogetpeername = (tgetpeername)DetourMaker(hMod, "getpeername", (LPBYTE)hkgetpeername, 5);
		ogetsockname = (tgetsockname)DetourMaker(hMod, "getsockname", (LPBYTE)hkgetsockname, 5);
		ogetsockopt = (tgetsockopt)DetourMaker(hMod, "getsockopt", (LPBYTE)hkgetsockopt, 5);
		ohtonl = (thtonl)DetourMaker(hMod, "htonl", (LPBYTE)hkhtonl, 5);
		ohtons = (thtons)DetourMaker(hMod, "htons", (LPBYTE)hkhtons, 5);
		oioctlsocket = (tioctlsocket)DetourMaker(hMod, "ioctlsocket", (LPBYTE)hkioctlsocket, 5);
		oinet_addr = (tinet_addr)DetourMaker(hMod, "inet_addr", (LPBYTE)hkinet_addr, 5);
		oinet_ntoa = (tinet_ntoa)DetourMaker(hMod, "inet_ntoa", (LPBYTE)hkinet_ntoa, 5);
		olisten = (tlisten)DetourMaker(hMod, "listen", (LPBYTE)hklisten, 5);
		ontohl = (tntohl)DetourMaker(hMod, "ntohl", (LPBYTE)hkntohl, 5);
		ontohs = (tntohs)DetourMaker(hMod, "ntohs", (LPBYTE)hkntohs, 5);
		*/
		orecv = (trecv)DetourMaker(hMod, "recv", (LPBYTE)hkrecv, 5);
		orecvfrom = (trecvfrom)DetourMaker(hMod, "recvfrom", (LPBYTE)hkrecvfrom, 5);
		//oselect = (tselect)DetourMaker(hMod, "select", (LPBYTE)hkselect, 5);
		osend = (tsend)DetourMaker(hMod, "send", (LPBYTE)hksend, 5);
		osendto = (tsendto)DetourMaker(hMod, "sendto", (LPBYTE)hksendto, 5);
		/*
		osetsockopt = (tsetsockopt)DetourMaker(hMod, "setsockopt", (LPBYTE)hksetsockopt, 5);
		oshutdown = (tshutdown)DetourMaker(hMod, "shutdown", (LPBYTE)hkshutdown, 5);
		osocket = (tsocket)DetourMaker(hMod, "socket", (LPBYTE)hksocket, 5);
		oGetAddrInfoW = (tGetAddrInfoW)DetourMaker(hMod, "GetAddrInfoW", (LPBYTE)hkGetAddrInfoW, 5);
		oGetNameInfoW = (tGetNameInfoW)DetourMaker(hMod, "GetNameInfoW", (LPBYTE)hkGetNameInfoW, 5);
		oWSApSetPostRoutine = (tWSApSetPostRoutine)DetourMaker(hMod, "WSApSetPostRoutine", (LPBYTE)hkWSApSetPostRoutine, 5);
		oFreeAddrInfoW = (tFreeAddrInfoW)DetourMaker(hMod, "FreeAddrInfoW", (LPBYTE)hkFreeAddrInfoW, 5);
		oWPUCompleteOverlappedRequest = (tWPUCompleteOverlappedRequest)DetourMaker(hMod, "WPUCompleteOverlappedRequest", (LPBYTE)hkWPUCompleteOverlappedRequest, 5);
		oWSAAccept = (tWSAAccept)DetourMaker(hMod, "WSAAccept", (LPBYTE)hkWSAAccept, 5);
		oWSAAddressToStringA = (tWSAAddressToStringA)DetourMaker(hMod, "WSAAddressToStringA", (LPBYTE)hkWSAAddressToStringA, 5);
		oWSAAddressToStringW = (tWSAAddressToStringW)DetourMaker(hMod, "WSAAddressToStringW", (LPBYTE)hkWSAAddressToStringW, 5);
		oWSACloseEvent = (tWSACloseEvent)DetourMaker(hMod, "WSACloseEvent", (LPBYTE)hkWSACloseEvent, 5);
		oWSAConnect = (tWSAConnect)DetourMaker(hMod, "WSAConnect", (LPBYTE)hkWSAConnect, 5);
		oWSACreateEvent = (tWSACreateEvent)DetourMaker(hMod, "WSACreateEvent", (LPBYTE)hkWSACreateEvent, 5);
		oWSADuplicateSocketA = (tWSADuplicateSocketA)DetourMaker(hMod, "WSADuplicateSocketA", (LPBYTE)hkWSADuplicateSocketA, 5);
		oWSADuplicateSocketW = (tWSADuplicateSocketW)DetourMaker(hMod, "WSADuplicateSocketW", (LPBYTE)hkWSADuplicateSocketW, 5);
		oWSAEnumNameSpaceProvidersA = (tWSAEnumNameSpaceProvidersA)DetourMaker(hMod, "WSAEnumNameSpaceProvidersA", (LPBYTE)hkWSAEnumNameSpaceProvidersA, 5);
		oWSAEnumNameSpaceProvidersW = (tWSAEnumNameSpaceProvidersW)DetourMaker(hMod, "WSAEnumNameSpaceProvidersW", (LPBYTE)hkWSAEnumNameSpaceProvidersW, 5);
		oWSAEnumNetworkEvents = (tWSAEnumNetworkEvents)DetourMaker(hMod, "WSAEnumNetworkEvents", (LPBYTE)hkWSAEnumNetworkEvents, 5);
		oWSAEnumProtocolsA = (tWSAEnumProtocolsA)DetourMaker(hMod, "WSAEnumProtocolsA", (LPBYTE)hkWSAEnumProtocolsA, 5);
		oWSAEnumProtocolsW = (tWSAEnumProtocolsW)DetourMaker(hMod, "WSAEnumProtocolsW", (LPBYTE)hkWSAEnumProtocolsW, 5);
		oWSAEventSelect = (tWSAEventSelect)DetourMaker(hMod, "WSAEventSelect", (LPBYTE)hkWSAEventSelect, 5);
		oWSAGetOverlappedResult = (tWSAGetOverlappedResult)DetourMaker(hMod, "WSAGetOverlappedResult", (LPBYTE)hkWSAGetOverlappedResult, 5);
		oWSAGetQOSByName = (tWSAGetQOSByName)DetourMaker(hMod, "WSAGetQOSByName", (LPBYTE)hkWSAGetQOSByName, 5);
		oWSAGetServiceClassInfoA = (tWSAGetServiceClassInfoA)DetourMaker(hMod, "WSAGetServiceClassInfoA", (LPBYTE)hkWSAGetServiceClassInfoA, 5);
		oWSAGetServiceClassInfoW = (tWSAGetServiceClassInfoW)DetourMaker(hMod, "WSAGetServiceClassInfoW", (LPBYTE)hkWSAGetServiceClassInfoW, 5);
		oWSAGetServiceClassNameByClassIdA = (tWSAGetServiceClassNameByClassIdA)DetourMaker(hMod, "WSAGetServiceClassNameByClassIdA", (LPBYTE)hkWSAGetServiceClassNameByClassIdA, 5);
		oWSAGetServiceClassNameByClassIdW = (tWSAGetServiceClassNameByClassIdW)DetourMaker(hMod, "WSAGetServiceClassNameByClassIdW", (LPBYTE)hkWSAGetServiceClassNameByClassIdW, 5);
		oWSAHtonl = (tWSAHtonl)DetourMaker(hMod, "WSAHtonl", (LPBYTE)hkWSAHtonl, 5);
		oWSAHtons = (tWSAHtons)DetourMaker(hMod, "WSAHtons", (LPBYTE)hkWSAHtons, 5);
		ogethostbyaddr = (tgethostbyaddr)DetourMaker(hMod, "gethostbyaddr", (LPBYTE)hkgethostbyaddr, 5);
		ogethostbyname = (tgethostbyname)DetourMaker(hMod, "gethostbyname", (LPBYTE)hkgethostbyname, 5);
		ogetprotobyname = (tgetprotobyname)DetourMaker(hMod, "getprotobyname", (LPBYTE)hkgetprotobyname, 5);
		ogetprotobynumber = (tgetprotobynumber)DetourMaker(hMod, "getprotobynumber", (LPBYTE)hkgetprotobynumber, 5);
		ogetservbyname = (tgetservbyname)DetourMaker(hMod, "getservbyname", (LPBYTE)hkgetservbyname, 5);
		ogetservbyport = (tgetservbyport)DetourMaker(hMod, "getservbyport", (LPBYTE)hkgetservbyport, 5);
		ogethostname = (tgethostname)DetourMaker(hMod, "gethostname", (LPBYTE)hkgethostname, 5);
		oWSAInstallServiceClassA = (tWSAInstallServiceClassA)DetourMaker(hMod, "WSAInstallServiceClassA", (LPBYTE)hkWSAInstallServiceClassA, 5);
		oWSAInstallServiceClassW = (tWSAInstallServiceClassW)DetourMaker(hMod, "WSAInstallServiceClassW", (LPBYTE)hkWSAInstallServiceClassW, 5);
		oWSAIoctl = (tWSAIoctl)DetourMaker(hMod, "WSAIoctl", (LPBYTE)hkWSAIoctl, 5);
		oWSAJoinLeaf = (tWSAJoinLeaf)DetourMaker(hMod, "WSAJoinLeaf", (LPBYTE)hkWSAJoinLeaf, 5);
		oWSALookupServiceBeginA = (tWSALookupServiceBeginA)DetourMaker(hMod, "WSALookupServiceBeginA", (LPBYTE)hkWSALookupServiceBeginA, 5);
		oWSALookupServiceBeginW = (tWSALookupServiceBeginW)DetourMaker(hMod, "WSALookupServiceBeginW", (LPBYTE)hkWSALookupServiceBeginW, 5);
		oWSALookupServiceEnd = (tWSALookupServiceEnd)DetourMaker(hMod, "WSALookupServiceEnd", (LPBYTE)hkWSALookupServiceEnd, 5);
		oWSALookupServiceNextA = (tWSALookupServiceNextA)DetourMaker(hMod, "WSALookupServiceNextA", (LPBYTE)hkWSALookupServiceNextA, 5);
		oWSALookupServiceNextW = (tWSALookupServiceNextW)DetourMaker(hMod, "WSALookupServiceNextW", (LPBYTE)hkWSALookupServiceNextW, 5);
		oWSANSPIoctl = (tWSANSPIoctl)DetourMaker(hMod, "WSANSPIoctl", (LPBYTE)hkWSANSPIoctl, 5);
		oWSANtohl = (tWSANtohl)DetourMaker(hMod, "WSANtohl", (LPBYTE)hkWSANtohl, 5);
		oWSANtohs = (tWSANtohs)DetourMaker(hMod, "WSANtohs", (LPBYTE)hkWSANtohs, 5);
		oWSAProviderConfigChange = (tWSAProviderConfigChange)DetourMaker(hMod, "WSAProviderConfigChange", (LPBYTE)hkWSAProviderConfigChange, 5);
		*/
		oWSARecv = (tWSARecv)DetourMaker(hMod, "WSARecv", (LPBYTE)hkWSARecv, 5);
		//oWSARecvDisconnect = (tWSARecvDisconnect)DetourMaker(hMod, "WSARecvDisconnect", (LPBYTE)hkWSARecvDisconnect, 5);
		oWSARecvFrom = (tWSARecvFrom)DetourMaker(hMod, "WSARecvFrom", (LPBYTE)hkWSARecvFrom, 5);
		//oWSARemoveServiceClass = (tWSARemoveServiceClass)DetourMaker(hMod, "WSARemoveServiceClass", (LPBYTE)hkWSARemoveServiceClass, 5);
		//oWSAResetEvent = (tWSAResetEvent)DetourMaker(hMod, "WSAResetEvent", (LPBYTE)hkWSAResetEvent, 5);
		oWSASend = (tWSASend)DetourMaker(hMod, "WSASend", (LPBYTE)hkWSASend, 5);
		//oWSASendDisconnect = (tWSASendDisconnect)DetourMaker(hMod, "WSASendDisconnect", (LPBYTE)hkWSASendDisconnect, 5);
		oWSASendTo = (tWSASendTo)DetourMaker(hMod, "WSASendTo", (LPBYTE)hkWSASendTo, 5);
		/*
		oWSASetEvent = (tWSASetEvent)DetourMaker(hMod, "WSASetEvent", (LPBYTE)hkWSASetEvent, 5);
		oWSASetServiceA = (tWSASetServiceA)DetourMaker(hMod, "WSASetServiceA", (LPBYTE)hkWSASetServiceA, 5);
		oWSASetServiceW = (tWSASetServiceW)DetourMaker(hMod, "WSASetServiceW", (LPBYTE)hkWSASetServiceW, 5);
		oWSASocketA = (tWSASocketA)DetourMaker(hMod, "WSASocketA", (LPBYTE)hkWSASocketA, 5);
		oWSASocketW = (tWSASocketW)DetourMaker(hMod, "WSASocketW", (LPBYTE)hkWSASocketW, 5);
		oWSAStringToAddressA = (tWSAStringToAddressA)DetourMaker(hMod, "WSAStringToAddressA", (LPBYTE)hkWSAStringToAddressA, 5);
		oWSAStringToAddressW = (tWSAStringToAddressW)DetourMaker(hMod, "WSAStringToAddressW", (LPBYTE)hkWSAStringToAddressW, 5);
		oWSAWaitForMultipleEvents = (tWSAWaitForMultipleEvents)DetourMaker(hMod, "WSAWaitForMultipleEvents", (LPBYTE)hkWSAWaitForMultipleEvents, 5);
		oWSCDeinstallProvider = (tWSCDeinstallProvider)DetourMaker(hMod, "WSCDeinstallProvider", (LPBYTE)hkWSCDeinstallProvider, 5);
		oWSCEnableNSProvider = (tWSCEnableNSProvider)DetourMaker(hMod, "WSCEnableNSProvider", (LPBYTE)hkWSCEnableNSProvider, 5);
		oWSCEnumProtocols = (tWSCEnumProtocols)DetourMaker(hMod, "WSCEnumProtocols", (LPBYTE)hkWSCEnumProtocols, 5);
		oWSCGetProviderPath = (tWSCGetProviderPath)DetourMaker(hMod, "WSCGetProviderPath", (LPBYTE)hkWSCGetProviderPath, 5);
		oWSCInstallNameSpace = (tWSCInstallNameSpace)DetourMaker(hMod, "WSCInstallNameSpace", (LPBYTE)hkWSCInstallNameSpace, 5);
		oWSCInstallProvider = (tWSCInstallProvider)DetourMaker(hMod, "WSCInstallProvider", (LPBYTE)hkWSCInstallProvider, 5);
		oWSCUnInstallNameSpace = (tWSCUnInstallNameSpace)DetourMaker(hMod, "WSCUnInstallNameSpace", (LPBYTE)hkWSCUnInstallNameSpace, 5);
		oWSCUpdateProvider = (tWSCUpdateProvider)DetourMaker(hMod, "WSCUpdateProvider", (LPBYTE)hkWSCUpdateProvider, 5);
		oWSCWriteNameSpaceOrder = (tWSCWriteNameSpaceOrder)DetourMaker(hMod, "WSCWriteNameSpaceOrder", (LPBYTE)hkWSCWriteNameSpaceOrder, 5);
		oWSCWriteProviderOrder = (tWSCWriteProviderOrder)DetourMaker(hMod, "WSCWriteProviderOrder", (LPBYTE)hkWSCWriteProviderOrder, 5);
		ofreeaddrinfo = (tfreeaddrinfo)DetourMaker(hMod, "freeaddrinfo", (LPBYTE)hkfreeaddrinfo, 5);
		ogetaddrinfo = (tgetaddrinfo)DetourMaker(hMod, "getaddrinfo", (LPBYTE)hkgetaddrinfo, 5);
		ogetnameinfo = (tgetnameinfo)DetourMaker(hMod, "getnameinfo", (LPBYTE)hkgetnameinfo, 5);
		oWSAAsyncSelect = (tWSAAsyncSelect)DetourMaker(hMod, "WSAAsyncSelect", (LPBYTE)hkWSAAsyncSelect, 5);
		oWSAAsyncGetHostByAddr = (tWSAAsyncGetHostByAddr)DetourMaker(hMod, "WSAAsyncGetHostByAddr", (LPBYTE)hkWSAAsyncGetHostByAddr, 5);
		oWSAAsyncGetHostByName = (tWSAAsyncGetHostByName)DetourMaker(hMod, "WSAAsyncGetHostByName", (LPBYTE)hkWSAAsyncGetHostByName, 5);
		oWSAAsyncGetProtoByNumber = (tWSAAsyncGetProtoByNumber)DetourMaker(hMod, "WSAAsyncGetProtoByNumber", (LPBYTE)hkWSAAsyncGetProtoByNumber, 5);
		oWSAAsyncGetProtoByName = (tWSAAsyncGetProtoByName)DetourMaker(hMod, "WSAAsyncGetProtoByName", (LPBYTE)hkWSAAsyncGetProtoByName, 5);
		oWSAAsyncGetServByPort = (tWSAAsyncGetServByPort)DetourMaker(hMod, "WSAAsyncGetServByPort", (LPBYTE)hkWSAAsyncGetServByPort, 5);
		oWSAAsyncGetServByName = (tWSAAsyncGetServByName)DetourMaker(hMod, "WSAAsyncGetServByName", (LPBYTE)hkWSAAsyncGetServByName, 5);
		oWSACancelAsyncRequest = (tWSACancelAsyncRequest)DetourMaker(hMod, "WSACancelAsyncRequest", (LPBYTE)hkWSACancelAsyncRequest, 5);
		oWSASetBlockingHook = (tWSASetBlockingHook)DetourMaker(hMod, "WSASetBlockingHook", (LPBYTE)hkWSASetBlockingHook, 5);
		oWSAUnhookBlockingHook = (tWSAUnhookBlockingHook)DetourMaker(hMod, "WSAUnhookBlockingHook", (LPBYTE)hkWSAUnhookBlockingHook, 5);
		oWSAGetLastError = (tWSAGetLastError)DetourMaker(hMod, "WSAGetLastError", (LPBYTE)hkWSAGetLastError, 5);
		oWSASetLastError = (tWSASetLastError)DetourMaker(hMod, "WSASetLastError", (LPBYTE)hkWSASetLastError, 5);
		oWSACancelBlockingCall = (tWSACancelBlockingCall)DetourMaker(hMod, "WSACancelBlockingCall", (LPBYTE)hkWSACancelBlockingCall, 5);
		oWSAIsBlocking = (tWSAIsBlocking)DetourMaker(hMod, "WSAIsBlocking", (LPBYTE)hkWSAIsBlocking, 5);
		oWSAStartup = (tWSAStartup)DetourMaker(hMod, "WSAStartup", (LPBYTE)hkWSAStartup, 5);
		oWSACleanup = (tWSACleanup)DetourMaker(hMod, "WSACleanup", (LPBYTE)hkWSACleanup, 5);
		o__WSAFDIsSet = (t__WSAFDIsSet)DetourMaker(hMod, "__WSAFDIsSet", (LPBYTE)hk__WSAFDIsSet, 5);
		oWEP = (tWEP)DetourMaker(hMod, "WEP", (LPBYTE)hkWEP, 5);
		*/
	}
	
}