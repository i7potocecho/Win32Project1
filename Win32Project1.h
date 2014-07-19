#pragma once

#include "resource.h"

void SendTextToEdit(HWND ,WCHAR*);
HWND hLogTextBox=NULL;
HWND hLabelInfo = NULL;
HWND hMyProg;
HDC hdcPaintArea = NULL;
HDC hdcMyProg;
RECT rectMyClient;
RECT rectTargetBox;
BOOL CALLBACK LookChildWindow(HWND, LPARAM);
HWND GetProcessWindow(IN DWORD dwProcessId);
HMENU hContextMenuForListProc=NULL;
HWND hResListBox=NULL;
int iCounter1 = 0;
HANDLE hSnapShot;
PROCESSENTRY32 peEntry;

PAINTSTRUCT ps;


