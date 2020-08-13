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

      return format_version("0.08");

   }


} // namespace version
