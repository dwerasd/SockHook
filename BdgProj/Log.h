#pragma once


#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "DarkLock.h"


#define MAX_LOG_BUFFER	(1 << 15)


class C_LOG
{
private:
	HANDLE hFile;
	C_CRITICAL_SECTION csWrite;

	WCHAR wszLogPath[MAX_LOG_BUFFER];

public:
	C_LOG();
	~C_LOG();

	void Init(LPSTR ptszPath, LPSTR ptszLogName);
	void Init(LPWSTR ptszLogName);
	void Init(LPSTR s);
	void Write(LPWSTR ptszMessage, ...);
	void Write(LPSTR ptszMessage, ...);
};

extern C_LOG *pLog;