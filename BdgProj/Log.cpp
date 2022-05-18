#include "stdafx.h"
#include "Log.h"



C_LOG *pLog = NULL;

C_LOG::C_LOG() : hFile(INVALID_HANDLE_VALUE)
{

}

C_LOG::~C_LOG()
{

}

void C_LOG::Init(LPSTR ptszPath, LPSTR ptszLogName)
{
	char szLogPath[MAX_PATH];
	wsprintfA(szLogPath, "%s%s", ptszPath, ptszLogName);
	//OutputDebugStringA(szLogPath);

	DWORD len = strlen(szLogPath) + 1;
	MultiByteToWideChar(CP_ACP, 0, szLogPath, len, wszLogPath, len);

	//OutputDebugStringW(wszLogPath);
	if (!PathIsDirectoryW(wszLogPath))
	{
		CreateDirectoryW(wszLogPath, NULL);
	}
}

void C_LOG::Init(LPWSTR pwszLogName)
{
	::GetCurrentDirectoryW(sizeof(wszLogPath), wszLogPath);
	wcscat_s(wszLogPath, L"\\");
	wcscat_s(wszLogPath, pwszLogName);
	// �α� ������ ������ �����Ѵ�
	if (!PathIsDirectoryW(wszLogPath))
	{
		CreateDirectoryW(wszLogPath, NULL);
	}
}

void C_LOG::Init(LPSTR s)
{
	WCHAR wszBuf[MAX_PATH];
	DWORD len = strlen(s) + 1;
	MultiByteToWideChar(CP_ACP, 0, s, len, wszBuf, len);
	Init(wszBuf);
}

void C_LOG::Write(LPSTR lpszFormat, ...)
{
	char szBuffer[MAX_LOG_BUFFER];
	va_list fmtList = NULL;
	va_start(fmtList, lpszFormat);
	_vsnprintf_s(szBuffer, MAX_LOG_BUFFER - 1, MAX_LOG_BUFFER - 1, lpszFormat, fmtList);
	va_end(fmtList);

	WCHAR wszBuf[MAX_PATH];
	DWORD len = strlen(szBuffer) + 1;
	MultiByteToWideChar(CP_ACP, 0, szBuffer, len, wszBuf, len);

	Write(wszBuf);
}
void C_LOG::Write(LPWSTR lpszFormat, ...)
{
	if (0 != wszLogPath[0])
	{
		csWrite.Enter();
		// ���� ��¥�� ����
		tm t;
		time_t tToday = time(0);
		localtime_s(&t, &tToday);
		WCHAR wszDate[1 << 5];
		wcsftime(wszDate, sizeof(wszDate), L"%Y-%m-%d", &t);
		// \$(LogPath)\$(today).txt ���Ͽ� �α׸� ����

		WCHAR wszSavePath[MAX_PATH];
		wsprintfW(wszSavePath, L"%s\\%s.txt", wszLogPath, wszDate);

		// ������ ������ ���� ������ �����Ѵ�.
		WIN32_FIND_DATA finddata;
		BOOL bExistFile = (INVALID_HANDLE_VALUE != FindFirstFile(wszSavePath, &finddata));
		DWORD dwCreationDisposition = bExistFile ? OPEN_ALWAYS : CREATE_ALWAYS;
		hFile = CreateFile(wszSavePath, GENERIC_WRITE, NULL, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
		// ������ ��������
		if (INVALID_HANDLE_VALUE != hFile)
		{
			DWORD dwWritten = 0;
			if (FALSE == bExistFile)	// ���� �����ѰŸ�
			{
				BYTE buf[2] = { 0xff, 0xfe };
				WriteFile(hFile, buf, 2, &dwWritten, NULL);
			}
			WCHAR wszBuffer[MAX_LOG_BUFFER] = { 0 };
			//ZeroMemory(wszBuffer, MAX_LOG_BUFFER);
			// �޽����� tszBuffer �� �ۼ��ϰ�
			va_list fmtList;
			va_start(fmtList, lpszFormat);
			_vsnwprintf_s(wszBuffer, MAX_LOG_BUFFER - 1, MAX_LOG_BUFFER - 1, lpszFormat, fmtList);
			va_end(fmtList);
			
			//DBGPRINT(L"%s", wszBuffer);

			// ��¥�� �ð��� ����. 
			WCHAR wszTimeStamp[1 << 5];	// 32
			wcsftime(wszTimeStamp, sizeof(wszTimeStamp), L"%Y-%m-%d %H:%M:%S", &t);
			//////////////////////////////////////////////////////////////////////////
			// $(date)$(time)  $(data)\r\n ���� �ۼ��Ѵ�
			//////////////////////////////////////////////////////////////////////////
			WCHAR wszSaveBuffer[MAX_LOG_BUFFER] = { 0 };
			//ZeroMemory(wszSaveBuffer, MAX_LOG_BUFFER);
			swprintf_s(wszSaveBuffer, L"(%s) %s\r\n", wszTimeStamp, wszBuffer);

			/*
			WCHAR wsztmp[MAX_PATH];
			wsprintfW(wsztmp, L"%d", wcslen(wszBuffer));
			OutputDebugStringW(wsztmp);
			*/
			// ������ ���� ���� ����.
			if (INVALID_SET_FILE_POINTER != SetFilePointer(hFile, 0, NULL, FILE_END))
			{
				DWORD dwWritten = 0;
				WriteFile(hFile, wszSaveBuffer, wcslen(wszSaveBuffer) * 2, &dwWritten, NULL);
			}
			CloseHandle(hFile);
		}
		csWrite.Leave();
	}
}
