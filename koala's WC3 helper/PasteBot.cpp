#include "StdAfx.h"
#include "PasteBot.h"
#include "util.h"



void PasteBot::Load()
{
	if (inBuffer!=NULL)
		Clean();

	// 如果已經開讀檔
	if (load_lock)
		return;

	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260];      // buffer for file name
	HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwndOwner;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = NULL;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("Text(*.txt)\0*.TXT\0All\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	//ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = TEXT("file to be pasted in War3");
	ofn.Flags = OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	// Display the Open dialog box. 

	load_lock = true;
	if ( GetOpenFileName(&ofn) !=0 )
	{
		hf = CreateFile(ofn.lpstrFile, GENERIC_READ, FILE_SHARE_READ| FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,	NULL);
		DWORD nBytesToRead = GetFileSize(hf, NULL);
		inBuffer = new char[nBytesToRead];

		ReadFile(hf, inBuffer, nBytesToRead, &fileBytes, NULL);
		CloseHandle(hf);

		//strchr();

		//前進一個字元
		//_mbsinc_l;
		//找字元
		//mbschr_l;
		//"cht"
		//65001 	utf-8
		//locale不能指定codepage為multi-byte (utf-7 utf-8 etc)，直接用\n (0a anscii同utf-8)
		//_locale_t uft8_locale = _create_locale(LC_ALL, ".65001");
		//_locale_t uft8_locale = _create_locale(LC_ALL, "German");
		//_free_locale(uft8_locale);
	}
	load_lock = false;
}


void PasteBot::Send(unsigned short &delay)
{
	if ( inBuffer==NULL)
		return;

	char * start = inBuffer;
	char * end;
	short length;
	bool finish = false;

	while ( !finish )
	{

		if ( (end = strchr(start, '\n')) == NULL )
		{
			//不包含最後一個字元 (eof?)
			end = inBuffer + fileBytes;
			finish = true;
		}

		//不包含換行
		length = end-start;

		// 多加個Enter讓在遊戲中的chat也可以貼
		SendMessage(hWar3, WM_KEYDOWN, VK_RETURN, MapVirtualKey(VK_RETURN, 0) << 16 | 0x00000000);
		SendMessage(hWar3, WM_KEYUP, VK_RETURN, MapVirtualKey(VK_RETURN, 0) << 16 | 0xC0000001);

		PasteUtf8(hwndOwner, hWar3, start, length);

		SendMessage(hWar3, WM_KEYDOWN, VK_RETURN, MapVirtualKey(VK_RETURN, 0) << 16 | 0x00000000);
		SendMessage(hWar3, WM_KEYUP, VK_RETURN, MapVirtualKey(VK_RETURN, 0) << 16 | 0xC0000001);
		Sleep(delay);

		start = end+1;
	}

}