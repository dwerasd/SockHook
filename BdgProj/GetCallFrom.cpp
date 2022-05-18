#include "stdafx.h"
#include "GetCallFrom.h"
#include "Log.h"



DWORD DetourCallFrom(LPBYTE src, const LPBYTE dst, const int len)
{
	LPBYTE ppSrc = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, len+5);
	if ( src && dst && len )
	{
		DWORD dwProtect = 0;
		//LPBYTE ppSrc = (LPBYTE)malloc(len+5);	
		if ( FALSE != VirtualProtect((LPVOID)src, 5, PAGE_READWRITE, &dwProtect ) )
		{
			pLog->Write("Hook: %x ", src);
			//memcpy((LPVOID)ppSrc, (LPVOID)src, len);		// 원래 코드를 보관하고
			for (long i = 0 ; i < len; i++)
			{
				ppSrc[i] = src[i];
			}
			ppSrc[len] = 0xE9;				// ppSrc+5바이트 한 곳으로 점프하는 코드를 생성
			*(LPDWORD)(ppSrc+len+1) = (DWORD)((LPBYTE)src-ppSrc)-5;
			//LPBYTE jmp = (LPBYTE)malloc(5+6);
			LPBYTE jmp = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 5+6);	
			jmp[0] = 0x58;	// pop eax
			jmp[1] = 0x83;	// sub
			jmp[2] = 0xEC;	// esp,
			jmp[3] = 0x04;	// 0x04
			jmp[4] = 0x50;	// push eax			리턴 주소를 넣으면 인자로 함수에서 받을 수 있음.
			jmp[5] = 0xE9;	// jmp
			*(LPDWORD)(jmp+6) = (DWORD)((LPBYTE)dst-jmp)-10;	// MySend
			src[0] = 0xE9;									// jmp
			*(LPDWORD)(src+1) = (DWORD)((jmp-len) - src);	// ppSrc
			VirtualProtect((LPVOID)src, 5, dwProtect, NULL );
		}
	}
	return (DWORD)ppSrc;
}



typedef int ( __stdcall *MessageBoxA_PROC ) (HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
MessageBoxA_PROC pMessageBoxA = NULL;

int __stdcall MyMessageBoxA(DWORD dwCallFrom, HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	pLog->Write("MyMessageBoxA: %x %X %s %s %d", dwCallFrom, hWnd, lpText, lpCaption, uType);
	return pMessageBoxA(hWnd, lpText, lpCaption, uType);	// 원래 코드를 호출해줌.
}

typedef int ( __stdcall *MessageBoxW_PROC ) (HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);
MessageBoxW_PROC pMessageBoxW = NULL;

int __stdcall MyMessageBoxW(DWORD dwCallFrom, HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	pLog->Write(L"MyMessageBoxW: %x %X %s %s %d", dwCallFrom, hWnd, lpText, lpCaption, uType);
	return pMessageBoxW(hWnd, lpText, lpCaption, uType);	// 원래 코드를 호출해줌.
}

typedef BOOL (__stdcall *ReadProcessMemory_PROC)(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesRead);
ReadProcessMemory_PROC pReadProcessMemory = NULL;

int __stdcall MyReadProcessMemory(DWORD dwCallFrom, HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesRead)
{
	pLog->Write("MyReadProcessMemory: %x %X %s %s %d %x", dwCallFrom, hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
	return pReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);	// 원래 코드를 호출해줌.
}

typedef BOOL (__stdcall *WriteProcessMemory_PROC)(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesWritten );
WriteProcessMemory_PROC pWriteProcessMemory = NULL;

int __stdcall MyWriteProcessMemory(DWORD dwCallFrom, HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesWritten)
{
	pLog->Write("MyWriteProcessMemory: %x %X %s %s %d %x", dwCallFrom, hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
	return pWriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);	// 원래 코드를 호출해줌.
}

void HookCallFrom()
{
	HMODULE hUser32 = (HMODULE)GetModuleHandle(_T("user32.dll"));
	if (0 != hUser32)
	{
		pMessageBoxA = (MessageBoxA_PROC)DetourCallFrom((LPBYTE)GetProcAddress(hUser32,"MessageBoxA"), (LPBYTE)MyMessageBoxA, 5);
		pMessageBoxW = (MessageBoxW_PROC)DetourCallFrom((LPBYTE)GetProcAddress(hUser32,"MessageBoxW"), (LPBYTE)MyMessageBoxW, 5);
	}
	HMODULE hKernel32 = (HMODULE)GetModuleHandle(_T("kernel32.dll"));
	if (0 != hKernel32)
	{
		pReadProcessMemory = (ReadProcessMemory_PROC)DetourCallFrom((LPBYTE)GetProcAddress(hKernel32,"ReadProcessMemory"), (LPBYTE)MyReadProcessMemory, 5);
		pWriteProcessMemory = (WriteProcessMemory_PROC)DetourCallFrom((LPBYTE)GetProcAddress(hKernel32,"WriteProcessMemory"), (LPBYTE)MyWriteProcessMemory, 5);
	}
	pLog->Write("hUser32: %x / hKernel32: %x", hUser32, hKernel32);
	//pLog->Write("%x %x %x %x", pMessageBoxA, pMessageBoxW, pReadProcessMemory, pWriteProcessMemory);
}

