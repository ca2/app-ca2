#include "framework.h"

#ifdef WINDOWS

#ifdef BOCKENGINEERINGWIN
[MTAThread]
#endif
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{


   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(lpReserved);


   if (dwReason == DLL_PROCESS_ATTACH)
   {


      ::output_debug_string(L"wndfrm_bockengineering.dll :: initializing!\n");


   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {


      ::output_debug_string(L"wndfrm_bockengineering.dll :: terminating!\n");


   }

   return 1;   // ok

}


#endif




