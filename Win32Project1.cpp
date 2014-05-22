// Win32Project1.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Win32Project1.h"

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


void MainAssistent()
{

}

typedef struct _Tl2info
{
	int pid;
	HWND hwnd;
	WCHAR szWindName[MAX_LOADSTRING];
}L2INFO;
L2INFO l2info[1000];
typedef struct tagENUMINFO {
	DWORD   dwProcess;
	HWND    hwnd;
} ENUMINFO, *PENUMINFO;
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

 	// TODO: ���������� ��� �����.
	MSG msg;
	HACCEL hAccelTable;

	// ������������� ���������� �����
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// ���� ��������� ���������:
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
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
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
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   //��������� �������� ���������� textbox, listbox, buttons


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND	- ��������� ���� ����������
//  WM_PAINT	-��������� ������� ����
//  WM_DESTROY	 - ������ ��������� � ������ � ���������.
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
	HWND hResListBox;
	HWND hLogTextBox;
	int iCounter1 = 0;
	WCHAR szBuffer[1024];
	switch (message)
	{
	case WM_CREATE:
		//CreateWindowEx("button", _T("������ ���������"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 100, 100, 100, 20, hWnd, (HMENU)IDB_LISTL2PORC, NULL, NULL);
		CreateWindowExW(0,_T("button"), _T("������ ���������"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 0, 0, 200, 20, hWnd, (HMENU)IDB_LISTL2PORC, NULL, NULL);
		CreateWindowExW(0, _T("button"), _T("������ �������"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 0, 25, 200, 20, hWnd, (HMENU)IDB_SETSCRIPT, NULL, NULL);
		CreateWindowExW(0, _T("listbox"), _T("������ ���������"), WS_CHILD | WS_VSCROLL | WS_VISIBLE|ES_AUTOVSCROLL|WS_BORDER, 201, 0, 200, 400, hWnd, (HMENU)IDLB_LISTBOCPROC, NULL, NULL);
		CreateWindowExW(0, _T("edit"), _T("���"), WS_CHILD | WS_VSCROLL | WS_VISIBLE | ES_AUTOVSCROLL | WS_BORDER, 403, 0, 500, 400, hWnd, (HMENU)IDTB_LOGTEXTBOX, NULL, NULL);

		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case IDB_SETSCRIPT:
			hLogTextBox = GetDlgItem(hWnd, IDTB_LOGTEXTBOX);
			//��� ������ ����� �������� ������� ������
			l2info[0].hwnd = GetProcessWindow(l2info[0].pid);
			
			PostMessage(l2info[0].hwnd, WM_KEYDOWN, VK_F1, 0);
			
			SendMessage(hLogTextBox, EM_SETSEL, GetWindowTextLength(hLogTextBox), GetWindowTextLength(hLogTextBox));
			GetWindowText(l2info[0].hwnd, szBuffer, sizeof(szBuffer));
			SendMessage(hLogTextBox, EM_REPLACESEL, FALSE, (LPARAM)szBuffer);
			SendMessage(hLogTextBox, EM_SCROLLCARET, 0, 0);
			break;
		case IDB_LISTL2PORC:
			//�������� ������ ���������
			hResListBox = GetDlgItem(hWnd, IDLB_LISTBOCPROC);
			hLogTextBox = GetDlgItem(hWnd, IDTB_LOGTEXTBOX);
			SendMessage(hResListBox, LB_RESETCONTENT, 0, 0);
			hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			peEntry.dwSize = sizeof(PROCESSENTRY32);
			Process32First(hSnapShot, &peEntry);

			do
			{
				
				if (!wcscmp(_T("l2.exe"), peEntry.szExeFile))
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
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �������� ����� ��� ���������...
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

// ���������� ��������� ��� ���� "� ���������".
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
