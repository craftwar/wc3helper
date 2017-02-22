// 下列 ifdef 區塊是建立巨集以協助從 DLL 匯出的標準方式。
// 這個 DLL 中的所有檔案都是使用命令列中所定義 HOOK_EXPORTS 符號編譯的。
// 任何使用這個 DLL 的專案都不應定義這個符號。
// 這樣一來，原始程式檔中包含這檔案的任何其他專案
// 會將 HOOK_API 函式視為從 DLL 匯入的，而這個 DLL 則會將這些符號視為
// 匯出的。
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