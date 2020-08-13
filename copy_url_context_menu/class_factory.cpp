#include "framework.h"
#pragma comment(lib, "shlwapi.lib")

extern long g_cDllRef;


ClassFactory::ClassFactory() : m_cRef(1)
{
   InterlockedIncrement(&g_cDllRef);
}

ClassFactory::~ClassFactory()
{
   InterlockedDecrement(&g_cDllRef);
}


//
// IUnknown
//

IFACEMETHODIMP ClassFactory::QueryInterface(REFIID riid, void **ppv)
{
   static const QITAB qit[] =
   {
      QITABENT(ClassFactory, IClassFactory),
      { 0 },
   };
   return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) ClassFactory::AddRef()
{
   return InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) ClassFactory::Release()
{
   ULONG cRef = InterlockedDecrement(&m_cRef);
   if (0 == cRef)
   {
      delete this;
   }
   return cRef;
}


//
// IClassFactory
//

IFACEMETHODIMP ClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{

   HRESULT hr = CLASS_E_NOAGGREGATION;

   if (pUnkOuter == NULL)
   {

      hr = E_OUTOFMEMORY;

      if (g_papp == NULL)
      {

         g_papp = new ::copy_url_context_menu::application();

         g_papp->pre_run();

      }

      if (g_papp != NULL)
      {

         // Query the specified interface.
         hr = g_papp->QueryInterface(riid, ppv);

         g_papp->Release();

      }

   }

   return hr;

}


IFACEMETHODIMP ClassFactory::LockServer(BOOL fLock)
{
   if (fLock)
   {
      InterlockedIncrement(&g_cDllRef);
   }
   else
   {
      InterlockedDecrement(&g_cDllRef);
   }
   return S_OK;
}