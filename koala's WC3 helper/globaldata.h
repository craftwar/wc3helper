#ifndef globaldata_h
#define globaldata_h


// �O�_�ϥ�Semaphore�קK�h�}War3�X�� (War3��delay dll load�ҥH���ݭn?)
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