#include "stdafx.h"
#include "globaldata.h"

SO_handles::SO_handles()
{
	//if (  (hMapSO = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, TEXT("Local\\koala\'s WC3 SO")) )== NULL )
		hMapSO = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,  PAGE_READWRITE, 0, sizeof(struct tagSO), TEXT("Local\\koala's WC3 SO"));
	lpMemSO = (tagSO *) MapViewOfFile( 
		hMapSO,     // object to map view of
		FILE_MAP_WRITE, // read/write access
		0,              // high offset:  map from
		0,              // low offset:   beginning
		0);             // default: map entire file
	lpMemSO->diableMapKeys = false;
}


SO_handles::~SO_handles()
{
	UnmapViewOfFile(lpMemSO);
	CloseHandle(hMapSO);
}