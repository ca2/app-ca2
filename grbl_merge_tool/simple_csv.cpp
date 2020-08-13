#include "framework.h"


simple_csv::simple_csv(string strSeparator)
{
   
   m_bAddEmpty = true;
   m_strSeparator = strSeparator;

}


simple_csv::~simple_csv()
{

}



bool simple_csv::simple_csv_addup_file(var varFile)
{

   string str = Context.file().as_string(varFile);

   if (!simple_csv_add_text(str))
   {

      return false;

   }

   return true;

}

bool simple_csv::simple_csv_load_file(var varFile)
{

   string str = Context.file().as_string(varFile);

   if (!simple_csv_set_text(str))
   {

      return false;

   }

   return true;

}


bool simple_csv::simple_csv_save_file(var varFile)
{

   string str;
   
   if (!simple_csv_get_text(str))
   {

      return false;

   }

   if (!Context.file().put_contents(varFile, str))
   {

      return false;

   }

   return true;

}


bool simple_csv::simple_csv_set_text(string & str)
{

   m_stra.remove_all();

   return simple_csv_add_text(str);

}


bool simple_csv::simple_csv_add_text(string & str)
{

   stringa stra;

   stra.add_lines(str, m_bAddEmpty);

   if (stra.has_elements())
   {

      m_straHeader.explode(m_strSeparator, stra[0]);

   }

   for (index i = 1; i < stra.get_size(); i++)
   {

      stringa stra2;

      stra2.explode(m_strSeparator, stra[i]);

      if (!m_bAddEmpty)
      {

         stra2.trim();

         bool bAllEmpty = true;

         for (index j = 0; j < stra2.get_size(); j++)
         {

            if (stra2[j].has_char())
            {

               bAllEmpty = false;

               break;

            }


         }

         if (bAllEmpty)
         {

            continue;

         }

      }

      m_stra.add(stra2);

   }

   simple_csv_on_load();

   return true;

}


bool simple_csv::simple_csv_get_text(string & str)
{

   simple_csv_on_save();

   ::count c = m_stra.get_size();

   str = m_straHeader.implode(m_strSeparator.c_str());

   stringa stra;

   for (index i = 0; i < c; i++)
   {

      str += "\r\n";

      str += m_stra[i].implode(m_strSeparator.c_str());

   }

   return true;

}

void simple_csv::simple_csv_on_load()
{


}

void simple_csv::simple_csv_on_save()
{


}


void simple_csv::prepare_field(string & strText)
{

   strText.replace("\r", "");
   strText.replace("\n", "");
   strText.replace("\t", "");
   strText.replace(m_strSeparator, "");

}



