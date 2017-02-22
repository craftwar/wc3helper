#ifndef globaldata_h
#define globaldata_h


// 是否使用Semaphore避免多開War3出錯 (War3有delay dll load所以不需要?)
#define NO_Semaphore





struct tagSO
{
	tagSO(): hWar3(NULL) {}
	HWND hWar3, hHelper;
	DWORD HelperPID;
	bool diableMapKeys;
	bool restrictMouse;
	HHOOK hHook;

	//HHOOK hhookMsg, hhookWnd, hhookKbd, hhookLLKbd;
	
};

class SO_handles
{
public:
	SO_handles();
	~SO_handles();

	HANDLE hMapSO;
	tagSO* lpMemSO;
};


#endif