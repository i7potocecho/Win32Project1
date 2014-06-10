// Win32Project1.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Win32Project1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

bool bVK_KEYS[256];
BYTE bKeys[256];

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

typedef struct _Tl2Command
{
	UINT uiCommId;
	WCHAR szCommType;
	DWORD dwTimer;
	DWORD dwKeyPressed;
	WCHAR wcParam[MAX_LOADSTRING];
	void (*pCommand)(int id,...);
	vector<BYTE>vbKeys;
}L2COMM,*PL2COMM;
typedef vector<L2COMM> lCommand;
lCommand tempcomm[100];
L2COMM tempcom;


typedef struct _Tl2info
{
	int pid;
	HWND hwnd;
	WCHAR szWindName[MAX_LOADSTRING];
	lCommand *lComm;
}L2INFO,*PL2INFO;
L2INFO l2info[100];
typedef struct tagENUMINFO {
	DWORD   dwProcess;
	HWND    hwnd;
} ENUMINFO, *PENUMINFO;
//запуск в потоке 
void StartMacros(L2INFO *pl2inf)
{
	L2INFO l2inf = *pl2inf;
	vector<DWORD>starttimer;
	vector<DWORD>curtimer;
	//Инициализация таймеров
	
	int counterl2com = 0;
	for(vector<L2COMM>::iterator iter = l2inf.lComm->begin();iter != l2inf.lComm->end();++iter)
	{
		starttimer.push_back(GetTickCount());
		curtimer.push_back(GetTickCount());
		counterl2com++;
	}
	L2COMM tempcomm1 ;
	
	while (true)
	{
		for(int i = 0;i<counterl2com;i++)
		{
			curtimer[i] = GetTickCount();
			
			tempcomm1 = (*l2inf.lComm)[i];
			if((curtimer[i] - starttimer[i]) >= tempcomm1.dwTimer)
			{
				starttimer[i] = GetTickCount();
				//выполнить команду

				if(tempcomm1.uiCommId == 1)
					tempcomm1.pCommand(0, l2inf,&tempcomm1);
				else if(tempcomm1.uiCommId == 2);

				tempcomm1.pCommand(0, &l2inf, &tempcomm1);

				SendTextToEdit(hLogTextBox,tempcomm1.wcParam);
			}
		}
		
		if(GetAsyncKeyState(VK_ESCAPE)&0x8000)break;
		
		
	}
	_endthreadex(0);
}
void CommPress(int startindex,...)
{
	//type id 1

	L2INFO l2handle = va_arg(startindex,L2INFO);
	L2COMM l2com = va_arg(startindex,L2COMM);
	PostMessage(l2handle.hwnd, WM_KEYDOWN, l2com.dwKeyPressed, 0);

	va_list marker;
	va_start(marker, startindex);
	L2INFO *pl2info = va_arg(marker, L2INFO*);
	L2COMM* pl2comm = va_arg(marker, L2COMM*);
	va_end(marker);
	PostMessage(pl2info->hwnd, WM_KEYDOWN,pl2comm->vbKeys[0] , 0);
}
void IsTarget(int startindex, ...)
{
	va_list marker;
	va_start(marker, startindex);
	L2INFO *pl2info = va_arg(marker, L2INFO*);
	L2COMM* pl2comm = va_arg(marker, L2COMM*);
	va_end(marker);

}
void NotTarget(int startindex, ...)
{
	va_list marker;
	va_start(marker, startindex);
	L2INFO *pl2info = va_arg(marker, L2INFO*);
	L2COMM* pl2comm = va_arg(marker, L2COMM*);
	va_end(marker);
	
		
	HDC hdcmain = GetDC(pl2info->hwnd);
	hdcPaintArea = CreateCompatibleDC(hdcmain);
	BITMAPINFOHEADER bmpInfoHeader;
	BITMAPINFO bmpInfo;	
	UINT bitmap_dx = 175;
	UINT bitmap_dy = 50;
	bmpInfoHeader.biSize = sizeof(bmpInfoHeader);
	bmpInfoHeader.biWidth = bitmap_dx;
	bmpInfoHeader.biHeight = bitmap_dy;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = bitmap_dx*bitmap_dy*(24 / 8);
	bmpInfoHeader.biXPelsPerMeter = 0;
	bmpInfoHeader.biYPelsPerMeter = 0;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biClrImportant = 0;
	BYTE*memory;
	//HBITMAP hBitmap = CreateDIBSection(hdcPaintArea, &bmpInfo, DIB_RGB_COLORS, (void**)&memory, NULL, 0);
	//SelectObject(hdcPaintArea, hBitmap);
	StretchBlt(hdcPaintArea, 0, 0, bitmap_dx, bitmap_dy, hdcmain, 0, 0, bitmap_dx, bitmap_dy, SRCCOPY);
	ReleaseDC(pl2info->hwnd, hdcmain);

}
void CommCheck(int startindex,...)
{
	//type id 2
	//проверить значение если тру то выполнить команду	
	L2INFO l2handle = va_arg(startindex,L2INFO);
	L2COMM l2com = va_arg(startindex,L2COMM);
}
void CheckRectOnScreen(int startindex,...)
{

}
BOOL CALLBACK LookChildWindow(HWND hwnd, LPARAM lparam)
{
	WCHAR  pwcTextBuffer[256];
	GetWindowText(hwnd, pwcTextBuffer, sizeof(pwcTextBuffer));
	SendTextToEdit(hLogTextBox, pwcTextBuffer);
	return TRUE;
}
void SendTextToEdit(HWND hwnd,WCHAR*buf)
{
	if(hwnd !=NULL)
	{
		SendMessage(hwnd, EM_SETSEL, GetWindowTextLength(hwnd), GetWindowTextLength(hwnd));
		SendMessage(hwnd, EM_REPLACESEL, FALSE, (LPARAM)buf);
		SendMessage(hwnd, EM_SCROLLCARET, 0, 0);
	}
}
static BOOL CALLBACK EnumProc(	IN HWND hWnd,	IN LPARAM lParam	)
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	PENUMINFO pInfo = (PENUMINFO)lParam;
	if (pInfo->dwProcess == dwProcessId)
	{
		pInfo->hwnd = hWnd;
		return FALSE;
	}

	return TRUE;
}
HWND GetProcessWindow(IN DWORD dwProcessId)
{
	ENUMINFO info;
	info.dwProcess = dwProcessId;
	info.hwnd = NULL;

	EnumWindows(EnumProc, (LPARAM)&info);

	return info.hwnd;
}
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPTSTR    lpCmdLine,_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}
//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   //Добавляем элементы управления textbox, listbox, buttons
   hMyProg = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HANDLE hSnapShot;
	PROCESSENTRY32 peEntry;
	TCHAR tchBuffer[1024];
	HWND hResListBox=NULL;
	int iCounter1 = 0;
	WCHAR szBuffer[1024];	
	TCHAR *strTemp1;
	
	switch (message)
	{
	case WM_CREATE:
		//CreateWindowEx("button", _T("Список процессов"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 100, 100, 100, 20, hWnd, (HMENU)IDB_LISTL2PORC, NULL, NULL);
		CreateWindowExW(0,_T("button"), _T("Список процессов"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 0, 0, 200, 20, hWnd, (HMENU)IDB_LISTL2PORC, NULL, NULL);
		CreateWindowExW(0, _T("button"), _T("Запуск скрипта"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 0, 25, 200, 20, hWnd, (HMENU)IDB_SETSCRIPT, NULL, NULL);
		CreateWindowExW(0, _T("listbox"), _T("Список процессов"), WS_CHILD | WS_VSCROLL | WS_VISIBLE|ES_AUTOVSCROLL|WS_BORDER, 201, 0, 200, 400, hWnd, (HMENU)IDLB_LISTBOCPROC, NULL, NULL);
		CreateWindowExW(0, _T("edit"), _T("Лог"), WS_CHILD | WS_VSCROLL | WS_VISIBLE | ES_AUTOVSCROLL | WS_BORDER, 403, 0, 500, 400, hWnd, (HMENU)IDTB_LOGTEXTBOX, NULL, NULL);
		CreateWindowExW(0, _T("static"), _T("label"), WS_CHILD |  WS_VISIBLE | WS_TABSTOP | WS_BORDER, 907, 0, 100, 100, hWnd, (HMENU)IDL_MOUSECOORDINFO, NULL, NULL);

		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDB_SETSCRIPT:
			hLogTextBox = GetDlgItem(hWnd, IDTB_LOGTEXTBOX);
			//для начала будем отсылать нажаите кнопки
			l2info[0].hwnd = GetProcessWindow(l2info[0].pid);
			
			tempcom.dwTimer = 120000;
			tempcom.pCommand = CommPress;
			tempcom.szCommType = 1;
			tempcom.uiCommId = 1;
			strTemp1 = _T("\nParametr");
			memcpy(tempcom.wcParam,strTemp1,sizeof(strTemp1)*8);

			tempcom.dwKeyPressed = VK_F1;

			tempcom.vbKeys.push_back(VK_F1);

			tempcomm[0].push_back(tempcom);
			tempcom.dwTimer = 3000;
			tempcom.pCommand = CommPress;
			tempcom.szCommType = 1;
			tempcom.uiCommId = 2;
			strTemp1 = _T("\nparam2");
			memcpy(tempcom.wcParam,strTemp1,sizeof(strTemp1)*6);

			tempcom.dwKeyPressed = VK_F2;

			tempcom.vbKeys.push_back(VK_OEM_5);

			tempcomm[0].push_back(tempcom);
			l2info[0].lComm = &tempcomm[0];
			GetWindowText(l2info[0].hwnd, szBuffer, sizeof(szBuffer));
			SendTextToEdit(hLogTextBox,szBuffer);


			//проверка функций

			NotTarget(0, &l2info[0], &l2info[0].lComm[0]);


			//CreateThread(NULL,0,&StartMacros,0,0,NULL);
			//_beginthreadex(NULL,0,(unsigned int(_stdcall*)(void*))StartMacros,&l2info[0],0,0);
			break;
		case IDB_LISTL2PORC:
			//Получеам список процессов
			hResListBox = GetDlgItem(hWnd, IDLB_LISTBOCPROC);
			hLogTextBox = GetDlgItem(hWnd, IDTB_LOGTEXTBOX);
			hLabelInfo = GetDlgItem(hWnd,IDL_MOUSECOORDINFO);
			//SetWindowTextW(hLabelInfo,_T("LABEL"));
			SendMessage(hResListBox, LB_RESETCONTENT, 0, 0);
			hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			peEntry.dwSize = sizeof(PROCESSENTRY32);
			Process32First(hSnapShot, &peEntry);

			do
			{
				
				if (!wcscmp(_T("calc.exe"), peEntry.szExeFile))
				{
					l2info[iCounter1].pid = peEntry.th32ProcessID;
					memcpy(l2info[iCounter1].szWindName, peEntry.szExeFile, sizeof(peEntry.szExeFile));
					SendMessage(hResListBox, LB_ADDSTRING, 0, (LPARAM)(l2info[iCounter1].szWindName));
					iCounter1++;
				}
			} while (Process32Next(hSnapShot,&peEntry));
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		bVK_KEYS[(BYTE)wParam] = true;
		break;
	case WM_KEYUP:
		bVK_KEYS[(BYTE)wParam] = false;
		break;
	case WM_PAINT:
		GetClientRect(hMyProg, &rectMyClient);
		hdc = BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...
		if (hdcPaintArea !=NULL)
			StretchBlt(hdc, rectMyClient.left+10, rectMyClient.bottom-10-50, 175, 50, hdcPaintArea, 0, 0, 175, 50, SRCCOPY);
		MoveToEx(hdc, rectMyClient.left + 10, rectMyClient.bottom - 10 - 50, NULL);
		LineTo(hdc, rectMyClient.left + 10 + 175, rectMyClient.bottom - 10 - 50);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
