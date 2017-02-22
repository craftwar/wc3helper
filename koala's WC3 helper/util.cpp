#include "stdafx.h"
#include "util.h"



void PasteUtf8(HWND ClipBoardOwner, HWND targetWnd, char * utf8, short length)
{


	if ( length <=0 )
		return;

	HGLOBAL hClipUtf8 = GlobalAlloc(GMEM_MOVEABLE, length);
	char * pUtf8 = (char *) GlobalLock(hClipUtf8);
	memcpy(pUtf8, utf8, length);

	GlobalUnlock(hClipUtf8);
	if ( OpenClipboard(ClipBoardOwner) )
	{
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hClipUtf8);
		CloseClipboard();

		SendPaste(targetWnd);
		//SendMessage(targetWnd, WM_KEYDOWN, VK_RETURN, MapVirtualKey(VK_RETURN, 0) << 16 | 0x00000000);
		//SendMessage(targetWnd, WM_KEYUP, VK_RETURN, MapVirtualKey(VK_RETURN, 0) << 16 | 0xC0000001);
		//Sleep(500);

	}
	else
		GlobalFree(hClipUtf8);


}

void SendPaste(HWND targetWnd)
{
	SendMessage(targetWnd, WM_KEYDOWN, VK_CONTROL, MapVirtualKey(VK_CONTROL, 0) << 16 | 0x00000000);
	SendMessage(targetWnd, WM_KEYDOWN, 'V', MapVirtualKey('V', 0) << 16 | 0x00000000);
	SendMessage(targetWnd, WM_KEYUP, 'V', MapVirtualKey('V', 0) << 16 | 0xC0000001);
	SendMessage(targetWnd, WM_KEYUP, VK_CONTROL, MapVirtualKey(VK_CONTROL, 0) << 16 | 0xC0000001);
}


void SendModifiedKey(int modifier, short vk)
{
	INPUT KeyInput[2];
	KeyInput[0].type = INPUT_KEYBOARD;
	KeyInput[0].ki.dwFlags = 0;
	KeyInput[0].ki.time = 0;
	KeyInput[0].ki.dwExtraInfo = NULL;

	switch (modifier)
	{
	case 1:
		KeyInput[0].ki.wVk = VK_CONTROL;
		break;
	case 2:
		KeyInput[0].ki.wVk = VK_MENU;
		break;
	case 4:
		KeyInput[0].ki.wVk = VK_SHIFT;
		break;
	default:
		break;
	}
	KeyInput[0].ki.wScan = MapVirtualKey(KeyInput[0].ki.wVk, 0);


	KeyInput[1].type = INPUT_KEYBOARD;
	KeyInput[1].ki.dwFlags = 0;
	KeyInput[1].ki.time = 0;
	KeyInput[1].ki.dwExtraInfo = NULL;
	KeyInput[1].ki.wVk = vk;
	KeyInput[1].ki.wScan = MapVirtualKey(KeyInput[1].ki.wVk, 0);


	SendInput(2, KeyInput, sizeof(INPUT));
	KeyInput[0].ki.dwFlags = KeyInput[1].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, &KeyInput[1], sizeof(INPUT));
	Sleep(50);
	SendInput(1, &KeyInput[0], sizeof(INPUT));
}

//ture表示通過
bool TestModifiers(short mod, KBDLLHOOKSTRUCT *pkbhs)
{

	//ctrl
	if ( (mod & 1)!=0 && GetKeyState(VK_CONTROL)>=0 ||
		 (mod & 1)==0 && GetKeyState(VK_CONTROL)<0 )
		return false;

	//alt
	else if ( (mod & 2)!=0 && (pkbhs->flags & LLKHF_ALTDOWN)==0 ||
		      (mod & 2)==0 && (pkbhs->flags & LLKHF_ALTDOWN)!=0 )
		return false;

	//shift
	else if ( (mod & 4)!=0 && GetKeyState(VK_SHIFT)>=0 ||
		      (mod & 4)==0 && GetKeyState(VK_SHIFT)<0 )
		return false;

	else
		return true;
}

//釋放啟動熱鍵再做動作
void ReleaseTriggerKeys(short mod, WORD wVk)
{
	INPUT KeyUpInput;
	KeyUpInput.type = INPUT_KEYBOARD;
	KeyUpInput.ki.dwFlags = KEYEVENTF_KEYUP;
	KeyUpInput.ki.time = 0;
	KeyUpInput.ki.dwExtraInfo = NULL;

	KeyUpInput.ki.wVk = wVk;

	KeyUpInput.ki.wScan = MapVirtualKey(KeyUpInput.ki.wVk, 0);
	SendInput(1, &KeyUpInput, sizeof(INPUT));

	//ctrl
	if ( (mod & 1)!=0 )
	{
		Sleep(50);
		KeyUpInput.ki.wVk = KeyUpInput.ki.wVk = VK_CONTROL;
		KeyUpInput.ki.wScan = MapVirtualKey(VK_CONTROL, 0);
		SendInput(1, &KeyUpInput, sizeof(INPUT));
	}

	//alt
	if ( (mod & 2)!=0 )
	{
		Sleep(50);
		KeyUpInput.ki.wVk = KeyUpInput.ki.wVk = VK_MENU;
		KeyUpInput.ki.wScan = MapVirtualKey(VK_MENU, 0);
		SendInput(1, &KeyUpInput, sizeof(INPUT));
	}
	//shift
	if ( (mod & 4)!=0 )
	{
		Sleep(50);
		KeyUpInput.ki.wVk = KeyUpInput.ki.wVk = VK_SHIFT;
		KeyUpInput.ki.wScan = MapVirtualKey(VK_SHIFT, 0);
		SendInput(1, &KeyUpInput, sizeof(INPUT));
	}
}


void MouseClick(POINT &pos)
{
	MouseClick(pos.x, pos.y);
}


void MouseClick(LONG x, LONG y)
{
	INPUT mInput;
	mInput.type = INPUT_MOUSE;
	mInput.mi.dx = x;
	mInput.mi.dy = y;
	mInput.mi.mouseData = 0;
	//mInput.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
	mInput.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	mInput.mi.time = 0;
	mInput.mi.dwExtraInfo = NULL;

	SendInput(1, &mInput, sizeof(INPUT) );

	Sleep(40);

	mInput.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &mInput, sizeof(INPUT) );

	Sleep(40);

	mInput.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, &mInput, sizeof(INPUT) );
}



void EnableDebugPriv()
{
    HANDLE hToken;
    LUID sedebugnameValue;
    TOKEN_PRIVILEGES tkp;

    if ( ! OpenProcessToken( GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
        return;
    if ( ! LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue ) ){
        CloseHandle( hToken );
        return;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if ( ! AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL ) )
        CloseHandle( hToken );
} 

//Gets the base of our dll
DWORD GetDLLBase(TCHAR* DllName, DWORD tPid)
{
    HANDLE snapMod;  
    MODULEENTRY32 me32;

    if (tPid == 0) return 0;
    snapMod = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, tPid);  
    me32.dwSize = sizeof(MODULEENTRY32);  
    if (Module32First(snapMod, &me32)){ 
        do{
            if (_tcscmp(DllName,me32.szModule) == 0){ 
                CloseHandle(snapMod); 
                return (DWORD) me32.modBaseAddr; 
            }
        }while(Module32Next(snapMod,&me32));
    }

    CloseHandle(snapMod); 
    return 0;  
}