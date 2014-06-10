#pragma once

#include "resource.h"

void SendTextToEdit(HWND ,WCHAR*);
HWND hLogTextBox=NULL;
HWND hLabelInfo = NULL;
HWND hMyProg;
HDC hdcPaintArea = NULL;
RECT rectMyClient;
RECT rectTargetBox;
BOOL CALLBACK LookChildWindow(HWND, LPARAM);


