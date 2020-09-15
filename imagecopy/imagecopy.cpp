#include "aura/aura/aura/aura.h"
#include "aura/node/windows/windows.h"
#undef new
#define min MIN
#define max MAX
#include <gdiplus.h>
#define new ACME_NEW


#include <stdio.h>




class imagecopy_app :
   virtual public ::aura::app
{
public:


   imagecopy_app() :
      aura::system(this, NULL)
   {

   }
   virtual ~imagecopy_app()
   {

   }

   string_map < ::datetime::time > m_mapImageTime;
   ::datetime::time get_image_time(string str);
   bool touch_file(string str);


   virtual i32 run() override
   {

      ::datetime::time  timeStart;
      ::datetime::time  timeEnd;
      string strDate;
      int iArg = 1;
      {

         int iPath = 0;

         int iPathCount = 0;

         if ((string(__argv[iArg]).contains("-") || string(__argv[iArg]).contains("/"))
            && string(__argv[iArg]).contains(":"))
         {

            strDate = string(__argv[iArg]);
            strDate = strDate.Left(strDate.find(" "));
            string str = string(__argv[iArg]);
            timeStart = datetime().strtotime(NULL, str, iPath, iPathCount);

            iArg++;

         }
         else if ((string(__argv[iArg]).contains("-") || string(__argv[iArg]).contains("/"))
            && string(__argv[iArg+1]).contains(":"))
         {

            strDate = string(__argv[iArg]);
            string str = string(__argv[iArg]) + " " + __argv[iArg + 1];
            timeStart = datetime().strtotime(NULL,str , iPath, iPathCount);

            iArg += 2;

         }
         else
         {

            timeStart = datetime().strtotime(NULL, string(__argv[iArg]) , iPath, iPathCount);

            iArg++;
         }

      }
      
      {

         int iPath = 0;

         int iPathCount = 0;

         if ((string(__argv[iArg]).contains("-") || string(__argv[iArg]).contains("/"))
            && string(__argv[iArg]).contains(":"))
         {

            strDate = string(__argv[iArg]);
            strDate = strDate.Left(strDate.find(" "));
            string str = string(__argv[iArg]);
            timeEnd = datetime().strtotime(NULL, str, iPath, iPathCount);

            iArg++;

         }
         else if ((string(__argv[iArg]).contains("-") || string(__argv[iArg]).contains("/"))
            && string(__argv[iArg]).contains(":"))
         {
            
            string str = string(__argv[iArg]) + " " + __argv[iArg + 1];

            timeEnd = datetime().strtotime(NULL, str, iPath, iPathCount);

            iArg += 2;

         }
         else
         {

            timeEnd = datetime().strtotime(NULL, strDate + " " + __argv[iArg], iPath, iPathCount);

            iArg++;
         }

      }

      int iInterval = MAX(1,atoi_dup(__argv[iArg]));
      iArg++;

      string str(__argv[iArg]);
      ::file::path pathFolder;
      str.trim();
      if (str.find(":") != 1 && !::str::begins_ci(str, "\\"))
      {
         wchar_t wsz[MAX_PATH * 8];
         _wgetcwd(wsz, sizeof(wsz) / sizeof(wchar_t));
         pathFolder = ::file::path(wsz) / str;
      }
      else
      {
         pathFolder = str;

      }

      iArg++;

      if (!dir().is(pathFolder, this))
      {
         
         dir().mk(pathFolder, this);

         if (!dir().is(pathFolder, this))
         {

            fprintf(stderr, "The specified path is not a folder and could not create folder: %s", pathFolder);

            return -1;
         }


      }
      
      printf("%s\n", string("Start Time: ") + datetime().international().get_local_date_time(timeStart));
      printf("%s\n", string("End Time: ") + datetime().international().get_local_date_time(timeEnd));
      printf("%s\n", string("Interval: ") + ::str::from(iInterval) + "s" );
      printf("%s\n", string("Destination Folder: ") + pathFolder);


      ::file::listing ls(this);
      wchar_t wsz[MAX_PATH * 8];
      _wgetcwd(wsz, sizeof(wsz) / sizeof(wchar_t));
      ls.ls_file(wsz);


      

      for (index i = 0; i < ls.get_size();)
      {
         printf("Checking File: %s", ls[i].name());
         if (!touch_file(ls[i]))
         {
            
            printf(" : not Image or Image has no properties.\n");
            ls.remove_at(i);

         }
         else
         {
            ::datetime::time t(get_image_time(ls[i]));
            string strTime = datetime().international().get_local_date_time(t);

            printf(" : OK (%s)\n", strTime);
            i++;

         }

      }

      printf("\n");
      printf("\n");
      printf("Moving Files...\n");

      ::sort::array::pred_quick_sort(ls,
         [&](const ::file::path & p1, const ::file::path & p2)
      {
         ::datetime::time t1 = get_image_time(p1);
         ::datetime::time t2 = get_image_time(p2);
         return t1 <= t2;
      });

      ::file::patha pathaFiltered;
      ::datetime::time timeNext;

      for (::datetime::time t = timeStart; t < timeEnd; t += ::datetime::time_span(0, 0, 0, iInterval))
      {
         timeNext = t + ::datetime::time_span(0, 0, 0, iInterval);
         for (index i = 0; i < ls.get_size(); i++)
         {
            ::file::path p = ls[i];
            ::datetime::time tFile(get_image_time(p));

            if (tFile >= t && tFile < timeNext)
            {

               pathaFiltered.add(p);
               break;

            }

         }
      }

      printf("%d images match the criteria\n", pathaFiltered.get_count());

      for (index i = 0; i < pathaFiltered.get_size(); i++)
      {
         ::file::path p = pathaFiltered[i];
         ::datetime::time t(get_image_time(p));
         string strTime = datetime().international().get_local_date_time(t);
         

         ::file::path pathDestination = pathFolder / p.name();

         printf("%5d: %s Copying file to: %s\n", i + 1, strTime.c_str(), pathDestination.c_str());

         if (!::CopyFileW(wstring(p), wstring(pathDestination), TRUE))
         {
            DWORD dwLastError = ::GetLastError();
            string strError = get_last_error_string();
            fprintf(stderr, "Failed to copy file: %s to %s \n", p.c_str(), pathDestination.c_str());
            fprintf(stderr, "Error (%d) : %s\n", dwLastError, strError.c_str());
            fprintf(stderr, "Operation aborted\n");
            if (i == 0)
            {
               fprintf(stderr, "No files copied.\n", i);
            }
            else
            {
               fprintf(stderr, "Only %d files copied so far.\n", i);
            }
            return -1;

         }

      }
      printf("%d images copied.\n", pathaFiltered.get_count());
      return 0;

   }

   virtual void draw2d_factory_exchange() override
   {

   }

};










int main(int argc, char *argv[])
{

   if (!defer_aura_init())
   {

      return -1;

   }

   

   if (argc <= 4)
   {
      ::file::path p = argv[0];
      fprintf(stderr, "imagecopy: 2017 May (C) ca2 Desenvolvimento de Software Ltda.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "Usage:\n");
      fprintf(stderr, "%s \"Start Datetime\" \"End Datetime\" <Interval in Seconds> \"Destination Folder\" \n", p.title());
      fprintf(stderr, "\n");
      fprintf(stderr, "Examples:\n");
      fprintf(stderr, "%s 2017-04-28 18:30 2017-04-28 18:35 60 C:\\folder1 \n", p.title());
      fprintf(stderr, "%s 2017-04-28 18:30 18:35 60 C:\\folder1 \n", p.title());
      return -1;
   }

   int iRet = ::aura::app_main < ::imagecopy_app >(NULL, NULL, NULL, SW_HIDE);


   defer_aura_term();

   return iRet;

}

using namespace Gdiplus;
#undef new

::datetime::time imagecopy_app::get_image_time(string str)
{
   
   return m_mapImageTime[str];

}

bool imagecopy_app::touch_file(string strPath)
{
   bool bSet = false;

   UINT    size = 0;
   UINT    count = 0;
   wstring wstr(strPath);
   Bitmap* bitmap = new Bitmap(wstr);
   bitmap->GetPropertySize(&size, &count);
   //printf("There are %d pieces of metadata in the file.\n", count);
   //printf("The total size of the metadata is %d bytes.\n", size);
   if (count > 0)
   {

      PropertyItem* propertyItem = NULL;

      // Assume that the image has a property item of type PropertyItemEquipMake.
      // Get the size of that property item.
      size = bitmap->GetPropertyItemSize(PropertyTagExifDTDigitized);

      // Allocate a buffer to receive the property item.
      propertyItem = (PropertyItem*)malloc(size);

      // Get the property item.
      if (bitmap->GetPropertyItem(PropertyTagExifDTDigitized, size, propertyItem) == Gdiplus::Ok)
      {


         if (propertyItem->type == PropertyTagTypeASCII)
         {
          
             string str = (const char *) propertyItem->value;
             str.trim();
             int iPath = 0;

             int iPathCount = 0;
             m_mapImageTime.set_at(strPath, datetime().strtotime(NULL, str, iPath, iPathCount));

                bSet = true;

         }

      }

      free(propertyItem);
      

   }
   delete bitmap;

   return bSet;
}