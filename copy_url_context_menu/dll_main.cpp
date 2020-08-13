#include "framework.h"

// {BFD98515-CD74-48A4-98E2-13D209E3EE4F}
// When you write your own handler, you must create a new CLSID by using the
// "Create GUID" tool in the Tools menu, and specify the CLSID value here.
// {99AC38A2-DC3A-46E4-9BBD-CD905EB7BF93
CLSID CLSID_copy_url_context_menu = { 0x99ac38a2, 0xdc3a, 0x46e4,{ 0x9b, 0xbd, 0xcd, 0x90, 0x5e, 0xb7, 0xbf, 0x93 } };


HINSTANCE   g_hInst = NULL;
long        g_cDllRef = 0;

::copy_url_context_menu::application * g_papp;

#undef new
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
   switch (dwReason)
   {
   case DLL_PROCESS_ATTACH:
   {
      defer_aura_init();

      // Hold the instance of this DLL module, we will use it to get the
      // path of the DLL to register the component.
      g_hInst = hModule;
      //DisableThreadLibraryCalls(hModule);
   }
   break;
   case DLL_THREAD_ATTACH:
      break;

   case DLL_THREAD_DETACH:
      break;
   case DLL_PROCESS_DETACH:
      if (g_papp != NULL)
      {
         ::aura::del(g_papp);
      }
      defer_aura_term();
      break;
   }
   return TRUE;
}


STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
   HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

   if (IsEqualCLSID(CLSID_copy_url_context_menu, rclsid))
   {
      hr = E_OUTOFMEMORY;

      ClassFactory *pClassFactory = new ClassFactory();
      if (pClassFactory)
      {
         hr = pClassFactory->QueryInterface(riid, ppv);
         pClassFactory->Release();
      }
   }

   return hr;
}


//
//   FUNCTION: DllCanUnloadNow
//
//   PURPOSE: Check if we can unload the component from the memory.
//
//   NOTE: The component can be unloaded from the memory when its context_object
//   count is zero (i.e. nobody is still using the component).
//
STDAPI DllCanUnloadNow(void)
{
   return g_cDllRef > 0 ? S_FALSE : S_OK;
}


//
//   FUNCTION: DllRegisterServer
//
//   PURPOSE: Register the COM server and the context menu handler.
//
STDAPI DllRegisterServer(void)
{
   HRESULT hr;

   wchar_t szModule[MAX_PATH];
   if (GetModuleFileNameW(g_hInst, szModule, ARRAYSIZE(szModule)) == 0)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      return hr;
   }

   // Register the component.
   hr = RegisterInprocServer(szModule, CLSID_copy_url_context_menu,
                             L"copy_url_context_menu.copy_url_context_menu Class",
                             L"Apartment");
   if (SUCCEEDED(hr))
   {
      hr = RegisterShellExtContextMenuHandler(L"*",
                                              CLSID_copy_url_context_menu,
                                              L"copy_url_context_menu.copy_url_context_menu");

   }
   if (SUCCEEDED(hr))
   {
      hr = RegisterShellExtContextMenuHandler(L"Directory",
                                              CLSID_copy_url_context_menu,
                                              L"copy_url_context_menu.copy_url_context_menu");
   }
   if (SUCCEEDED(hr))
   {
      hr = RegisterShellExtContextMenuHandler(L"Directory\\Background",
                                              CLSID_copy_url_context_menu,
                                              L"copy_url_context_menu.copy_url_context_menu");
   }

   return hr;
}


//
//   FUNCTION: DllUnregisterServer
//
//   PURPOSE: Unregister the COM server and the context menu handler.
//
STDAPI DllUnregisterServer(void)
{
   HRESULT hr = S_OK;

   wchar_t szModule[MAX_PATH];
   if (GetModuleFileNameW(g_hInst, szModule, ARRAYSIZE(szModule)) == 0)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      return hr;
   }

   // Unregister the component.
   hr = UnregisterInprocServer(CLSID_copy_url_context_menu);
   if (SUCCEEDED(hr))
   {
      // Unregister the context menu handler.
      hr = UnregisterShellExtContextMenuHandler(L"*", CLSID_copy_url_context_menu);
   }

   return hr;
}