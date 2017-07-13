
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

#include <windows.h>
#include "trackdll.h"
#include <stdio.h>


#define DBG

#define MK_CONTROL = 0x8


 SPYMSGDATA gsmd;
// COPYDATASTRUCT gcds = { 0, sizeof(SPYMSGDATA), &gsmd };
HANDLE	hInstance;				// Global instance handle for	DLL
#pragma data_seg("SHARDATA")
     static volatile BOOL flsendK = FALSE;
     static volatile BOOL flsendKBOOK = FALSE;
     static HHOOK hhkMouseMessage = NULL;
	 static HHOOK ghwndSpyHookKeyBoard = NULL;
     static HHOOK hhkGetMessage = NULL;
     static HHOOK hhkCallWndProc = NULL;
     static HWND ghwndSpyHook = NULL;   // the handle back to the spy executable
     MSG pmsg;
     static volatile BOOL flControl = FALSE;
     static volatile BOOL flControlBOOK = FALSE;
	 
#pragma data_seg()

//#ifdef DBG
//VOID DbgPrintf(LPTSTR fmt, ...);
//#endif



/*****************************************************************************\
* DllMain (hModule,cbHeap,lpchCmdLine)
*
* Called when the libary is loaded
*
* Arguments:
*    PVOID hModule - Module handle for the libary.
*    ULONG ulReason - DLL purpose
*    PCONTEXT pctx - not used
*
* Returns:
*    TRUE - Everything is ok
*    FALSE- Error.
\*****************************************************************************/

BOOL
APIENTRY DllMain(
    PVOID hModule,
    ULONG ulReason,
    PCONTEXT pctx
    )
{
    //
    // This function is called for every instance of the DLL. We must find
    // and store the handle to the spy window every time an instance of the
    // DLL is instantiated.
    //
    if ( ulReason == DLL_PROCESS_ATTACH ) {
    hInstance = hModule;
					flsendK = FALSE;
		 		   flControl = FALSE;
					flsendKBOOK = FALSE;
		 		   flControlBOOK = FALSE;
    }

    return TRUE;
}




/*****************************************************************************\
* SetSpyHook
*
* Sets the windows hooks used to trap the messages.  After this
* is called with a TRUE for fSet, the messages will start flowing
* through the hook DLL.
*
* Arguments:
*   BOOL fSet - TRUE to hook, FALSE to unhook.
*
* Returns:
*   TRUE if successful.
*
\*****************************************************************************/

BOOL
SetSpyHook(
    BOOL fSet
    )
{
  

    if (fSet)
    {

      if (!hhkGetMessage)
        {
            if (!(hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE,
                (HOOKPROC)SpyGetMsgProc, hInstance, 0)))
            {
                return FALSE;
            }
        }

        if (!hhkCallWndProc)
        {
            if (!(hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC,
                SpyCallWndProc, hInstance, 0)))
            {
                UnhookWindowsHookEx(hhkGetMessage);
                return FALSE;
            }
        }


        if (!hhkMouseMessage)
        {
            if (!(hhkMouseMessage = SetWindowsHookEx(WH_MOUSE,
                MouseFunc, hInstance, 0)))
            {
                UnhookWindowsHookEx(hhkGetMessage);
                UnhookWindowsHookEx(hhkCallWndProc);
 
                return FALSE;
            }
        }


        if (!ghwndSpyHookKeyBoard)
        {
            if (!(ghwndSpyHookKeyBoard = SetWindowsHookEx( WH_KEYBOARD,
                KeyboardFunc, hInstance, 0)))
            {
                UnhookWindowsHookEx(hhkGetMessage);
                UnhookWindowsHookEx(hhkCallWndProc);
                UnhookWindowsHookEx(hhkMouseMessage);
 
                return FALSE;
            }
        }


		

    }
    else
    {
        if (hhkGetMessage)
        {
            UnhookWindowsHookEx(hhkGetMessage);
            hhkGetMessage = NULL;
        }

        if (hhkCallWndProc)
        {
            UnhookWindowsHookEx(hhkCallWndProc);
            hhkCallWndProc = NULL;
        }


        if (hhkMouseMessage)
        {
            UnhookWindowsHookEx(hhkMouseMessage);
            hhkMouseMessage = NULL;
        }


        if (ghwndSpyHookKeyBoard)
        {
            UnhookWindowsHookEx(ghwndSpyHookKeyBoard);
            ghwndSpyHookKeyBoard = NULL;
        }



		

    }

    return TRUE;
}



LRESULT CALLBACK	InstallHook(HWND target_hWnd ){
	ghwndSpyHook = target_hWnd;
          SetSpyHook(TRUE);
}
LRESULT CALLBACK	UninstallHook(){
	          SetSpyHook(FALSE);
}


/*****************************************************************************\
* HookProc( hWnd, uiMessage, wParam, lParam )
*
* The hook proc for the windows hook being spied on
*
* Arguments:
*    HWND hWnd - window handle for the parent window
*    UINT uiMessage - message number
*    WPARAM wParam - message-dependent
*    LPARAM lParam - message-dependent
*
* Returns:
*    0 if processed, nonzero if ignored
\*****************************************************************************/

BOOL WINAPI
HookProc(
    HWND hwnd,
    UINT uiMessage,
    WPARAM wParam,
    LPARAM lParam
    )
{

  
    if (ghwndSpyHook != NULL && hwnd != ghwndSpyHook)
    {

        //
        // Send the message on asynchronously for Spy to deal with if
        // it is the appropriate hwndSpyingOn window to spy on.
        //

            gsmd.wParam = wParam;
            gsmd.lParam = lParam;

            gcds.dwData = uiMessage;




			if (uiMessage == WM_KEYDOWN ) {

                  SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&gcds);

//				    0x11 - left control

				if (wParam ==0x11 ){ 
				   flControl = TRUE;
				   flsendK = FALSE;
                                   SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&gcds);
				}
			}



			if (uiMessage == WM_KEYUP ) {



				if (wParam ==0x11 ){ 
		 		   flControl = FALSE;
				}


                               SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&gcds);
			}



			if (uiMessage == WM_RBUTTONDOWN ) {
			}




			if (uiMessage == WM_ACTIVATE) {
				if (flControl == TRUE ){
                                  SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&gcds);
                                  PostMessage(hwnd, WM_KEYDOWN, (WPARAM)0x0000001B, (LPARAM)0x0010001);		
                                  PostMessage(hwnd, WM_KEYUP, (WPARAM)0x0000001B, (LPARAM)0xC0010001);		
				}
			}
            

			if (uiMessage == WM_CONTEXTMENU) {
			}

			if (uiMessage == WM_RBUTTONUP ) {
				if (flControl == TRUE ){

                                  PostMessage(hwnd, WM_KEYDOWN, (WPARAM)0x0000001B, (LPARAM)0x0010001);		
                                  PostMessage(hwnd, WM_KEYUP, (WPARAM)0x0000001B, (LPARAM)0xC0010001);		

                                  PostMessage(hwnd, WM_KEYDOWN, (WPARAM)0x00000043, (LPARAM)0x002E0001);		
                                  PostMessage(hwnd, WM_KEYUP, (WPARAM)0x00000043, (LPARAM)0xC02E0001);

                                  SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&gcds);
					return 1;

			}



			}



            return TRUE;
    }

    return FALSE;
}




void ClearMyClipboard()
{
    // Get data from the clipboard
    if (OpenClipboard(NULL))                     
    { 
 

       EmptyClipboard();
    
       CloseClipboard();                     
    }
}


// KeyboardFunc    17 - ctrl ; 18- alt
// lparam < 0 
//
// Filter function for the WH_KEYBOARD
//
//--------------------------------------------------------------------------

LRESULT CALLBACK KeyboardFunc (int nCode, WPARAM wParam, LPARAM lParam )
{
   HDC	       hDC;
    static int index = 1;
   if ( nCode >= 0 ) {


				if ((wParam ==17 )&&(lParam < 0)){ 
		 		   flControl = FALSE;
				}


				if ((wParam ==16 )&&(lParam < 0)){   // shift
		 		   flControlBOOK = FALSE;
				   flsendKBOOK = FALSE;
				}



				if ((wParam ==27 )&&(lParam > 0 )){ 
                                   gsmd.wParam = wParam;
                                   gsmd.lParam = lParam;

                                   gcds.dwData = WM_KEYDOWN;
                                   SendMessage(ghwndSpyHook, WM_COPYDATA, wParam, (LPARAM)&gcds);
				}



				if ((wParam == 16 )&&(lParam > 0 )){ 
					if (flsendKBOOK == FALSE ){
				           flControlBOOK = TRUE;
					}
 				   flsendK = FALSE;
                                   SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&gcds);
				}



				if ((wParam ==17 )&&(lParam > 0 )){ 
					if (flControlBOOK == FALSE ){
				           flControl = TRUE;
					}
 				   flsendK = FALSE;
                                   SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&gcds);
				}


          if ( nCode == HC_NOREMOVE )
 		  strcpy(szType, "NOT Removed from Queue");
          else
 		 strcpy(szType, "REMOVED from Queue");

 	  if (lParam < 0 ){
                wsprintf((LPSTR)szFilterLine[KEYBOARDINDEX],
 	       "KEYBOARD\tKey:%d %d\t%s KEYUP",wParam,lParam,(LPSTR)szType);
 	  } else {
               wsprintf((LPSTR)szFilterLine[KEYBOARDINDEX],
 	       "KEYBOARD\tKey:%d %d\t%sKEYDOWN",wParam,lParam,(LPSTR)szType);

 	  }

       hDC = GetDC(hwndMain);
       TabbedTextOut(hDC, 1, nLineHeight * index,
 	    (LPSTR)szFilterLine[KEYBOARDINDEX],
 	    strlen(szFilterLine[KEYBOARDINDEX]), 0, NULL, 1);
           ReleaseDC(hwndMain, hDC);
 	  index++;
 	  if (index > 40 ) index = 1;
   }

   	return 1L;
   //
   // We looked at the message ... sort of processed it but since we are
   // looking we will pass all messages on to CallNextHookEx.
   //
   return( CallNextHookEx(ghwndSpyHookKeyBoard, nCode, wParam,lParam));
}




//--------------------------------------------------------------------------
// MouseFunc
//
// Filter function for the WH_MOUSE
//
//--------------------------------------------------------------------------
LRESULT CALLBACK MouseFunc (int nCode, WPARAM wParam, LPARAM lParam )
{

   LPMOUSEHOOKSTRUCT MouseHookParam;

	static int index = 1;


   if ( nCode >= 0 ) {


	  MouseHookParam = (MOUSEHOOKSTRUCT *) lParam;



	  if (wParam == WM_LBUTTONUP ){
         gsmd.wParam = wParam;
            gsmd.lParam = lParam;

            gcds.dwData = WM_LBUTTONUP;


                SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)MouseHookParam->hwnd,  (LPARAM)&gcds);

	  }


	  if (wParam == WM_RBUTTONDOWN ){

				if (flControl == TRUE ) {
					flsendK = TRUE;
					ClearMyClipboard();
                    PostMessage(MouseHookParam->hwnd, WM_KEYDOWN, (WPARAM)0x00000043, (LPARAM)0x002E0001);		
                    PostMessage(MouseHookParam->hwnd, WM_KEYUP, (WPARAM)0x00000043, (LPARAM)0xC02E0001);
			   					  return 1L;
				} 



				if (flControlBOOK == TRUE){
					if (flsendKBOOK == FALSE){
					  flsendKBOOK = TRUE;
					  ClearMyClipboard();
                      PostMessage(MouseHookParam->hwnd, WM_KEYDOWN, (WPARAM)17, (LPARAM)1900545);		

                      PostMessage(MouseHookParam->hwnd, WM_KEYDOWN, (WPARAM)0x00000043, (LPARAM)0x002E0001);		
                      PostMessage(MouseHookParam->hwnd, WM_KEYUP, (WPARAM)0x00000043, (LPARAM)0xC02E0001);


                      PostMessage(MouseHookParam->hwnd, WM_KEYUP, (WPARAM)17, (LPARAM)-1071841279);		

					}
			   		  return 1L;
				} 


	  }


	  if (wParam == WM_RBUTTONUP ){
				if (flsendK == TRUE ){



                     PostMessage(hwnd, WM_KEYDOWN, (WPARAM)0x0000001B, (LPARAM)0x0010001);		
                    PostMessage(hwnd, WM_KEYUP, (WPARAM)0x0000001B, (LPARAM)0xC0010001);		



            gsmd.wParam = wParam;
            gsmd.lParam = lParam;

            gcds.dwData = WM_RBUTTONUP;
					flsendK = FALSE;
		 		   flControl = FALSE;
                      SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)MouseHookParam->hwnd,  (LPARAM)&gcds);
					  return 1L;
			}


				if (flsendKBOOK == TRUE ){
				flsendKBOOK = FALSE;

            gsmd.wParam = wParam;
            gsmd.lParam = lParam;

            gcds.dwData = WM_COPYDATA;
					flsendKBOOK = FALSE;
		 		   flControlBOOK = FALSE;
                SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)MouseHookParam->hwnd,  (LPARAM)&gcds);
					  return 1L;
			}

	  }

   }



   //
   // We looked at the message ... sort of processed it but since we are
   // looking we will pass all messages on to CallNextHookEx.
   //
   return( CallNextHookEx(hhkMouseMessage, nCode, wParam, lParam));
}


/*****************************************************************************\
* SpyGetMsgProc
*
* The Get Message hook function.
*
\*****************************************************************************/

LRESULT CALLBACK
SpyGetMsgProc(
    INT hc,
    WPARAM wParam,
    LPARAM lParam
    )
{
    PMSG pmsg;

    pmsg = (PMSG)lParam;

    if (hc >= 0 && pmsg && pmsg->hwnd)
    {
         HookProc(pmsg->hwnd, pmsg->message, pmsg->wParam, pmsg->lParam);

         return CallNextHookEx(hhkGetMessage, hc, wParam, lParam);

    }

    //
    // Note that CallNextHookEx ignores the first parameter (hhook) so
    // it is acceptable (barely) to pass in a NULL.
    //
    return CallNextHookEx(NULL, hc, wParam, lParam);
}



/*****************************************************************************\
* SpyCallWndProc
*
* The Call Window Proc (Send Message) hook function.
*
\*****************************************************************************/

LRESULT CALLBACK
SpyCallWndProc(
    INT hc,
    WPARAM wParam,
    LPARAM lParam
    )
{
    PCWPSTRUCT pcwps;

    pcwps = (PCWPSTRUCT)lParam;

    if (hc >= 0 && pcwps && pcwps->hwnd)
    {
              HookProc(pcwps->hwnd, pcwps->message, pcwps->wParam, pcwps->lParam);
		       return CallNextHookEx(hhkCallWndProc, hc, wParam, lParam);
    }

    //
    // Note that CallNextHookEx ignores the first parameter (hhook) so
    // it is acceptable (barely) to pass in a NULL.
    //
    return CallNextHookEx(NULL, hc, wParam, lParam);
}



#ifdef DBG
/****************************************************************************
* DBGprintf
*
* This debugging function prints out a string to the debug output.
*
* Arguments:
*   LPTSTR fmt - Format string (printf style).
*   ...        - Variable number of arguments.
* Returns:
*    VOID
\****************************************************************************/

VOID DbgPrintf(
    LPTSTR fmt,
    ...
    )
{
    va_list marker;
    TCHAR szBuf[256];

    va_start(marker, fmt);
    wvsprintf(szBuf, fmt, marker);
    va_end(marker);

    OutputDebugString(szBuf);
    OutputDebugString(TEXT("\r\n"));
}
#endif


