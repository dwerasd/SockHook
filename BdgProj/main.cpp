// BdgProj.cpp : DLL 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Hooks.h"
#include "GetCallFrom.h"



char fPath[MAX_PATH], fName[MAX_PATH], DllPath[MAX_PATH];


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);
			typedef HANDLE (__stdcall *CreateThread_PROC) ( LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD );
			CreateThread_PROC pCreateThread = (CreateThread_PROC)GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateThread");
			if ( 0 != pCreateThread )
			{
				char fPath[MAX_PATH] = {0}, fName[MAX_PATH] = {0}, DllPath[MAX_PATH] = {0};
				GetModuleFileNameA(hModule, fPath, MAX_PATH);
				strcpy(fName, strrchr(fPath,'\\')+1);					// 자신 이름 얻고
				strncpy(DllPath, fPath, strlen(fPath) - strlen(fName));	// 경로 얻음
				
				OutputDebugStringA(fPath);
				OutputDebugStringA(fName);
				OutputDebugStringA(DllPath);

				pLog = new C_LOG();
				pLog->Init(DllPath, "pklog");
				pLog->Write(L"됨");

				//HookCallFrom();

				//pCreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReciveLogThread,0,0,0);	// 로그 스레드 시작.
				//recvHook();
				
				pCreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Hooks, (LPVOID)hModule,0,0);
			}
			else
			{
				OutputDebugStringA("못찾크스");
			}
		}
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

