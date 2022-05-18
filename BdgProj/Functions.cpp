#include "stdafx.h"
#include "Functions.h"



ULONG FindPattern(ULONG dwEntry , ULONG dwMemSize , LPBYTE Pattern, long dwpSize)
{
	ULONG Addr = NULL;
	__asm
	{
		mov eax, dwEntry
		add eax, dwMemSize
		mov edx, eax
		mov ebx, dwpSize
		sub edx, ebx
		mov ecx, ebx
		mov esi, dwEntry
		mov edi, Pattern
Tracing:
		mov al, byte ptr [edi]
		cmp al, 0x91
		je NextCheck
		cmp al, byte ptr [esi]
		jne Restore
NextCheck:
		inc esi
		inc edi
		dec ecx
		cmp ecx, 0
		jne Tracing
		mov eax, ebx
		sub eax, ecx
		sub esi, eax
		mov eax, esi
		jmp EndLoop
Restore:
		mov eax, ebx
		sub eax, ecx
		sub esi, eax
		sub edi, eax
		mov ecx, ebx
		add esi, 1
		cmp esi, edx
		jne Tracing
EndLoop:
		mov dword ptr [Addr], eax
	}
	return Addr;
}
