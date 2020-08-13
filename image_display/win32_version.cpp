#include "framework.h"
#include <stdio.h>


/***********************************************************************************/
/*                                                                                 */
/* Class:   CGlobalFunctions                                                       */
/* Method:  GetVersionInfo                                                         */
/*                                                                                 */
/* Parameters:                                                                     */
/* -----------                                                                     */
/*   HMODULE hLib                                                                  */
/*                Handle to the module that contains the resource (EXE or DLL)     */
/*                A value of NULL specifies the current applications resources     */
/*                                                                                 */
/*   string csEntry                                                               */
/*                Specifies the name of the resource. For more information,        */
/*                see the Remarks section.                                         */
/*                                                                                 */
/* Return Values:                                                                  */
/* --------------                                                                  */
/* If the function succeeds, the return value is a string containing the value     */
/* of the specified resource.                                                      */
/* If the function fails, the returned string is empty. To get extended error      */
/* information, call GetLastError.                                                 */
/*                                                                                 */
/* Remarks:                                                                        */
/* --------                                                                        */
/* Since the Win32 API resource information is encoded in Unicode, this method     */
/* also strips the strings from Unicode.                                           */
/*                                                                                 */
/* The following valid values for csEntry, as specified by Microsoft are:          */
/*   CompanyName, FileDescription, FileVersion, InternalName, LegalCopyright,      */
/*   OriginalFilename, ProductName, ProductVersion, Comments, LegalTrademarks,     */
/*   PrivateBuild, SpecialBuild                                                    */
/*                                                                                 */
/* Opening the rc-file as "text" or with a text-editor allows you to add further   */
/* entries to your version information structure and it is retrievable using       */
/* this same method.                                                               */
/*                                                                                 */
/***********************************************************************************/
namespace version
{

   string get_version_info(HMODULE hLib, string strEntry);
   string format_version(string str);



   string get_version_info(HMODULE hLib, string csEntry)
   {

      string csRet;

      if (hLib == NULL)
      {

         hLib = ::aura::system::g_p->m_hinstance;

      }

      WCHAR wsz[1024];

      if (!::GetModuleFileNameW(hLib, wsz, sizeof(wsz) / sizeof(wsz[0])))
      {

         return "";

      }

      DWORD dwSize = GetFileVersionInfoSizeW(wsz, NULL);

      if (dwSize == 0)
      {

         return "";

      }

      memory m;

      m.allocate(dwSize);

      LPVOID versionInfo = m.get_data();

      if (!GetFileVersionInfoW(wsz, 0, dwSize, versionInfo))
      {

         return "";

      }


      //HRSRC hVersion = FindResource(hLib, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
      //if (hVersion != NULL)
      //{
      //   HGLOBAL hGlobal = LoadResource(hLib, hVersion);
      //   if (hGlobal != NULL)
      //   {

      if (versionInfo != NULL)
      {
         DWORD vLen;
         BOOL retVal;

         LPVOID retbuf = NULL;

         char fileEntry[256];

         sprintf(fileEntry, "\\VarFileInfo\\Translation");
         vLen = 0;
         struct LANGANDCODEPAGE
         {
            WORD wLanguage;
            WORD wCodePage;
         } *plangandcodepage;
         retVal = VerQueryValue(versionInfo, fileEntry, (LPVOID *) &plangandcodepage, (UINT *)&vLen);
         if (retVal && vLen == 4)
         {
            sprintf(fileEntry, "\\StringFileInfo\\%04x%04x\\%s",
                    plangandcodepage->wLanguage,
                    plangandcodepage->wCodePage, csEntry.c_str());
         }
         else
            sprintf(fileEntry, "\\StringFileInfo\\%04x04b0\\%s", GetUserDefaultLangID(), csEntry.c_str());

         retbuf = NULL;
         vLen = 0;
         if (VerQueryValue(versionInfo, fileEntry, &retbuf, (UINT *)&vLen))
            csRet = (char*)retbuf;
      }
      //   }

      //   UnlockResource(hGlobal);
      //   FreeResource(hGlobal);
      //}

      return csRet;
   }

   /***********************************************************************************/
   /*                                                                                 */
   /* Class:   CGlobalFunctions                                                       */
   /* Method:  FormatVersion                                                          */
   /*                                                                                 */
   /* Parameters:                                                                     */
   /* -----------                                                                     */
   /*   string cs                                                                    */
   /*                Specifies a version number such as "FileVersion" or              */
   /*                "ProductVersion" in the format "m, n, o, p"                      */
   /*                (e.g. "1, 2, 3, a")                                              */
   /*                                                                                 */
   /* Return Values:                                                                  */
   /* --------------                                                                  */
   /* If the function succeeds, the return value is a string containing the version   */
   /* in the format "m.nop" (e.g. "1.23a")                                            */
   /*                                                                                 */
   /* If the function fails, the returned string is empty.                            */
   /*                                                                                 */
   /***********************************************************************************/
   string format_version(string cs)
   {

      bool bOp = true;
      while (bOp)
      {
         bOp = ::str::ends_eat_ci(cs, ".0");
         bOp = bOp || ::str::ends_eat_ci(cs, ",0");
      }

      return cs;

   }

   /***********************************************************************************/
   /*                                                                                 */
   /* Class:   CGlobalFunctions                                                       */
   /* Method:  GetFileVersionX                                                        */
   /*                                                                                 */
   /* Parameters:                                                                     */
   /* -----------                                                                     */
   /*                                                                                 */
   /* Return Values:                                                                  */
   /* --------------                                                                  */
   /* If the function succeeds, the return value is a string containing the           */
   /* "FileVersion" in the format "m.nop" (e.g. "1.23a")                              */
   /*                                                                                 */
   /* If the function fails, the returned string is empty.                            */
   /*                                                                                 */
   /***********************************************************************************/
//string CGlobalFunctions::GetFileVersionX()
//{
//   if (m_csFileVersion.IsEmpty())
//   {
//      string csVersion = FormatVersion(GetVersionInfo(NULL, "FileVersion"));
//      m_csFileVersion.Format("Version %s (Build %s)", csVersion, GetVersionInfo(NULL, "SpecialBuild"));
//   }

//   return m_csFileVersion;
//}

   /***********************************************************************************/
   /*                                                                                 */
   /* Class:   CGlobalFunctions                                                       */
   /* Method:  GetFileVersionX                                                        */
   /*                                                                                 */
   /* Parameters:                                                                     */
   /* -----------                                                                     */
   /*                                                                                 */
   /* Return Values:                                                                  */
   /* --------------                                                                  */
   /* If the function succeeds, the return value is a string containing the           */
   /* "ProductVersion" in the format "m.nop" (e.g. "1.23a")                           */
   /*                                                                                 */
   /* If the function fails, the returned string is empty.                            */
   /*                                                                                 */
   /***********************************************************************************/
   string get_version()
   {

      return format_version(get_version_info(NULL, "ProductVersion"));

   }




} // namespace version
