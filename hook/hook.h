// �U�C ifdef �϶��O�إߥ����H��U�q DLL �ץX���зǤ覡�C
// �o�� DLL �����Ҧ��ɮ׳��O�ϥΩR�O�C���ҩw�q HOOK_EXPORTS �Ÿ��sĶ���C
// ����ϥγo�� DLL ���M�׳������w�q�o�ӲŸ��C
// �o�ˤ@�ӡA��l�{���ɤ��]�t�o�ɮת������L�M��
// �|�N HOOK_API �禡�����q DLL �פJ���A�ӳo�� DLL �h�|�N�o�ǲŸ�����
// �ץX���C
#ifdef HOOK_EXPORTS
#define HOOK_API __declspec(dllexport)
#else
#define HOOK_API __declspec(dllimport)
#endif




//extern HOOK_API int nhook;
#ifdef __cplusplus
extern "C" {
#endif


HOOK_API LRESULT CALLBACK WndProc(int code,WPARAM wParam,LPARAM lParam);

#ifdef __cplusplus
}
#endif

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved );  // reserved
void RestrictMouse(HWND targetWnd);