#ifndef util_h
#define util_h

#include <Windows.h>
#include <Tlhelp32.h>


extern void PasteUtf8(HWND ClipBoardOwner, HWND targetWnd, char * utf8, short length);
//void SendString(WCHAR * sendstring);
extern void SendPaste(HWND targetWnd);
extern void SendModifiedKey(int modifier, short vk);
extern bool TestModifiers(short mod, KBDLLHOOKSTRUCT *pkbhs);
extern void ReleaseTriggerKeys(short mod, WORD wVk);
extern void MouseClick(POINT &pos);
extern void MouseClick(LONG x, LONG y);
void EnableDebugPriv();
DWORD GetDLLBase(TCHAR* DllName, DWORD tPid);



#endif