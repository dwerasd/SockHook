#pragma once


#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>



typedef SOCKET (__stdcall *taccept)(SOCKET s, struct sockaddr *addr, int *addrlen);
typedef int (__stdcall *tbind)(SOCKET s, const struct sockaddr *name, int namelen);
typedef int (__stdcall *tclosesocket)(SOCKET s);
typedef int (__stdcall *tconnect)(SOCKET s, const struct sockaddr *name, int namelen);
typedef int (__stdcall *tgetpeername)(SOCKET s, struct sockaddr *name, int *namelen);
typedef int (__stdcall *tgetsockname)(SOCKET s, struct sockaddr *name, int *namelen);
typedef int (__stdcall *tgetsockopt)(SOCKET s, int level, int optname, char *optval, int *optlen);
typedef u_long (__stdcall *thtonl)(u_long hostlong);
typedef u_short (__stdcall *thtons)(u_short hostshort);
typedef int (__stdcall *tioctlsocket)(SOCKET s, long cmd, u_long *argp);
typedef unsigned long (__stdcall *tinet_addr)(const char *cp);
typedef char* (__stdcall *tinet_ntoa)(struct in_addr in);
typedef int (__stdcall *tlisten)(SOCKET s, int backlog);
typedef u_long (__stdcall *tntohl)(u_long netlong);
typedef u_short (__stdcall *tntohs)(u_short netshort);
typedef int (__stdcall *trecv)(SOCKET s, char *buf, int len, int flags);
typedef int (__stdcall *trecvfrom)(SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen);
typedef int (__stdcall *tselect)(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout);
typedef int (__stdcall *tsend)(SOCKET s, const char *buf, int len, int flags);
typedef int (__stdcall *tsendto)(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen);
typedef int (__stdcall *tsetsockopt)(SOCKET s, int level, int optname, const char *optval, int optlen);
typedef int (__stdcall *tshutdown)(SOCKET s, int how);
typedef SOCKET (__stdcall *tsocket)(int af, int type, int protocol);
typedef int (__stdcall *tGetAddrInfoW)(PCWSTR pNodeName, PCWSTR pServiceName, const ADDRINFOW *pHints, PADDRINFOW *ppResult);
typedef int (__stdcall *tGetNameInfoW)(const SOCKADDR *pSockaddr, socklen_t SockaddrLength, PWCHAR pNodeBuffer, DWORD NodeBufferSize,
									PWCHAR pServiceBuffer, DWORD ServiceBufferSize, INT Flags);
typedef INT (__stdcall *tWSApSetPostRoutine)(LPVOID lpPostRoutine);
typedef void (__stdcall *tFreeAddrInfoW)(PADDRINFOW pAddrInfo);
typedef WSAEVENT (__stdcall *tWPUCompleteOverlappedRequest)(SOCKET s, LPWSAOVERLAPPED lpOverlapped, DWORD dwError, DWORD cbTransferred,
														 LPINT lpErrno);
typedef SOCKET (__stdcall *tWSAAccept)(SOCKET s, struct sockaddr *addr, LPINT addrlen, LPCONDITIONPROC lpfnCondition, DWORD dwCallbackData);
typedef INT (__stdcall *tWSAAddressToStringA)(LPSOCKADDR lpsaAddress, DWORD dwAddressLength, LPWSAPROTOCOL_INFO lpProtocolInfo,
										   LPTSTR lpszAddressString, LPDWORD lpdwAddressStringLength);
typedef INT (__stdcall *tWSAAddressToStringW)(LPSOCKADDR lpsaAddress, DWORD dwAddressLength, LPWSAPROTOCOL_INFO lpProtocolInfo,
										   LPTSTR lpszAddressString, LPDWORD lpdwAddressStringLength);
typedef BOOL (__stdcall *tWSACloseEvent)(WSAEVENT hEvent);
typedef int (__stdcall *tWSAConnect)(SOCKET s, const struct sockaddr *name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData,
								  LPQOS lpSQOS, LPQOS lpGQOS);
typedef WSAEVENT (__stdcall *tWSACreateEvent)(void);
typedef int (__stdcall *tWSADuplicateSocketA)(SOCKET s, DWORD dwProcessId, LPWSAPROTOCOL_INFO lpProtocolInfo);
typedef int (__stdcall *tWSADuplicateSocketW)(SOCKET s, DWORD dwProcessId, LPWSAPROTOCOL_INFO lpProtocolInfo);
typedef INT (__stdcall *tWSAEnumNameSpaceProvidersA)(LPDWORD lpdwBufferLength, LPWSANAMESPACE_INFO lpnspBuffer);
typedef INT (__stdcall *tWSAEnumNameSpaceProvidersW)(LPDWORD lpdwBufferLength, LPWSANAMESPACE_INFO lpnspBuffer);
typedef int (__stdcall *tWSAEnumNetworkEvents)(SOCKET s, WSAEVENT hEventObject, LPWSANETWORKEVENTS lpNetworkEvents);
typedef int (__stdcall *tWSAEnumProtocolsA)(LPINT lpiProtocols, LPWSAPROTOCOL_INFO lpProtocolBuffer, LPDWORD lpdwBufferLength);
typedef int (__stdcall *tWSAEnumProtocolsW)(LPINT lpiProtocols, LPWSAPROTOCOL_INFO lpProtocolBuffer, LPDWORD lpdwBufferLength);
typedef int (__stdcall *tWSAEventSelect)(SOCKET s, WSAEVENT hEventObject, long lNetworkEvents);
typedef BOOL (__stdcall *tWSAGetOverlappedResult)(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait,
											   LPDWORD lpdwFlags);
typedef BOOL (__stdcall *tWSAGetQOSByName)(SOCKET s, LPWSABUF lpQOSName, LPQOS lpQOS);
typedef INT (__stdcall *tWSAGetServiceClassInfoA)(LPGUID lpProviderId, LPGUID lpServiceClassId, LPDWORD lpdwBufferLength,
											   LPWSASERVICECLASSINFO lpServiceClassInfo);
typedef INT (__stdcall *tWSAGetServiceClassInfoW)(LPGUID lpProviderId, LPGUID lpServiceClassId, LPDWORD lpdwBufferLength,
											   LPWSASERVICECLASSINFO lpServiceClassInfo);
typedef INT (__stdcall *tWSAGetServiceClassNameByClassIdA)(LPGUID lpServiceClassId, LPTSTR lpszServiceClassName, LPDWORD lpdwBufferLength);
typedef INT (__stdcall *tWSAGetServiceClassNameByClassIdW)(LPGUID lpServiceClassId, LPTSTR lpszServiceClassName, LPDWORD lpdwBufferLength);
typedef int (__stdcall *tWSAHtonl)(SOCKET s, u_long hostlong, u_long *lpnetlong);
typedef int (__stdcall *tWSAHtons)(SOCKET s, u_short hostshort, u_short *lpnetshort);
typedef struct hostent* (__stdcall *tgethostbyaddr)(const char *addr, int len, int type);
typedef struct hostent* (__stdcall *tgethostbyname)(const char *name);
typedef struct protoent* (__stdcall *tgetprotobyname)(const char *name);
typedef struct protoent* (__stdcall *tgetprotobynumber)(int number);
typedef struct servent* (__stdcall *tgetservbyname)(const char *name, const char *proto);
typedef struct servent* (__stdcall *tgetservbyport)(int port, const char *proto);
typedef int (__stdcall *tgethostname)(char *name, int namelen);
typedef INT (__stdcall *tWSAInstallServiceClassA)(LPWSASERVICECLASSINFO lpServiceClassInfo);
typedef INT (__stdcall *tWSAInstallServiceClassW)(LPWSASERVICECLASSINFO lpServiceClassInfo);
typedef int (__stdcall *tWSAIoctl)(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer,
								DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef SOCKET (__stdcall *tWSAJoinLeaf)(SOCKET s, const struct sockaddr *name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData,
									  LPQOS lpSQOS, LPQOS lpGQOS, DWORD dwFlags);
typedef INT (__stdcall *tWSALookupServiceBeginA)(LPWSAQUERYSET lpqsRestrictions, DWORD dwControlFlags, LPHANDLE lphLookup);
typedef INT (__stdcall *tWSALookupServiceBeginW)(LPWSAQUERYSET lpqsRestrictions, DWORD dwControlFlags, LPHANDLE lphLookup);
typedef INT (__stdcall *tWSALookupServiceEnd)(HANDLE hLookup);
typedef INT (__stdcall *tWSALookupServiceNextA)(HANDLE hLookup, DWORD dwControlFlags, LPDWORD lpdwBufferLength, LPWSAQUERYSET lpqsResults);
typedef INT (__stdcall *tWSALookupServiceNextW)(HANDLE hLookup, DWORD dwControlFlags, LPDWORD lpdwBufferLength, LPWSAQUERYSET lpqsResults);
typedef int (__stdcall *tWSANSPIoctl)(HANDLE hLookup, DWORD dwControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer,
								   DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSACOMPLETION lpCompletion);
typedef int (__stdcall *tWSANtohl)(SOCKET s, u_long netlong, u_long *lphostlong);
typedef int (__stdcall *tWSANtohs)(SOCKET s, u_short netshort, u_short *lphostshort);
typedef int (__stdcall *tWSAProviderConfigChange)(LPHANDLE lpNotificationHandle, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef int (__stdcall *tWSARecv)(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags,
							   LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef int (__stdcall *tWSARecvDisconnect)(SOCKET s, LPWSABUF lpInboundDisconnectData);
typedef int (__stdcall *tWSARecvFrom)(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags,
struct sockaddr *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef INT (__stdcall *tWSARemoveServiceClass)(LPGUID lpServiceClassId);
typedef BOOL (__stdcall *tWSAResetEvent)(WSAEVENT hEvent);
typedef int (__stdcall *tWSASend)(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
							   LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef int (__stdcall *tWSASendDisconnect)(SOCKET s, LPWSABUF lpOutboundDisconnectData);
typedef int (__stdcall *tWSASendTo)(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
								 const struct sockaddr *lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef BOOL (__stdcall *tWSASetEvent)(WSAEVENT hEvent);
typedef INT (__stdcall *tWSASetServiceA)(LPWSAQUERYSET lpqsRegInfo, WSAESETSERVICEOP essOperation, DWORD dwControlFlags);
typedef INT (__stdcall *tWSASetServiceW)(LPWSAQUERYSET lpqsRegInfo, WSAESETSERVICEOP essOperation, DWORD dwControlFlags);
typedef SOCKET (__stdcall *tWSASocketA)(int af, int type, int protocol, LPWSAPROTOCOL_INFO lpProtocolInfo, GROUP g, DWORD dwFlags);
typedef SOCKET (__stdcall *tWSASocketW)(int af, int type, int protocol, LPWSAPROTOCOL_INFO lpProtocolInfo, GROUP g, DWORD dwFlags);
typedef INT (__stdcall *tWSAStringToAddressA)(LPTSTR AddressString, INT AddressFamily, LPWSAPROTOCOL_INFO lpProtocolInfo,
										   LPSOCKADDR lpAddress, LPINT lpAddressLength);
typedef INT (__stdcall *tWSAStringToAddressW)(LPTSTR AddressString, INT AddressFamily, LPWSAPROTOCOL_INFO lpProtocolInfo,
										   LPSOCKADDR lpAddress, LPINT lpAddressLength);
typedef DWORD (__stdcall *tWSAWaitForMultipleEvents)(DWORD cEvents, const WSAEVENT *lphEvents, BOOL fWaitAll, DWORD dwTimeout,
												  BOOL fAlertable);
typedef int (__stdcall *tWSCDeinstallProvider)(LPGUID lpProviderId, LPINT lpErrno);
typedef int (__stdcall *tWSCEnableNSProvider)(LPGUID lpProviderId, BOOL fEnable);
typedef int (__stdcall *tWSCEnumProtocols)(LPINT lpiProtocols, LPWSAPROTOCOL_INFOW lpProtocolBuffer, LPDWORD lpdwBufferLength,
										LPINT lpErrno);
typedef int (__stdcall *tWSCGetProviderPath)(LPGUID lpProviderId, LPWSTR lpszProviderDllPath, LPINT lpProviderDllPathLen,
										  LPINT lpErrno);
typedef int (__stdcall *tWSCInstallNameSpace)(LPWSTR lpszIdentifier, LPWSTR lpszPathName, DWORD dwNameSpace, DWORD dwVersion,
										   LPGUID lpProviderId);
typedef int (__stdcall *tWSCInstallProvider)(const LPGUID lpProviderId, const LPWSTR lpszProviderDllPath, const LPWSAPROTOCOL_INFO lpProtocolInfoList,
										  DWORD dwNumberOfEntries, LPINT lpErrno);
typedef int (__stdcall *tWSCUnInstallNameSpace)(LPGUID lpProviderId);
typedef int (__stdcall *tWSCUpdateProvider)(LPGUID lpProviderId, const WCHAR *lpszProviderDllPath, const LPWSAPROTOCOL_INFO lpProtocolInfoList,
										 DWORD dwNumberOfEntries, LPINT lpErrno);
typedef int (__stdcall *tWSCWriteNameSpaceOrder)(LPGUID lpProviderId, DWORD dwNumberOfEntries);
typedef int (__stdcall *tWSCWriteProviderOrder)(LPDWORD lpwdCatalogEntryId, DWORD dwNumberOfEntries);
typedef void (__stdcall *tfreeaddrinfo)(struct addrinfo *ai);
typedef int (__stdcall *tgetaddrinfo)(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res);
typedef int (__stdcall *tgetnameinfo)(const struct sockaddr FAR *sa, socklen_t salen, char FAR *host, DWORD hostlen, char FAR *serv,
								   DWORD servlen, int flags);
typedef int (__stdcall *tWSAAsyncSelect)(SOCKET s, HWND hWnd, unsigned int wMsg, long lEvent);
typedef HANDLE (__stdcall *tWSAAsyncGetHostByAddr)(HWND hWnd, unsigned int wMsg, const char *addr, int len, int type, char *buf,
												int buflen);
typedef HANDLE (__stdcall *tWSAAsyncGetHostByName)(HWND hWnd, unsigned int wMsg, const char *name, char *buf, int buflen);
typedef HANDLE (__stdcall *tWSAAsyncGetProtoByNumber)(HWND hWnd, unsigned int wMsg, int number, char *buf, int buflen);
typedef HANDLE (__stdcall *tWSAAsyncGetProtoByName)(HWND hWnd, unsigned int wMsg, const char *name, char *buf, int buflen);
typedef HANDLE (__stdcall *tWSAAsyncGetServByPort)(HWND hWnd, unsigned int wMsg, int port, const char *proto, char *buf,
												int buflen);
typedef HANDLE (__stdcall *tWSAAsyncGetServByName)(HWND hWnd, unsigned int wMsg, const char *name, const char *proto, char *buf,
												int buflen);
typedef int (__stdcall *tWSACancelAsyncRequest)(HANDLE hAsyncTaskHandle);
typedef FARPROC (__stdcall *tWSASetBlockingHook)(FARPROC lpBlockingFunc);
typedef int (__stdcall *tWSAUnhookBlockingHook)(void);
typedef int (__stdcall *tWSAGetLastError)(void);
typedef void (__stdcall *tWSASetLastError)(int iError);
typedef int (__stdcall *tWSACancelBlockingCall)(void);
typedef BOOL (__stdcall *tWSAIsBlocking)(void);
typedef int (__stdcall *tWSAStartup)(WORD wVersionRequested, LPWSADATA lpWSAData);
typedef int (__stdcall *tWSACleanup)(void);
typedef int (__stdcall *t__WSAFDIsSet)(SOCKET fd, fd_set *set);
typedef int (__stdcall *tWEP)(void);