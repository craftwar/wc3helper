// MyContent.h: interface for the MyContent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCONTENT_H__E1B3AF99_0FA6_44CD_82E3_55719F9E3806__INCLUDED_)
#define AFX_MYCONTENT_H__E1B3AF99_0FA6_44CD_82E3_55719F9E3806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "globaldata.h"
#include "action.h"




class Settings
{


public:
	Settings(): show_state(true) {}
	struct FnKey
	{
		short modifiers;
		unsigned short vk;
		//USHORT target;

	};
	class MapKey
	{
	public:
		// 要初始化NULL，避免MapKey設太大  關掉時delete會error
		MapKey(): target(NULL) {}
		~MapKey() {delete[] target; }
		short modifiers;
		unsigned short vk;
		//short target;
		WCHAR* target;
	};


	~Settings() { delete[] mapkeys; }

	bool ChangeWindow, OnTop, chat, show_state, hide_close, fastgamecreate;
	short x, y;		// 視窗位置座標 (左上角)	-1	自動置中
	short width, height;	// 視窗大小，都設-1最大化
	unsigned short RunWar3;		//War3不存在時是否執行
	unsigned short keepRatio;	//視窗大小是否改成適合遊戲的4:3(視窗化才有效)
	unsigned short fastpublicgameob;
	unsigned short fastpublicgamevis;
	unsigned short fastprivategamevis;
	unsigned short fastgamedelay;
	unsigned short PasteDelay;
	unsigned short latency;
	FnKey suspend;
	FnKey hide_war3;
	FnKey pastebot_load, pastebot_send;
	FnKey fastjoingame, fastpublicgame, fastprivategame;
	USHORT mapkey_number;
	MapKey* mapkeys;


};



class MyContent : public ISAXContentHandler
{
public:
	MyContent(SO_handles &SO, class Settings &cfg, Action *&pAction);
	virtual ~MyContent() {};

	// This must be correctly implemented, if your handler must be a COM Object (in this example it does not)
	long __stdcall QueryInterface(const struct _GUID &,void ** );
	unsigned long __stdcall AddRef(void);
	unsigned long __stdcall Release(void);

    public:
        virtual HRESULT STDMETHODCALLTYPE putDocumentLocator( 
            /* [in] */ ISAXLocator *pLocator);
        
        virtual HRESULT STDMETHODCALLTYPE startDocument( void);
        
        virtual HRESULT STDMETHODCALLTYPE endDocument( void);
        
        virtual HRESULT STDMETHODCALLTYPE startPrefixMapping( 
            /* [in] */ const wchar_t *pwchPrefix,
            /* [in] */ int cchPrefix,
            /* [in] */ const wchar_t *pwchUri,
            /* [in] */ int cchUri);
        
        virtual HRESULT STDMETHODCALLTYPE endPrefixMapping( 
            /* [in] */ const wchar_t *pwchPrefix,
            /* [in] */ int cchPrefix);
        
        virtual HRESULT STDMETHODCALLTYPE startElement( 
            /* [in] */ const wchar_t *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ const wchar_t *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ const wchar_t *pwchQName,
            /* [in] */ int cchQName,
            /* [in] */ ISAXAttributes *pAttributes);
        
        virtual HRESULT STDMETHODCALLTYPE endElement( 
            /* [in] */ const wchar_t *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ const wchar_t *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ const wchar_t *pwchQName,
            /* [in] */ int cchQName);
        
        virtual HRESULT STDMETHODCALLTYPE characters( 
            /* [in] */ const wchar_t *pwchChars,
            /* [in] */ int cchChars);
        
        virtual HRESULT STDMETHODCALLTYPE ignorableWhitespace( 
            /* [in] */ const wchar_t *pwchChars,
            /* [in] */ int cchChars);
        
        virtual HRESULT STDMETHODCALLTYPE processingInstruction( 
            /* [in] */ const wchar_t *pwchTarget,
            /* [in] */ int cchTarget,
            /* [in] */ const wchar_t *pwchData,
            /* [in] */ int cchData);
        
        virtual HRESULT STDMETHODCALLTYPE skippedEntity( 
            /* [in] */ const wchar_t *pwchName,
            /* [in] */ int cchName);


private:
	enum {Settings_section, Fnkey_section, Mapkey_section} section;
	unsigned short cnt;
	SO_handles &SO;
	Settings &cfg;
	Action *&pAction;
};

#endif // !defined(AFX_MYCONTENT_H__E1B3AF99_0FA6_44CD_82E3_55719F9E3806__INCLUDED_)