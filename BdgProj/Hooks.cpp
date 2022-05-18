#include "stdafx.h"
#include "Hooks.h"
#include "Functions.h"



ULONG ulHookAddr = 0, ulJmpAddr = 0;
ULONG ulDecBufPtr = 0, ulDecBufAddr = 0, ulDecBufPtr2 = 0, ulDecBufAddr2 = 0;
long nPacketSize = 0;

wchar_t szBuf[1 << 4];
wchar_t szBuffer[1 << 15];

BYTE bytPtnHook1[] =
{
	0xA1, 0x7C, 0x53, 0x44, 0x00, 0x8B, 0x3D, 0x84, 0x53, 0x44, 0x00
};

typedef struct _LOG_DATA
{
	wchar_t wszTag[1 << 4];
	WORD nSize;
	char szBuf[(1 << 15)];
} LOG_DATA, *LPLOG_DATA;


C_IOCP_QUEUE<LPLOG_DATA> *pLogQueue = 0;
C_OBJECT_POOL_DLF<LOG_DATA> *pLogDataPool = 0;
long b;


void __stdcall HookLogThread()
{
	do 
	{
		LPLOG_DATA pLogData = pLogQueue->pop();

		wchar_t wszBuffer[(1 << 15)];
		wsprintfW(wszBuffer, L"[%s/%04d] ", pLogData->wszTag, pLogData->nSize);
		for (b = 0 ; b < pLogData->nSize ; b++ )
		{
			wchar_t wszBuf[4] = { 0 };
			wsprintfW(wszBuf, L"%02x ", *(LPBYTE)(pLogData->szBuf+b) & 0xff);
			if (0 < b && 0 == (b % 256) )
			{
				wcscat(wszBuffer, L"\r\n");
			}
			wcscat(wszBuffer, wszBuf);
		}
		pLog->Write(wszBuffer);
		pLogDataPool->Deallocate(pLogData);
	} while (TRUE);
}

DWORD dwOriAddr = 0x0040198e, dwTemp = 0x00445384;
WORD nSize, i;
LPLOG_DATA pLogData = 0;
LPBYTE pReciveBuffer = 0;
__declspec(naked) void __stdcall NormalReturn()
{
	__asm
	{
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		nop
		
		pushad
		mov eax, dword ptr [0x0044537c]
		mov dword ptr [pReciveBuffer], eax
		mov dx, word ptr [0x00445384]
		mov word ptr [nSize], dx
	}
	if ( 0x1000 > nSize )
	{
		pLogData = pLogDataPool->Allocate();
		wsprintfW(pLogData->wszTag, L"NormalReturn");
		pLogData->nSize = nSize;
		for (i = 0 ; i < pLogData->nSize ; i++ )
		{
			pLogData->szBuf[i] = pReciveBuffer[i];
		}
		pLogQueue->push(pLogData);
	}

	//pLogData = 0;
	__asm
	{
		popad
		pop esi
		ret
	}
}

__declspec(naked) void __stdcall ZeroReturn()
{
	__asm
	{
		pushad
		mov eax, dword ptr [0x0044537c]
		mov dword ptr [pReciveBuffer], eax
		mov dx, word ptr [0x004452bc]
		mov word ptr [nSize], dx
	}
	pLogData = pLogDataPool->Allocate();
	wsprintfW(pLogData->wszTag, L"ZeroReturn");
	pLogData->nSize = nSize;
	for (i = 0 ; i < pLogData->nSize ; i++ )
	{
		pLogData->szBuf[i] = pReciveBuffer[i];
	}
	pLogQueue->push(pLogData);
	//pLogData = 0;
	__asm
	{
		popad
		pop esi
		ret
	}
}

DWORD dwMyDecReturn = 0, dwFlag = 0x004452d0;
__declspec(naked) void __stdcall DecHook()
{
	__asm
	{
		mov dword ptr [nSize], edx
		pushad
		mov eax, 0x0044537c	// 03~~~~a4
		mov ebx, dword ptr [eax]
		mov dword ptr [pReciveBuffer], ebx
	}
	pLogData = pLogDataPool->Allocate();
	wsprintfW(pLogData->wszTag, L"DecHook");
	pLogData->nSize = nSize;
	for (i = 0 ; i < pLogData->nSize ; i++ )
	{
		pLogData->szBuf[i] = pReciveBuffer[i];
	}
	pLogQueue->push(pLogData);
	//pLogData = 0;
	__asm
	{
		popad
		jmp dwMyDecReturn
	}
}

DWORD dwDecReturn = 0x401951;
__declspec(naked) void __stdcall DecReturn()
{
	__asm
	{
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		xor eax, eax
		jmp dwDecReturn
	}
}

BYTE bytPing[] = { 0x1a, 0x09, 0x00, 0x5c, 0x00, 0x04, 0x00, 0x7b, 0x00 };
/*
BYTE bytShutDown[] = { 0x29, 0x0d, 0x00, 0xd9, 0x03 };
DWORD dwJmpCode = 0x4b0262;
__declspec(naked) void __stdcall RaiderZ_WSARecv()
{
	__asm
	{
		lea ecx, dword ptr [edi+0x30]
		push ecx
		push eax
		pushad
		mov eax, dword ptr [ecx + 4]
		mov dword ptr [pReciveBuffer], eax
	}
	nSize = *(LPWORD)(pReciveBuffer + 1);
	if ( 0x1000 > nSize )
	{
		if (13 == nSize)
		{
			if (0x29 == pReciveBuffer[0] && 0x81 == pReciveBuffer[7] && 0x1f == pReciveBuffer[8])
			{
				DBGPRINT("¼Ë´Ù¿îÆÐÅ¶ ¹Ù²Þ");
				for (i = 0 ; i < 13 ; i++)
				{
					pReciveBuffer[i] = bytPing[i];
				}
			}
		}
		pLogData = pLogDataPool->Allocate();
		swprintf_s(pLogData->wszTag, L"WSARecvReturn");
		pLogData->nSize = nSize;
		for (i = 0 ; i < pLogData->nSize ; i++ )
		{
			pLogData->szBuf[i] = pReciveBuffer[i];
		}
		pLogQueue->push(pLogData);
	}
	__asm
	{
		popad
		jmp dwJmpCode
	}
}
*/

DWORD dwJmpCode2 = 0x774630;
__declspec(naked) void __stdcall RaiderZ_recv()
{
	__asm
	{
		pushad
		mov dword ptr [nSize], eax
		lea eax, dword ptr [esi+0x78]
		mov dword ptr [pReciveBuffer], eax
	}
	//nSize = *(LPWORD)(pReciveBuffer + 1);
	if (13 == nSize)
	{
		if (0x1A == pReciveBuffer[0] && 0x71 == pReciveBuffer[7] && 0x39 == pReciveBuffer[8])
		{
			//DBGPRINT("¼Ë´Ù¿îÆÐÅ¶ ¹Ù²Þ");
			for (i = 0 ; i < 9 ; i++)
			{
				pReciveBuffer[i] = bytPing[i];
			}
			nSize = 9;
		}
		/*
		pLogData = pLogDataPool->Allocate();
		wsprintfW(pLogData->wszTag, L"recvReturn");
		pLogData->nSize = nSize;
		for (i = 0 ; i < pLogData->nSize ; i++ )
		{
			pLogData->szBuf[i] = pReciveBuffer[i];
		}
		pLogQueue->push(pLogData);
		*/
	}
	//DBGPRINT("nSize: %d", nSize);
	__asm
	{
		popad
		mov edi, dword ptr [nSize]
		cmp edi, 0xffffffff
		jmp dwJmpCode2
	}
}

void Hooks(HMODULE h)
{
	/*
	pLogQueue = new C_IOCP_QUEUE<LPLOG_DATA>();
	pLogDataPool = new C_OBJECT_POOL_DLF<LOG_DATA>((ULONG_PTR)1000);
	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)HookLogThread,0,0,0);
	*/
	
	DWORD dwProtect;
	/*
	DWORD dwRaiderZ_WSARecv = 0x4b025d;
	do 
	{
		if ( *(LPBYTE)(dwRaiderZ_WSARecv) == 0x8d )
		{
			break;
		}
		Sleep(10);
	} while (TRUE);

	
	VirtualProtect((LPVOID)dwRaiderZ_WSARecv, 5, PAGE_READWRITE, &dwProtect);
	*(LPBYTE)(dwRaiderZ_WSARecv) = 0xe9;
	*(LPDWORD)(dwRaiderZ_WSARecv + 1) = ((DWORD)RaiderZ_WSARecv-dwRaiderZ_WSARecv)-5;
	VirtualProtect((LPVOID)dwRaiderZ_WSARecv, 5, dwProtect, 0);
	*/

	DWORD dwRaiderZ_recv = 0x77462B;
	do 
	{
		if ( *(LPBYTE)(dwRaiderZ_recv) == 0x8b )
		{
			break;
		}
		Sleep(10);
	} while (TRUE);

	VirtualProtect((LPVOID)dwRaiderZ_recv, 5, PAGE_READWRITE, &dwProtect);
	*(LPBYTE)(dwRaiderZ_recv) = 0xe9;
	*(LPDWORD)(dwRaiderZ_recv + 1) = ((DWORD)RaiderZ_recv-dwRaiderZ_recv)-5;
	VirtualProtect((LPVOID)dwRaiderZ_recv, 5, dwProtect, 0);
}