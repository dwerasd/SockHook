// BdgProj.cpp : DLL ���� ���α׷��� ���� �������� �����մϴ�.
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
				strcpy(fName, strrchr(fPath,'\\')+1);					// �ڽ� �̸� ���
				strncpy(DllPath, fPath, strlen(fPath) - strlen(fName));	// ��� ����
				
				OutputDebugStringA(fPath);
				OutputDebugStringA(fName);
				OutputDebugStringA(DllPath);

				pLog = new C_LOG();
				pLog->Init(DllPath, "pklog");
				pLog->Write(L"��");

				//HookCallFrom();

				//pCreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ReciveLogThread,0,0,0);	// �α� ������ ����.
				//recvHook();
				
				pCreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Hooks, (LPVOID)hModule,0,0);
			}
			else
			{
				OutputDebugStringA("��ãũ��");
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

