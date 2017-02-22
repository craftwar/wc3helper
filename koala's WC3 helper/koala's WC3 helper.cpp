#include "stdafx.h"
#include "koala's WC3 helper.h"


#include <shellapi.h>
#include <process.h>
#include "globaldata.h"
//extern "C" {
//#include "..\hook\hook.h"
//}
#include "MyContent.h"
#include "util.h"
#include "PasteBot.h"



using namespace std;

//�ϥ�_snsprintf
//#pragma warning(disable : 4995)




// �����ܼ�:
HWND hWnd;										// �D�{��window
HINSTANCE hInst;								// �ثe�������
HWND  hwndChat = NULL;							// Chat dialog HWND
HMENU hMenu;
HMENU hTrayMenu;								// TrayIcon Menu
NOTIFYICONDATA tnid;							// TrayIcon
SO_handles g_SO;								// �޲zshared memory
Settings cfg;									// ����bshared memory���]�w

HANDLE hSemaphore;								// ����one instance per session �M cirical session

#ifndef NO_Semaphore

HANDLE hReadSemaphore;							// �����g�J SO.lpMemSO->hWar3, SO.lpMemSO->hHook
HANDLE hWriteSemaphore;							// �����g�J SO.lpMemSO->hWar3, SO.lpMemSO->hHook

#endif

HANDLE hCatchWar3Thread;						// ��War3��thread


HMODULE hinstDLLHook;							// hook.dll


_TWar3DataList War3DataList;




PasteBot myPasteBot;




//hook related/*
HHOOK g_hhookLLKbd;
class Action *g_action;







// �o�ӵ{���X�Ҳդ��ҥ]�t���禡���V�e�ŧi:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);





int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	MSG msg;
	//HACCEL hAccelTable;	// �S�Ψ�

	//���� one instance in per session
	hSemaphore = CreateSemaphore(NULL, 1, 1, TEXT("Local\\koala's WC3 helper"));
	//if ( hMutex == NULL || ERROR_ALREADY_EXISTS == GetLastError() )
	if ( hSemaphore == NULL || ERROR_ALREADY_EXISTS == GetLastError() )
		_exit(EXIT_FAILURE);

#ifndef NO_Semaphore
	hReadSemaphore = CreateSemaphore(NULL, 0, 1, TEXT("Local\\koala's WC3 helperR"));
	hWriteSemaphore = CreateSemaphore(NULL, 1, 1, TEXT("Local\\koala's WC3 helperW"));
#endif



	MyRegisterClass(hInstance);

	// �������ε{����l�]�w:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KOALASWC3HELPER));

	// �D�T���j��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//if (!IsDialogMessage(hwndChat, &msg) && !TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		if ( !IsDialogMessage(hwndChat, &msg) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �禡: MyRegisterClass()
//
//  �γ~: ���U�������O�C
//
//  ����:
//
//    �u����z�Ʊ榹�{���X��ۮe�� Windows 95 �� 
//    'RegisterClassEx' �禡�󦭪� Win32 �t�ήɡA
//    �~�|�ݭn�[�J�Ψϥγo�Ө禡�C
//    �z�����I�s�o�Ө禡�A�����ε{�����o�P�������� 
//    �u�榡���T�v���ϥܡC
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
	//	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KOALASWC3HELPER));
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("koala's WC3 helper");
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �禡: InitInstance(HINSTANCE, int)
//
//   �γ~: �x�s������鱱��N�X�åB�إߥD����
//
//   ����:
//
//        �b�o�Ө禡���A�ڭ̷|�N������鱱��N�X�x�s�b�����ܼƤ��A
//        �åB�إߩM��ܥD�{�������C
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	hInst = hInstance;		// �N������鱱��N�X�x�s�b�����ܼƤ�
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	si.cb = sizeof(STARTUPINFO);


	//hWnd = CreateWindow(TEXT("koala's WC3 helper"), TEXT("koala's WC3 helper"), WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	//message only window
	hWnd = CreateWindow(TEXT("koala's WC3 helper"), TEXT("koala's WC3 helper"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, HWND_MESSAGE, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	g_SO.lpMemSO->hHelper = hWnd;
	g_SO.lpMemSO->HelperPID = GetCurrentProcessId();
	myPasteBot.SetOwner(hWnd);
	Action::SetHHelper(hWnd);





	//MSXML  SAX����
	//Ū�]�w


	//CoInitialize(NULL);
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	ISAXXMLReader* pRdr = NULL;


	//CLSCTX_ALL->CLSCTX_FROM_DEFAULT_CONTEXT�n���|�����D
	//HRESULT hr = CoCreateInstance(
	//	__uuidof(SAXXMLReader60),
	//	NULL,
	//	CLSCTX_ALL,
	//	__uuidof(ISAXXMLReader),
	//	(void **)&pRdr);

	HRESULT hr = CoCreateInstance(
		__uuidof(SAXXMLReader60),
		NULL,
		//CLSCTX_LOCAL_SERVER,
		//CLSCTX_INPROC_SERVER,
			CLSCTX_ALL,
		__uuidof(ISAXXMLReader),
		(void **)&pRdr);

	if(!FAILED(hr))
	{
		MyContent * pMc = new MyContent(g_SO, cfg, g_action);
		hr = pRdr->putContentHandler(pMc);

		if (!FAILED(hr))


		hr = pRdr->parseURL( TEXT("koala's WC3 helper.xml"));

		pRdr->Release();
	}

	CoUninitialize();


	//��War3Exe���|
	//TrayIcon
	HKEY	hWar3Key;			// registry of Warcraft III
	TCHAR * War3XPath;			// Frozen Throne.exe���|
	DWORD PathLen=MAX_PATH+1;	// ���|�����ܼ�
	RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Blizzard Entertainment\\Warcraft III"), 0, KEY_QUERY_VALUE, &hWar3Key);
	//RegGetValue WinXP 64bit/Vista + only
	//RegGetValue(HKEY_CURRENT_USER, TEXT("Software\\Warcraft III"), TEXT("ProgramX"), RRF_RT_ANY, NULL, War3XPath, &PathLen);
	War3XPath = new TCHAR[PathLen];
	PathLen *= sizeof(TCHAR);
	//RegQueryValueEx buffer�e�q��bytes
	RegQueryValueEx(hWar3Key, TEXT("ProgramX"), NULL, NULL, (LPBYTE) War3XPath, &PathLen);
	PathLen /= sizeof(TCHAR);


	//RunWar3
	BOOL bRunProc = false;
	g_SO.lpMemSO->hWar3 = FindWindow(TEXT("Warcraft III"), TEXT("Warcraft III"));
	if (g_SO.lpMemSO->hWar3 == NULL)
	{
		TCHAR * RunWar3XExe;
		if ( cfg.RunWar3 >0 )
		{
			switch (cfg.RunWar3)
			{
				case 1:
					//D:\Warcraft III\Frozen Throne.exe
					//Frozen Throne.exe	17
					//war3.exe			8
					//�e��["			2
					//-17+8+2 = -7
					RunWar3XExe = new TCHAR[PathLen-7];
					RunWar3XExe[0] = TEXT('"');
					wmemcpy(RunWar3XExe+1, War3XPath, PathLen-18);
					wmemcpy(RunWar3XExe+PathLen-17, TEXT("war3.exe\""), 10);
					bRunProc = CreateProcess(NULL, RunWar3XExe, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE |CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi);
					//ShellExecute(NULL, TEXT("open"), War3XPath, NULL, NULL, 0);
					break;

				case 2:
					// -window			8
					RunWar3XExe = new TCHAR[PathLen+1];
					RunWar3XExe[0] = TEXT('"');
					wmemcpy(RunWar3XExe+1, War3XPath, PathLen-18);
					wmemcpy(RunWar3XExe+PathLen-17, TEXT("war3.exe\" -window"), 18);
					bRunProc = CreateProcess(NULL, RunWar3XExe, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE |CREATE_NEW_PROCESS_GROUP , NULL, NULL, &si, &pi);
					//ShellExecute(NULL, TEXT("open"), War3XPath, TEXT("-window"), NULL, 0);
					break;
			}
			if ( bRunProc )
			{
				delete[] RunWar3XExe;

				WaitForInputIdle(pi.hProcess, 10000);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				g_SO.lpMemSO->hWar3 = FindWindow(TEXT("Warcraft III"), TEXT("Warcraft III"));
				cfg.ChangeWindow = true;
			}
		}

	}
	//War3�w�s�b�A�]0�קKSendMessage����ƹ�
	else
		cfg.RunWar3 = 0;




	//��l�Ƴ]�w

	//TrayIcon
	InitTrayIcon(hWnd, War3XPath);
	delete[] War3XPath;


	g_hhookLLKbd = SetWindowsHookEx(WH_KEYBOARD_LL, LLKeyBoardProc, hInstance, 0);
	//InstallHooks(hInstance);



	//hCatchWar3Thread = CreateThread(NULL, 1, CatchWar3, NULL, 0, NULL);
	//hCatchWar3Thread = (HANDLE) _beginthread(CatchWar3, 0, NULL);
	hCatchWar3Thread = (HANDLE) _beginthreadex(NULL, 0, CatchWar3, NULL, 0, NULL);
	SetThreadPriority(hCatchWar3Thread, THREAD_PRIORITY_BELOW_NORMAL);
		

	if ( g_SO.lpMemSO->hWar3 != NULL )
	{
		ShowWindowAsync(g_SO.lpMemSO->hWar3, SW_SHOW);
		//SetForegroundWindow(g_SO.lpMemSO->hWar3);

		WINDOWINFO * pWINDOWINFO = new WINDOWINFO;
		pWINDOWINFO->cbSize = sizeof(WINDOWINFO);
		GetWindowInfo(g_SO.lpMemSO->hWar3, pWINDOWINFO);

		// �P�_�O�_window mode
		if ( (pWINDOWINFO->dwStyle & WS_MAXIMIZEBOX) !=0)
		{
			g_SO.lpMemSO->restrictMouse = true;
			if ( cfg.ChangeWindow )
			{
				UINT uFlags = 0;
//2010.1.15
				int width, height;					// �ୱ�u�@�ϰ�j�p
				int x = cfg.x, y = cfg.y;			// �����y��
				int cx= cfg.width, cy= cfg.height;	// �����j�p
//	2010.1.15
				// �ù��w�g�O4:3�A���������̤j��
				if ( cfg.width == -1 && cfg.height == -1 )
				{
//2010.1.15
					if ( cfg.keepRatio == 0 ) {
						uFlags = SWP_NOREPOSITION | SWP_NOSIZE;
						ShowWindowAsync(g_SO.lpMemSO->hWar3, SW_MAXIMIZE);
					}
//	2010.1.15
				}
cx = cy * 4 / 3;
				// ���O4:3�ù��̤j��
				if ( uFlags == 0 ) {
					RECT workAreaRECT;
					SystemParametersInfo(SPI_GETWORKAREA, 0, &workAreaRECT, 0);
					width = workAreaRECT.right - workAreaRECT.left;
					height = workAreaRECT.bottom - workAreaRECT.top;

					// keepRatio�@�w�O1
					if ( cfg.width == -1 && cfg.height == -1 ) {
						cy = height;
					}
					if ( cfg.keepRatio == 1 )
						cx = cy * 4 / 3;

					if ( cfg.x == -1 )
						x = (width - cx) / 2;
					if ( cfg.y == -1 )
						y = (height - cy) / 2;
				}
//2010.1.15
				//if ( cfg.x
				if ( cfg.OnTop)
					SetWindowPos(g_SO.lpMemSO->hWar3, HWND_TOPMOST, x, y, cx, cy, uFlags);
				else
					SetWindowPos(g_SO.lpMemSO->hWar3, HWND_NOTOPMOST, x, y, cx, cy, uFlags);
			}
			// �b�����ܰʧ���e WM_ACTIVATE hook�N�|����ƹ�
			// ��RunWar3�~�e
			if ( cfg.RunWar3 !=0 )
				//SendMessage(g_SO.lpMemSO->hWar3, WM_ACTIVATE, WA_ACTIVE, NULL);
				SendMessage(g_SO.lpMemSO->hWar3, WM_ACTIVATEAPP, TRUE, 0);

			//LONG style;
			//style = GetWindowLong(hWar3, GWL_STYLE);
			//SetWindowLong(hWar3, GWL_STYLE, style & ~WS_THICKFRAME );
		}
		else
			g_SO.lpMemSO->restrictMouse = false;

		delete pWINDOWINFO;
	}




	/*
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	*/

	//CreateWindow(TEXT("EDIT"),      // predefined class 
	//	NULL,        // no window title 
	//	WS_CHILD | WS_VISIBLE | WS_VSCROLL | 
	//	ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
	//	50, 50, 100, 100,  // set size in WM_SIZE message 
	//	hWnd,        // parent window 
	//	NULL,   // edit control ID 
	//	hInstance,
	//	NULL);       // pointer not needed

	if (cfg.chat)
		CreateChatWindow(hWnd);


	return TRUE;
}

//
//  �禡: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �γ~:  �B�z�D�������T���C
//
//  WM_COMMAND	- �B�z���ε{���\���
//  WM_PAINT	- ø�s�D����
//  WM_DESTROY	- ��ܵ����T���M���^
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//int wmId;
	//int wmId, wmEvent;
	//PAINTSTRUCT ps;
	//HDC hdc;

	switch (message)
	{
	case WM_USER:
		switch( LOWORD(lParam) )
		{
		case WM_LBUTTONDBLCLK:
			CreateChatWindow(hWnd);
			break;
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			{
				POINT pt;
				GetCursorPos(&pt);
				SetForegroundWindow(hWnd);
				TrackPopupMenuEx(hTrayMenu, TPM_BOTTOMALIGN, pt.x, pt.y, hWnd, NULL);
			}
			break;
		}
		break;
		// ����Active War3
	case WM_USER+1:
		{
			SetActiveWar3( (HWND) wParam);
		}
			break;
		// �B�zWar3������ List �M unhook
	case WM_USER+2:
		{
			_TWar3DataList::iterator result = FindWar3Data(War3DataList.begin(), War3DataList.end(), (HWND) wParam );
			if ( result != War3DataList.end() )
			{
				UnhookWindowsHookEx( result->second );
				War3DataList.erase(result);
			}
			if ( War3DataList.empty() )
			{
				//while ( IsWindow( (HWND) wParam ) )
				//{
				//	;
				//}
				Sleep(5000);

				ResumeThread(hCatchWar3Thread);
			}
		}
		break;
	case WM_COMMAND:
		{
			//wmId    = LOWORD(wParam);
			//wmEvent = HIWORD(wParam);
			// ��R�\���������:
			switch (LOWORD(wParam))
			{
			case IDM_TOP:
				if ( cfg.OnTop )
				{
					SetWindowPos(g_SO.lpMemSO->hWar3, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					cfg.OnTop = false;
					CheckMenuItem(hTrayMenu, IDM_TOP, MF_BYCOMMAND | MF_UNCHECKED );
				}
				else
				{
					SetWindowPos(g_SO.lpMemSO->hWar3, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					cfg.OnTop = true;
					CheckMenuItem(hTrayMenu, IDM_TOP, MF_BYCOMMAND | MF_CHECKED );
				}
				break;
			case IDM_CHECK:
				ShellExecute(NULL, TEXT("open"), TEXT("http://of.openfoundry.org/projects/1274/download"), NULL, NULL, 0);
				break;
			case IDM_FIND:
				ResumeThread(hCatchWar3Thread);
				break;
			case IDM_PASTE_LOAD:
				myPasteBot.Load();
				//EnableMenuItem(hTrayMenu, IDM_PASTE_SEND, MF_BYCOMMAND | MF_ENABLED);
				break;
			case IDM_PASTE_SEND:
				myPasteBot.Send(cfg.PasteDelay);
				break;
			case IDM_PASTE_FREE:
				myPasteBot.Clean();
				//EnableMenuItem(hTrayMenu, IDM_PASTE_SEND, MF_BYCOMMAND | MF_GRAYED);
				break;
			case IDM_CHAT:
				CreateChatWindow(hWnd);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
				break;
			}
		}
		break;
		//case WM_PAINT:
		//	hdc = BeginPaint(hWnd, &ps);
		//	// TODO: �b���[�J����ø�ϵ{���X...
		//	EndPaint(hWnd, &ps);
		//	break;
	case WM_DESTROY:
		{

			//hook
			UnInstallHooks();


			//trayicon delete
			Shell_NotifyIcon(NIM_DELETE, &tnid);
			DestroyMenu(hMenu);

			CloseHandle(hCatchWar3Thread);

			CloseHandle(hSemaphore);
#ifndef NO_Semaphore
			CloseHandle(hReadSemaphore);
			CloseHandle(hWriteSemaphore);
#endif

			PostQuitMessage(0);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void InitTrayIcon(HWND hWnd, LPTSTR War3XPath)
{

	HINSTANCE hExe;		// handle to loaded .EXE file 
	//taskbar trayicon���

	//ZeroMemory(&tnid, sizeof(NOTIFYICONDATA) );
	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = hWnd;
	//possible bug
	tnid.uID = IDC_MYICON;
	tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	//WM_USER�}�l��Y�B application private�ϥ�
	tnid.uCallbackMessage = WM_USER;
	//tnid.uVersion = NOTIFYICON_VERSION;
	//trayicon war3 id 1, 4,  7


	hExe = LoadLibrary( War3XPath );
	//free(War3XExePath);
	tnid.hIcon = (HICON) LoadImage(hExe, MAKEINTRESOURCE(101), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE|LR_SHARED);
	//tnid.hIcon = LoadIcon(hExe, MAKEINTRESOURCE(101) );
	FreeLibrary(hExe);


	//tnid.uVersion = NOTIFYICON_VERSION;
	memcpy(tnid.szTip, TEXT("koala's WC3 helper"), sizeof(TCHAR) * 19);

	//Shell_NotifyIcon(NIM_SETVERSION, &tnid);
	Shell_NotifyIcon(NIM_ADD, &tnid);
	hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENUTRAY));
	hTrayMenu = GetSubMenu(hMenu, 0);
	SetMenuDefaultItem(hTrayMenu, IDM_CHAT, false);
	if ( cfg.OnTop )
		CheckMenuItem(hTrayMenu, IDM_TOP, MF_BYCOMMAND | MF_CHECKED );
	//else
	//	CheckMenuItem(hTrayMenu, IDM_TOP, MF_BYCOMMAND | MF_UNCHECKED );

	//EnableMenuItem(hTrayMenu, IDM_PASTE_SEND, MF_BYCOMMAND | MF_GRAYED);

	//hTrayMenu =	CreatePopupMenu();
	////   AppendMenu(hTrayMenu, MF_STRING, KWM_EXIT, TEXT("EXIT") );
	//AppendMenu(hTrayMenu, MF_STRING, IDM_Paste, TEXT("Paste_bot") );
	//AppendMenu(hTrayMenu, MF_STRING, IDM_CHAT, TEXT("Chat") );
	//AppendMenu(hTrayMenu, MF_STRING, IDM_EXIT, TEXT("E&xit") );
}



void UnInstallHooks()
{
	FreeLibrary(hinstDLLHook);

	UnhookWindowsHookEx(g_hhookLLKbd);
	_TWar3DataList::iterator iter;
	for ( iter = War3DataList.begin(); iter != War3DataList.end(); ++iter)
		UnhookWindowsHookEx(iter->second);
}

LRESULT CALLBACK LLKeyBoardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
	if ( nCode < 0 || nCode != HC_ACTION || (pkbhs->flags & LLKHF_INJECTED)!=0 )
		return CallNextHookEx(g_hhookLLKbd, nCode, wParam, lParam);


	bool bWar3;		//�O�_�bWar3��
	bool bKeyUp;	//�O�_��}����


	bWar3 = GetForegroundWindow() == g_SO.lpMemSO->hWar3;

	if  (wParam==WM_KEYDOWN || wParam==WM_SYSKEYDOWN)
		bKeyUp = false;
	else
		bKeyUp = true;



	//global�\����

	//hide war3
	if ( !bKeyUp )
	{
		if ( pkbhs->vkCode == cfg.hide_war3.vk && TestModifiers(cfg.hide_war3.modifiers, pkbhs) )
		{
			//SetWindowPos	��size show state
			//MoveWindow	��size
			//show_state ture��War3���
			if ( cfg.show_state )
			{
				cfg.show_state = false;
				ShowWindowAsync(g_SO.lpMemSO->hWar3, SW_MINIMIZE);
				//ShowWindowAsync(hWar3, SW_FORCEMINIMIZE);
				ShowWindowAsync(g_SO.lpMemSO->hWar3, SW_HIDE);

				if ( cfg.hide_close )
					DestroyWindow(hWnd);
				else
				{
					Shell_NotifyIcon(NIM_DELETE, &tnid);
					if (hwndChat != NULL)
						ShowWindowAsync(hwndChat, SW_HIDE);
				}
			}
			else
			{
				cfg.show_state = true;
				//ShowWindowAsync(hWar3, SW_SHOW);
				// �[�J�̤p��detection �٭�   ��  ����
				ShowWindowAsync(g_SO.lpMemSO->hWar3, SW_RESTORE);
				SetForegroundWindow(g_SO.lpMemSO->hWar3);
				Shell_NotifyIcon(NIM_ADD, &tnid);
				if (hwndChat != NULL)
					ShowWindowAsync(hwndChat, SW_SHOW);
			}

			return 1;
		}
	} //end bKeyUp


	//����ܪ��ɭ��������F hide_war3�H�~������
	if ( !cfg.show_state )
		return CallNextHookEx(g_hhookLLKbd, nCode, wParam, lParam);


	//�\����
	if ( bWar3 )
	{
		if ( !bKeyUp )
		{
			if ( pkbhs->vkCode == cfg.suspend.vk && TestModifiers(cfg.suspend.modifiers, pkbhs) )
			{
				g_SO.lpMemSO->diableMapKeys = ! g_SO.lpMemSO->diableMapKeys;
				return 1;
			}
			//�짤��
			//else if ( pkbhs->vkCode == VK_F10 )
			//{
			//	POINT pos1, pos2, pos3;
			//	RECT rectWar3Client;
			//	POINT topcorner;
			//	GetClientRect(g_SO.lpMemSO->hWar3, &rectWar3Client);
			//	topcorner.x = rectWar3Client.left;
			//	topcorner.y = rectWar3Client.top;
			//	ClientToScreen(g_SO.lpMemSO->hWar3, &topcorner);
			//	OffsetRect(&rectWar3Client, topcorner.x, topcorner.y);
			//	int clientWidth = rectWar3Client.right - rectWar3Client.left;
			//	int clientHeight = rectWar3Client.bottom - rectWar3Client.top;
			//	//h 1001


			//	POINT pt;
			//	GetCursorPos(&pt);
			//	//pt.y 470 593
			//	ScreenToClient(g_SO.lpMemSO->hWar3, &pt);
			//	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			//	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
			//}

			else if ( pkbhs->vkCode == cfg.pastebot_send.vk && TestModifiers(cfg.pastebot_send.modifiers, pkbhs))
			{
				ReleaseTriggerKeys(cfg.pastebot_send.modifiers, cfg.pastebot_send.vk);
				myPasteBot.Send(cfg.PasteDelay);
				return 1;
			}
			//FastJoinGame
			else if ( pkbhs->vkCode == cfg.fastjoingame.vk && TestModifiers(cfg.fastjoingame.modifiers, pkbhs))
			{
				ReleaseTriggerKeys(cfg.fastjoingame.modifiers, cfg.fastjoingame.vk);
				//BlockInput(true);
				//Sleep(2000);
				SendModifiedKey(2, 'G');
				Sleep(cfg.fastgamedelay);
				SendModifiedKey(1, 'V');
				Sleep(50);
				SendModifiedKey(2, 'J');
				//BlockInput(false);
				return 1;
			}
			//FastPublicGame
			else if ( pkbhs->vkCode == cfg.fastpublicgame.vk && TestModifiers(cfg.fastpublicgame.modifiers, pkbhs))
			{
				ReleaseTriggerKeys(cfg.fastpublicgame.modifiers, cfg.fastpublicgame.vk);
				FastCreateGame(false);
				if ( cfg.fastgamecreate )
					SendModifiedKey(2, 'C');
				return 1;
			}
			//FastPrivateGame
			else if ( pkbhs->vkCode == cfg.fastprivategame.vk && TestModifiers(cfg.fastprivategame.modifiers, pkbhs))
			{
				ReleaseTriggerKeys(cfg.fastprivategame.modifiers, cfg.fastprivategame.vk);
				FastCreateGame(true);
				return 1;
			}

		}	//end bKeyUp


		//�۩w����A�ثekeydown�N�۰ʰ���down and up
		//				keyup������
		if ( !g_SO.lpMemSO->diableMapKeys )
		{
			for (int i=0; i< cfg.mapkey_number; ++i)
			{
				if ( pkbhs->vkCode == cfg.mapkeys[i].vk && TestModifiers(cfg.mapkeys[i].modifiers, pkbhs) )
				{
					//��action�e������Ұʼ���
					//
					ReleaseTriggerKeys(cfg.mapkeys[i].modifiers, cfg.mapkeys[i].vk );

					g_action[i].Do(bKeyUp);
					return 1;
				}
			}
		}
	} //end if bWar3

	//���bWar3�� (global)
	else if ( !bKeyUp )
	{
		if ( pkbhs->vkCode == cfg.pastebot_load.vk && TestModifiers(cfg.pastebot_load.modifiers, pkbhs))
		{
			myPasteBot.Load();
			//EnableMenuItem(hTrayMenu, IDM_PASTE_SEND, MF_BYCOMMAND | MF_ENABLED);
			return 1;
		}

	}

	return CallNextHookEx(g_hhookLLKbd, nCode, wParam, lParam);
}




void CreateChatWindow(HWND hWnd)
{
	if ( hwndChat==NULL )
		hwndChat = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOGCHAT), hWnd, Chat);
	else
		SetForegroundWindow(hwndChat);
}


// [Chat] ������T���B�z�`���C
INT_PTR CALLBACK Chat(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
#define EDIT_MAX_LENGTH 80

	UNREFERENCED_PARAMETER(lParam);
	TCHAR szText[EDIT_MAX_LENGTH+1];		//��edit��text
	LPSTR pUtf8 = NULL;						//UTF8 edit text
	int utf8EditLength;
	//	DWORD ThreadIDEdit = GetWindowThreadProcessId(GetForegroundWindow(), NULL);

	switch (message)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDC_EDIT1, EM_LIMITTEXT, EDIT_MAX_LENGTH, 0);
		//ThreadIDEdit = GetWindowThreadProcessId(hDlg, NULL);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		{
			if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ID_SEND)
			{
				if ( GetDlgItemText(hDlg, IDC_EDIT1, szText, EDIT_MAX_LENGTH) != 0 )
				{
					// ��-1 ���צ��p��'\0'
					utf8EditLength = WideCharToMultiByte(CP_UTF8, 0, szText, -1, 0, 0, NULL, NULL);
					HGLOBAL hClipUtf8;
					hClipUtf8 = GlobalAlloc(GMEM_MOVEABLE, utf8EditLength);
					pUtf8 = (LPSTR) GlobalLock(hClipUtf8);
					WideCharToMultiByte(CP_UTF8, 0, szText, -1, pUtf8, utf8EditLength, NULL, NULL);

					GlobalUnlock(hClipUtf8);
					if ( OpenClipboard(hDlg) )
					{
						EmptyClipboard();
						SetClipboardData(CF_TEXT, hClipUtf8);
						CloseClipboard();


						SendMessage(g_SO.lpMemSO->hWar3, WM_KEYDOWN, VK_RETURN, MapVirtualKey(VK_RETURN, 0) << 16 | 0x00000000);
						SendMessage(g_SO.lpMemSO->hWar3, WM_KEYUP, VK_RETURN, MapVirtualKey(VK_RETURN, 0) << 16 | 0xC0000001);
						Sleep(500);

						SendPaste(g_SO.lpMemSO->hWar3);

						SendMessage(g_SO.lpMemSO->hWar3, WM_KEYDOWN, VK_RETURN, MapVirtualKey(VK_RETURN, 0) << 16 | 0x00000000);
						SendMessage(g_SO.lpMemSO->hWar3, WM_KEYUP, VK_RETURN, MapVirtualKey(VK_RETURN, 0) << 16 | 0xC0000001);


						SetDlgItemText(hDlg, IDC_EDIT1, TEXT(""));
					}
					else
						GlobalFree(hClipUtf8);
				}
				return (INT_PTR)FALSE;
			}
			else if ( LOWORD(wParam) == IDCANCEL)
			{
				hwndChat = NULL;
				DestroyWindow(hDlg);
			}
			//EndDialog(hDlg, FALSE);
			//SendMessage(GetParent(hDlg), WM_DESTROY, 0, 0);
			//SendMessage(GetParent(hDlg), WM_COMMAND, MAKEWPARAM(IDM_EXIT, 0), 0);
		} //end case WM_COMMAND
		break;
	}
	return (INT_PTR)FALSE;
}

void FastGameCommon(bool PrivateGame)
{
	SendModifiedKey(2, 'G');
	Sleep(cfg.fastgamedelay);
	SendModifiedKey(2, 'C');
	Sleep(cfg.fastgamedelay);
	if ( cfg.fastpublicgameob != 0 || cfg.fastpublicgamevis !=0 || PrivateGame)
	{
		SendModifiedKey(2, 'O');
		Sleep(cfg.fastgamedelay);
	}
}

void FastCreateGame(bool PrivateGame)
{
	POINT posOb1, posOb2, posOb3, posVis1, posVis2;

	RECT rectWar3Client;
	POINT topcorner;
	GetClientRect(g_SO.lpMemSO->hWar3, &rectWar3Client);
	topcorner.x = rectWar3Client.left;
	topcorner.y = rectWar3Client.top;
	ClientToScreen(g_SO.lpMemSO->hWar3, &topcorner);
	OffsetRect(&rectWar3Client, topcorner.x, topcorner.y);
	int clientWidth = rectWar3Client.right - rectWar3Client.left;
	int clientHeight = rectWar3Client.bottom - rectWar3Client.top;
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	bool vis;
	//pos1.x = (topcorner.x + 0.85546875*clientWidth)/ screenWidth *65535;
	//pos1.y = (topcorner.y + 0.40364583*clientHeight)/ screenHeight *65535;
	//pos2.y = (topcorner.y + 0.45428571*clientHeight)/ screenHeight *65535;
	//pos2.y = (topcorner.y + 0.50390625*clientHeight)/ screenHeight *65535;
	//pos3.x = (topcorner.x + 0.345703125*clientWidth)/ screenWidth *65535;
	//pos3.y = (topcorner.y + 0.18489583*clientHeight)/ screenHeight *65535

	//��ob�]�w
	posOb1.x = static_cast<long>( (topcorner.x + 0.85546*clientWidth)/ screenWidth *65535 );
	posOb1.y = static_cast<long>( (topcorner.y + 0.40364*clientHeight)/ screenHeight *65535 );
	posVis2.x = posVis1.x = posOb2.x = posOb1.x;

	//ob
	if ( cfg.fastpublicgameob == 1)
		posOb2.y = static_cast<long>( (topcorner.y + 0.45428*clientHeight)/ screenHeight *65535 );
	//refree
	else if ( cfg.fastpublicgameob == 2 || PrivateGame)
		posOb2.y = static_cast<long>( (topcorner.y + 0.50390*clientHeight)/ screenHeight *65535 );

	//private
	posOb3.x = static_cast<long>( (topcorner.x + 0.345703*clientWidth)/ screenWidth *65535 );
	posOb3.y = static_cast<long>( (topcorner.y + 0.184895*clientHeight)/ screenHeight *65535 );

	//�i����
	vis = (!PrivateGame && cfg.fastpublicgamevis == 1) || (PrivateGame && cfg.fastprivategamevis == 1);
	if ( vis )
	{
		posVis1.y = static_cast<long>( (topcorner.y + 0.469530*clientHeight)/ screenHeight *65535 );
		posVis2.y = static_cast<long>( (topcorner.y + 0.592407*clientHeight)/ screenHeight *65535 );
	}

	//else
	//{
	//	//ob
	//	pos1.x = (int) (0.85546875*65535);
	//	pos1.y = (int) (0.40364583*65535);
	//	//referee
	//	//pos2.x = pos1.x;
	//	pos2.y = (int) (0.50390625*65535);
	//	//private
	//	pos3.x = (int) (0.345703125*65535);
	//	pos3.y = (int) (0.18489583*65535);
	//}
	//pos2.x = pos1.x;

	
	FastGameCommon(PrivateGame);


	//��ob
	if ( cfg.fastpublicgameob != 0 || PrivateGame)
	{
		MouseClick(posOb1);
		Sleep(800);
		MouseClick(posOb2);
		Sleep(50);
	}

	if ( vis )
	{
		MouseClick(posVis1);
		Sleep(800);
		MouseClick(posVis2);
		Sleep(50);
	}

	if (PrivateGame)
	{
		MouseClick(posOb3);
		Sleep(200);
	}

	SendModifiedKey(1, 'V');
}

BOOL CALLBACK HooksWar3s(HWND hwnd, LPARAM lparam)
{
	TCHAR szBuffer[15];          // buffer
	GetWindowText(hwnd,szBuffer,15);


	// �OWar3 ���blist���~�B�z
	if( _tcscmp(szBuffer, TEXT("Warcraft III") ) == 0 )
	{
		RealGetWindowClass(hwnd, szBuffer, 15);
		if ( _tcscmp(szBuffer, TEXT("Warcraft III") ) == 0  )
		{
			if ( ! War3DataList.empty() )
			{
				_TWar3DataList::iterator result = FindWar3Data(War3DataList.begin(), War3DataList.end(), hwnd);
				if ( result != War3DataList.end() )
					return true;
			}

#ifndef NO_Semaphore
			WaitForSingleObject(hWriteSemaphore, INFINITE);
			WaitForSingleObject(hSemaphore, INFINITE);
#endif

			g_SO.lpMemSO->hWar3 = hwnd;
			DWORD War3Pid;
			DWORD ThreadIDWar3 = GetWindowThreadProcessId(hwnd, &War3Pid);
			g_SO.lpMemSO->hHook = SetWindowsHookEx(WH_CALLWNDPROC , (HOOKPROC) GetProcAddress(hinstDLLHook, "_WndProc@12") , hinstDLLHook, ThreadIDWar3);
			//TODO, dunno why extern "C" not work :(
			//g_SO.lpMemSO->hHook = SetWindowsHookEx(WH_CALLWNDPROC , (HOOKPROC) GetProcAddress(hinstDLLHook, "WndProc") , hinstDLLHook, ThreadIDWar3);


			War3DataList.push_back ( pair<HWND, HHOOK>(hwnd, g_SO.lpMemSO->hHook) );


#ifndef NO_Semaphore
			ReleaseSemaphore(hSemaphore, 1, NULL);
			ReleaseSemaphore(hReadSemaphore, 1, NULL);
#endif


			// ��Network Latency
			// �̨ΤƼg�k? (�|��prefetch else?)



			if (cfg.latency <= 0 )
				;
			else
			{
				HANDLE War3Process;
				//char latency;
				War3Process = OpenProcess(PROCESS_ALL_ACCESS, false, War3Pid);
				DWORD gameBase = GetDLLBase(TEXT("Game.dll"), War3Pid);
				//WriteProcessMemory(War3Process, (LPVOID)0x6F62FD72, "\x90\x90\x90\x90\x90\x90", 6, NULL);

				//latency = cfg.latency;
				//_snprintf(latency, 2, "%x", (unsigned short) cfg.latency);

				// offset���w
				//WriteProcessMemory(War3Process, (LPVOID) (0x6F65E531), &latency, 1, NULL);
				//offset for 1.24b
				//WriteProcessMemory(War3Process, (LPVOID) (gameBase + 0x65E531), &cfg.latency, 1, NULL);
				//1.24c
				//WriteProcessMemory(War3Process, (LPVOID) (gameBase + 0x65E4F1), &cfg.latency, 1, NULL);
				//1.24d
				WriteProcessMemory(War3Process, (LPVOID) (gameBase + 0x65E551), &cfg.latency, 1, NULL);
				//6F65E551

				CloseHandle(War3Process);
			}
		}
	}
	return true;
}

//DWORD WINAPI CatchWar3( LPVOID lpParameter )
//void __cdecl  CatchWar3( LPVOID lpParameter )
unsigned WINAPI CatchWar3( LPVOID lpParameter )
{
	hinstDLLHook = LoadLibrary( TEXT("hook.dll"));
	EnableDebugPriv();
	for ( ;; )
	{
		do
		{
			EnumWindows(HooksWar3s, 0);
			//Sleep(15000);
		} while ( War3DataList.empty() );
		SetActiveWar3(g_SO.lpMemSO->hWar3);
		SuspendThread(hCatchWar3Thread);
	}
	return 0;
}


void SetActiveWar3(HWND hWar3)
{
	Action::SetHWar3(hWar3);
	myPasteBot.SetHWar3(hWar3);
	g_SO.lpMemSO->hWar3 = hWar3;
}


_TWar3DataList::iterator FindWar3Data(_TWar3DataList::iterator first, _TWar3DataList::iterator last, HWND hWar3)
{
	_TWar3DataList::iterator iter;
	for ( iter = first; iter != last; ++iter )
	{
		if ( iter->first == hWar3)
			break;
	}
	return iter;
}

/* TODO
�h�} boss key
�����M�D�����V��

*/