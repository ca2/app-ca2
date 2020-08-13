#include "framework.h"

CLSID CLSID_copy_url_context_menu = { 0x99ac38a2, 0xdc3a, 0x46e4,{ 0x9b, 0xbd, 0xcd, 0x90, 0x5e, 0xb7, 0xbf, 0x93 } };


HINSTANCE   g_hInst = NULL;
long        g_cDllRef = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
   switch (dwReason)
   {
   case DLL_PROCESS_ATTACH:
   {
      g_hInst = hModule;
   }
   break;
   case DLL_THREAD_ATTACH:
      break;

   case DLL_THREAD_DETACH:
      break;
   case DLL_PROCESS_DETACH:
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


STDAPI DllCanUnloadNow(void)
{
   return g_cDllRef > 0 ? S_FALSE : S_OK;
}


STDAPI DllRegisterServer(void)
{
   HRESULT hr;

   wchar_t szModule[MAX_PATH];
   if (GetModuleFileNameW(g_hInst, szModule, ARRAYSIZE(szModule)) == 0)
   {
      hr = HRESULT_FROM_WIN32(GetLastError());
      return hr;
   }

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


STDAPI DllUnregisterServer(void)
{

   HRESULT hr = S_OK;

   wchar_t szModule[MAX_PATH];

   if (GetModuleFileNameW(g_hInst, szModule, ARRAYSIZE(szModule)) == 0)
   {

      hr = HRESULT_FROM_WIN32(GetLastError());

      return hr;

   }

   hr = UnregisterInprocServer(CLSID_copy_url_context_menu);

   if (SUCCEEDED(hr))
   {

      hr = UnregisterShellExtContextMenuHandler(L"*", CLSID_copy_url_context_menu);

      hr = UnregisterShellExtContextMenuHandler(L"Directory", CLSID_copy_url_context_menu);

      hr = UnregisterShellExtContextMenuHandler(L"Directory\\Background", CLSID_copy_url_context_menu);


   }

   return hr;
}