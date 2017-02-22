// MyContent.cpp: implementation of the MyContent class. 
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyContent.h"


MyContent::MyContent(SO_handles &SO, Settings &cfg, Action *&pAction): section(Settings_section), cnt(0), SO(SO), cfg(cfg), pAction(pAction)
{
}

HRESULT STDMETHODCALLTYPE MyContent::startElement( 
            /* [in] */ const wchar_t *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ const wchar_t *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ const wchar_t *pwchQName,
            /* [in] */ int cchQName,
            /* [in] */ ISAXAttributes *pAttributes)
{
	//§ìattribute¥Î
	const wchar_t * pwchValue;
	int cchValue;


	if ( wcsncmp( pwchLocalName, _T("Fnkey"), cchLocalName) == 0 )
		section = Fnkey_section;


	switch (section)
	{
	case Settings_section:
		{
			if (wcsncmp(pwchLocalName, _T("Run"), cchLocalName) == 0 )
			{
				STARTUPINFO si = {0};
				PROCESS_INFORMATION pi = {0};
				si.cb = sizeof(STARTUPINFO);
				WCHAR * run;
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				run = new WCHAR[cchValue+1];
				wmemcpy(run, pwchValue, cchValue);
				run[cchValue] = '\0';
				CreateProcess(NULL, run, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE |CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi);
				delete [] run;
			}
			//RunWar3
			else if (wcsncmp(pwchLocalName, _T("RunWar3"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%hu", &cfg.RunWar3);

				pAttributes->getValue(1,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%u", &cchValue);
				cfg.ChangeWindow = (cchValue!=0);

				pAttributes->getValue(2,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%u", &cchValue);
				cfg.OnTop = (cchValue!=0);

				pAttributes->getValue(3,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hd", &cfg.x);

				pAttributes->getValue(4,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hd", &cfg.y);

				pAttributes->getValue(5,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hu", &cfg.keepRatio);

				pAttributes->getValue(6,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hd", &cfg.width);

				pAttributes->getValue(7,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hd", &cfg.height);
			}
			else if (wcsncmp(pwchLocalName, _T("Chat"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%u", &cchValue);
				cfg.chat = (cchValue!=0);
			}
			else if (wcsncmp(pwchLocalName, _T("FastGameDelay"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hu", &cfg.fastgamedelay);
			}
			else if (wcsncmp(pwchLocalName, _T("Pastebot_delay"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hu", &cfg.PasteDelay);
			}
			else if (wcsncmp(pwchLocalName, _T("Network_latency"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hu", &cfg.latency);
			}
		}
		break;
	case Fnkey_section:
		{
			if (wcsncmp(pwchLocalName, _T("Suspend"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%hd", &cfg.suspend.modifiers);

				pAttributes->getValue(1,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hx", &cfg.suspend.vk);
			}
			else if (wcsncmp(pwchLocalName, _T("Hide_War3"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%hd", &cfg.hide_war3.modifiers);

				pAttributes->getValue(1,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hx", &cfg.hide_war3.vk);

				pAttributes->getValue(2,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%u", &cchValue);
				cfg.hide_close = (cchValue!=0);
			}
			else if (wcsncmp(pwchLocalName, _T("Pastebot_load"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%hd", &cfg.pastebot_load.modifiers);

				pAttributes->getValue(1,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hx", &cfg.pastebot_load.vk);
			}
			else if (wcsncmp(pwchLocalName, _T("Pastebot_send"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%hd", &cfg.pastebot_send.modifiers);

				pAttributes->getValue(1,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hx", &cfg.pastebot_send.vk);
			}
			else if (wcsncmp(pwchLocalName, _T("FastJoinGame"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%hd", &cfg.fastjoingame.modifiers);

				pAttributes->getValue(1,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hx", &cfg.fastjoingame.vk);
			}
			else if (wcsncmp(pwchLocalName, _T("FastPublicGame"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%hd", &cfg.fastpublicgame.modifiers);

				pAttributes->getValue(1,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hx", &cfg.fastpublicgame.vk);

				pAttributes->getValue(2,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hd", &cfg.fastpublicgameob);

				pAttributes->getValue(3,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hd", &cfg.fastpublicgamevis);

				pAttributes->getValue(4,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%u", &cchValue);
				cfg.fastgamecreate = (cchValue!=0);
			}
			else if (wcsncmp(pwchLocalName, _T("FastPrivateGame"), cchLocalName) == 0 )
			{
				pAttributes->getValue(0,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, 1, L"%hd", &cfg.fastprivategame.modifiers);

				pAttributes->getValue(1,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hx", &cfg.fastprivategame.vk);

				pAttributes->getValue(2,  &pwchValue, &cchValue);
				_snwscanf_s(pwchValue, cchValue, L"%hd", &cfg.fastprivategamevis);
			}

			else if ( wcsncmp(pwchLocalName, _T("Mapkey_number"), cchLocalName) == 0 )
			{
				section = Mapkey_section;

				pAttributes->getValue(0,  &pwchValue, &cchValue);

				// handle to mapfile object
				_snwscanf_s(pwchValue, cchValue, L"%hd", &cfg.mapkey_number);
				cfg.mapkeys = new Settings::MapKey[cfg.mapkey_number];
				pAction = new Action[cfg.mapkey_number];
			}
		}
		break;
	case Mapkey_section:
		{
			pAttributes->getValue(0,  &pwchValue, &cchValue);
			_snwscanf_s(pwchValue, 1, L"%hd", &cfg.mapkeys[cnt].modifiers);

			pAttributes->getValue(1,  &pwchValue, &cchValue);
			_snwscanf_s(pwchValue, cchValue, L"%hx", &cfg.mapkeys[cnt].vk);


			pAttributes->getValue(2,  &pwchValue, &cchValue);
			cfg.mapkeys[cnt].target = new WCHAR[cchValue+1];
			
			wmemcpy(cfg.mapkeys[cnt].target, pwchValue, cchValue);
			cfg.mapkeys[cnt].target[cchValue] = L'\0';


			pAction[cnt].Parse(cfg.mapkeys[cnt].target);


			++cnt;
		}

		break;
	default:
		break;


	}

	return S_OK;
}


HRESULT STDMETHODCALLTYPE MyContent::putDocumentLocator( 
    /* [in] */ ISAXLocator *pLocator)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MyContent::startDocument( void)
{
	return S_OK;
}



HRESULT STDMETHODCALLTYPE MyContent::endDocument( void)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE MyContent::startPrefixMapping( 
            /* [in] */ const wchar_t *pwchPrefix,
            /* [in] */ int cchPrefix,
            /* [in] */ const wchar_t *pwchUri,
            /* [in] */ int cchUri)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE MyContent::endPrefixMapping( 
            /* [in] */ const wchar_t *pwchPrefix,
            /* [in] */ int cchPrefix)
{
	return S_OK;
}




HRESULT STDMETHODCALLTYPE MyContent::endElement( 
            /* [in] */ const wchar_t *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ const wchar_t *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ const wchar_t *pwchQName,
            /* [in] */ int cchQName)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MyContent::characters( 
            /* [in] */ const wchar_t *pwchChars,
            /* [in] */ int cchChars)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE MyContent::ignorableWhitespace( 
            /* [in] */ const wchar_t *pwchChars,
            /* [in] */ int cchChars)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE MyContent::processingInstruction( 
            /* [in] */ const wchar_t *pwchTarget,
            /* [in] */ int cchTarget,
            /* [in] */ const wchar_t *pwchData,
            /* [in] */ int cchData)
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE MyContent::skippedEntity( 
            /* [in] */ const wchar_t *pwchName,
            /* [in] */ int cchName)
{
	return S_OK;
}



long __stdcall MyContent::QueryInterface(const struct _GUID &riid,void ** ppvObject)
{
	return 0;
}

unsigned long __stdcall MyContent::AddRef()
{
	return 0;
}

unsigned long __stdcall MyContent::Release()
{
	return 0;
}