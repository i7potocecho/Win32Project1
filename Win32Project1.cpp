// Win32Project1.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "Win32Project1.h"

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����

bool bVK_KEYS[256];
BYTE bKeys[256];

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

typedef struct _Tl2Command
{
	UINT uiCommId;
	WCHAR szCommType;
	DWORD dwTimer;
	WCHAR wcParam[MAX_LOADSTRING];
	void (*pCommand)(int id,...);
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
//������ � ������ 
void StartMacros(L2INFO *pl2inf)
{
	L2INFO l2inf = *pl2inf;
	vector<DWORD>starttimer;
	vector<DWORD>curtimer;
	//������������� ��������
	
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
				//��������� �������
				SendTextToEdit(hLogTextBox,tempcomm1.wcParam);
			}
		}
		//����� ������� ������ �� ������
		if(GetAsyncKeyState(VK_ESCAPE)&0x8000)break;
		
		
	}
	_endthreadex(0);
}
void CommPress(int startindex,...)
{
	//type id 1
	HWND handle = va_arg(startindex,HWND);
	DWORD VK_KEY = va_arg(startindex,DWORD);
	PostMessage(handle, WM_KEYDOWN, VK_KEY, 0);
}
void CommCheck(int startindex,...)
{
	//type id 2
	//��������� �������� ���� ��� �� ��������� �������	

}
void CheckRectOnScreen(int startindex,...)
{

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
	HWND hResListBox=NULL;
	
	int iCounter1 = 0;
	WCHAR szBuffer[1024];
	
	
	TCHAR *strTemp1;
	
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
			
			tempcom.dwTimer = 200;
			tempcom.pCommand = CommPress;
			tempcom.szCommType = 1;
			tempcom.uiCommId = 1;
			strTemp1 = _T("\nParametr");
			memcpy(tempcom.wcParam,strTemp1,sizeof(strTemp1)*8);
			tempcomm[0].push_back(tempcom);
			tempcom.dwTimer = 300;
			tempcom.pCommand = CommPress;
			tempcom.szCommType = 1;
			tempcom.uiCommId = 2;
			strTemp1 = _T("\nparam2");
			memcpy(tempcom.wcParam,strTemp1,sizeof(strTemp1)*6);
			tempcomm[0].push_back(tempcom);
			l2info[0].lComm = &tempcomm[0];
			GetWindowText(l2info[0].hwnd, szBuffer, sizeof(szBuffer));
			SendTextToEdit(hLogTextBox,szBuffer);
			//CreateThread(NULL,0,&StartMacros,0,0,NULL);
			_beginthreadex(NULL,0,(unsigned int(_stdcall*)(void*))StartMacros,&l2info[0],0,0);
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
	case WM_KEYDOWN:
		bVK_KEYS[(BYTE)wParam] = true;
		break;
	case WM_KEYUP:
		bVK_KEYS[(BYTE)wParam] = false;
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
