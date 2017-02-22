// hook.cpp : 定義 DLL 應用程式的進入點。
//

#include "stdafx.h"
#include "hook.h"
#include "..\koala's WC3 helper\globaldata.h"



#ifdef _MANAGED
#pragma managed(push, off)
#endif



#ifdef _MANAGED
#pragma managed(pop)
#endif



SO_handles SO;
HWND hWar3;
HHOOK hHook;

#ifndef NO_Semaphore
HANDLE hSemaphore;								// 限制cirical session
HANDLE hReadSemaphore;							// 限制不能寫入 SO.lpMemSO->hWar3, SO.lpMemSO->hHook
HANDLE hWriteSemaphore;							// 限制不能寫入 SO.lpMemSO->hWar3, SO.lpMemSO->hHook
#endif





BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
			{
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
				// 如果是koala's WC3 helper  不處理


				if ( GetCurrentProcessId() == SO.lpMemSO->HelperPID )
					break;

#ifndef NO_Semaphore
				hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, TEXT("Local\\koala's WC3 helper") );
				hReadSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, TEXT("Local\\koala's WC3 helperR") );
				hWriteSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, TEXT("Local\\koala's WC3 helperW") );

				WaitForSingleObject(hReadSemaphore, INFINITE);
				WaitForSingleObject(hSemaphore, INFINITE);
#endif


				hWar3 = SO.lpMemSO->hWar3;
				hHook = SO.lpMemSO->hHook;

#ifndef NO_Semaphore
				ReleaseSemaphore(hSemaphore, 1, NULL);
				ReleaseSemaphore(hWriteSemaphore, 1, NULL);
#endif
			}

            break;
		//case DLL_THREAD_ATTACH:
        //case DLL_PROCESS_DETACH:
		case DLL_THREAD_DETACH:
         // Perform any necessary cleanup.
#ifndef NO_Semaphore
			{
				CloseHandle(hSemaphore);
				CloseHandle(hReadSemaphore);
				CloseHandle(hWriteSemaphore);
			}
#endif
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}


#ifdef __cplusplus
extern "C" {
#endif
HOOK_API LRESULT CALLBACK WndProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if ( nCode < 0 || nCode != HC_ACTION )
		return CallNextHookEx(hHook, nCode, wParam, lParam);


	CWPSTRUCT  *pCWPSTRUCT = (CWPSTRUCT *) lParam;

	// 判斷hwnd是不是War3
	if ( pCWPSTRUCT->hwnd != hWar3 )
		return CallNextHookEx(hHook, nCode, wParam, lParam);


	switch ( pCWPSTRUCT->message )
	{
		// 輸入法切換偵測
		// LANGID都是1028，無法用來判定
	case WM_INPUTLANGCHANGE:
		{
			//if ( pCWPSTRUCT->lParam != 0x04040404 )
			//	SO.lpMemSO->diableMapKeys = true;
			//else
			//	SO.lpMemSO->diableMapKeys = false;
			SO.lpMemSO->diableMapKeys = (pCWPSTRUCT->lParam != 0x04040404);
		}
		break;

		// 切到視窗鎖滑鼠
	case WM_ACTIVATEAPP:
		{
			if ( pCWPSTRUCT->wParam == FALSE )
				ClipCursor(NULL);	
			else
			//else if ( pCWPSTRUCT->wParam == true )
			{
				if (SO.lpMemSO->restrictMouse)
					RestrictMouse(hWar3);
				SendMessage(SO.lpMemSO->hHelper, WM_USER+1, (WPARAM)hWar3, 0 );
				//SendNotifyMessage(SO.lpMemSO->hHelper, WM_USER+2, (WPARAM)hWar3, 0 );
			}
		}
		break;

//  其實是已知bug，沒去修  因為我不會這樣用
//→ kkk520:有BUG 就是我畫面如果不最大的話 在下面的快捷列按最大化    08/25 01:01
//→ kkk520:就會把滑鼠鎖定在原本視窗大小 而不是最大化的大小          08/25 01:02

	//case WM_SIZE:
	//	{
	//		if ( pCWPSTRUCT->wParam == SIZE_MAXSHOW )
	//			RestrictMouse(hWar3);
	//	}



	//case WM_ACTIVATE:
	//	{
	//		if ( LOWORD(pCWPSTRUCT->wParam) == WA_INACTIVE )
	//			ClipCursor(NULL);
	//		//the high-order word specifies the minimized state of the window being activated or deactivated. A nonzero value indic[]ates the window is minimized. 
	//		//全螢幕時high-order 先送fMinimized ture再送false，所以會兩次
	//		//避免送兩次
	//		//else if ( HIWORD(pCWPSTRUCT->wParam) ==0 )
	//		else
	//		{
	//			if (SO.lpMemSO->restrictMouse)
	//				RestrictMouse(pCWPSTRUCT->hwnd);
	//			SendMessage(SO.lpMemSO->hHelper, WM_USER+1, (WPARAM)hWar3, 0 );
	//		}
	//	} // case WM_ACTIVATE
	//	break;


		//處理War3關掉，通知helper
	case WM_DESTROY:
		

		// 關掉War3告知koala's WC3 helper做處理
		// 不能用SendMessage，會等目標處理完才return
		//		SendMessage(hHelper, WM_DESTROY, 0, 0);
		SendNotifyMessage(SO.lpMemSO->hHelper, WM_USER+2, (WPARAM)hWar3, 0 );
		break;


		//end WM_ACTIVATE
		//case WM_SIZE:
		//case WM_WINDOWPOSCHANGED:
		//	RestrictMouse(((WINDOWPOS *) (pCWPSTRUCT->lParam))->hwnd);
		//	break;
	default:
		break;
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

#ifdef __cplusplus
}
#endif





void RestrictMouse(HWND targetWnd)
{
	RECT rectWar3Client;
	POINT topcorner;
	GetClientRect(targetWnd, &rectWar3Client);
	topcorner.x = rectWar3Client.left;
	topcorner.y = rectWar3Client.top;
	ClientToScreen(targetWnd, &topcorner);
	OffsetRect(&rectWar3Client, topcorner.x, topcorner.y);
	ClipCursor(&rectWar3Client);
}