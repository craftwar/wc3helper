#include "stdafx.h"
#include "action.h"
#include "util.h"


HWND Action::hWar3;
HWND Action::hWC3_helper;


Action_node::~Action_node()
{
	if (next != NULL)
		delete next;
}

Action::~Action()
{
	if ( action_steps!= NULL)
		delete action_steps;
}

void Action::Parse(WCHAR* target)
{
	// start   key code�}�l ( '{' )
	LPWSTR pStr, start, end;
	Action_node *pDataNode, **pInsert_pos;

	pStr = target;
	pInsert_pos = &action_steps;



	// �٦����e
	while (pStr!=NULL && *pStr !=L'\0')
	{

		pDataNode = new Action_node();
		*pInsert_pos = pDataNode;
		pInsert_pos = & pDataNode->next;


		start = wcschr(pStr, L'{');



		// �����key && �U�@����ƬOkey
		if ( start == pStr )
		{
			pDataNode->type = Action_node::key_node;
			++start;
			end = wcschr(start+1, L'}');
			_snwscanf_s(start, end-start, L"%hx", & pDataNode->data.key);
			pStr = end+1;
		}


		// �U�@����ƬOtext (�]�taction�S��key�����p)
		else
		{
			pDataNode->type = Action_node::text_node;

			// �ѤU���Otext (�䤣�� '{' )
			if ( start==NULL )
			{
				//start = pStr;
				//length = -1;
				//pStr = NULL;

				// ���]�t '\0'
				pDataNode->data.text.length = WideCharToMultiByte(CP_UTF8, 0, pStr, -1, 0, 0, NULL, NULL);
				pDataNode->data.text.utf8  = new char[pDataNode->data.text.length];
				WideCharToMultiByte(CP_UTF8, 0, pStr, pDataNode->data.text.length, pDataNode->data.text.utf8, pDataNode->data.text.length, NULL, NULL);
			}
			// text, key...  text node�᭱���@��key
			else
			{
				// { - ���B�z�}�l  =  text���פ��t'\0'
				short length = start-pStr;
			
				// �n�h��1�� '\0'   clipboard CF_TEXT�n��'\0'����
				pDataNode->data.text.length = WideCharToMultiByte(CP_UTF8, 0, pStr, length, 0, 0, NULL, NULL);
				pDataNode->data.text.utf8  = new char[pDataNode->data.text.length+1];


				WideCharToMultiByte(CP_UTF8, 0, pStr, length, pDataNode->data.text.utf8, pDataNode->data.text.length, NULL, NULL);
				pDataNode->data.text.utf8[pDataNode->data.text.length++] = '\0';

				pStr = start;
			}

		}

	}

}

void Action::Do(bool bKeyUp)
{
	if (bKeyUp)
		return;

	Action_node * pDataNode = action_steps;

	while(pDataNode!=NULL)
	{
		switch (pDataNode->type)
		{
		case Action_node::key_node:
			if (pDataNode->data.key !=0 )
			{

				INPUT KeyInput;
				KeyInput.type = INPUT_KEYBOARD;
				KeyInput.ki.wVk = pDataNode->data.key;
				KeyInput.ki.wScan = MapVirtualKey(KeyInput.ki.wVk, 0);
				KeyInput.ki.time = 0;
				KeyInput.ki.dwExtraInfo = NULL;

				//KEYEVENTF_EXTENDEDKEY;
				//if ( bKeyUp )
				//	KeyInput.ki.dwFlags = KEYEVENTF_KEYUP;
				//else
				//	KeyInput.ki.dwFlags = 0;
				//SendInput(1, &KeyInput, sizeof(INPUT));

				KeyInput.ki.dwFlags = 0;
				SendInput(1, &KeyInput, sizeof(INPUT));
				Sleep(10);
				KeyInput.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &KeyInput, sizeof(INPUT));

			}
			break;
			case Action_node::text_node:
				{
					PasteUtf8(Action::hWC3_helper, Action::hWar3, pDataNode->data.text.utf8, pDataNode->data.text.length);
					break;
				}
		default:
			break;
		}

		pDataNode = pDataNode->next;

	}


}