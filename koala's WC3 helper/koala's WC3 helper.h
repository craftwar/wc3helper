#pragma once

#include "resource.h"
#include <Windows.h>
#include <utility>
#include <list>

using namespace std;

typedef list<pair<HWND,HHOOK>> _TWar3DataList;


void CreateChatWindow(HWND hWnd);
void FastGameCommon(bool PrivateGame);
void FastCreateGame(bool PrivateGame);



// [Chat] 方塊的訊息處理常式。
INT_PTR CALLBACK Chat(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


void InitTrayIcon(HWND hWnd, LPTSTR War3ExePath);
void InstallHooks(HINSTANCE hInstance);
void UnInstallHooks();

LRESULT CALLBACK LLKeyBoardProc(int nCode,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK HooksWar3s(HWND hwnd, LPARAM lparam);
//DWORD WINAPI CatchWar3( LPVOID lpParameter );
//void __cdecl  CatchWar3( LPVOID lpParameter );
unsigned WINAPI CatchWar3( LPVOID lpParameter );
void SetActiveWar3(HWND hWar3);
_TWar3DataList::iterator FindWar3Data(_TWar3DataList::iterator first, _TWar3DataList::iterator last, HWND hWar3);