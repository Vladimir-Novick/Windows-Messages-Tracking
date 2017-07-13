//
// Author: Vladimir Novick , <vlad.novick@gmail.com>
//
//               https://www.linkedin.com/in/vladimirnovick
//
//                  https://github.com/Vladimir-Novick/
//                      
// (c) 1999 - 2017,  Vladimir Novick
//
// Purpose: Sets the windows hooks used to trap the window's messages
//
//*****************************************************************************/
#define PUBLIC
#if !DBG
#define PRIVATE static
#else
#define PRIVATE
#endif

// special hwnds for the hooks to recognize
#define HWND_ALL  ((HWND)0xffffffff)
#define HWND_NULL ((HWND)0L)

#define SPYAPPNAME      "EDictionary"
#define SPYCLASSNAME    "EDictionary Class"
#define HOOKWINDOWNAME  "About EDictionary"
#define HOOKWINDOWCLASS "#32770"


//
// Structure that contains the spy message data sent from the hook
// to the spy app to display.  The hwnd is sent using wParam of
// the WM_COPYDATA message, and the message number is in the dwData
// field of the COPYDATASTRUCT.  This structure allows the wParam,
// lParam and any optional extra data to be passed across.
//
typedef struct
{
    WPARAM wParam;
    LPARAM lParam;
    BYTE ExtraData[64];
} SPYMSGDATA, *PSPYMSGDATA;


PRIVATE SPYMSGDATA gsmd;
PRIVATE COPYDATASTRUCT gcds = { 0, sizeof(SPYMSGDATA), &gsmd };


 LRESULT CALLBACK SpyGetMsgProc(INT hc, WPARAM wParam, LPARAM lParam);
 LRESULT CALLBACK SpyCallWndProc(INT hc, WPARAM wParam, LPARAM lParam);
 LRESULT CALLBACK MouseFunc (int nCode, WPARAM wParam, LPARAM lParam );
 LRESULT CALLBACK	InstallHook(HWND target_hWnd );
 LRESULT CALLBACK	UninstallHook();      
LRESULT CALLBACK KeyboardFunc (int nCode, WPARAM wParam, LPARAM lParam );
 
BOOL SetSpyHook(BOOL fSet);

