#pragma once



class CLASS_DECL_APP_CORE_TWITTER_AUTOMATOR simple_csv :
   virtual public ::object
{
public:

   
   stringa              m_straHeader;
   array < stringa >    m_stra;
   bool                 m_bAddEmpty;
   string               m_strSeparator;


   simple_csv(string strSeparator = ",");
   virtual ~simple_csv();

   virtual bool simple_csv_addup_file(var varFile);
   virtual bool simple_csv_load_file(var varFile);
   virtual bool simple_csv_save_file(var varFile);

   virtual bool simple_csv_set_text(string & str);
   virtual bool simple_csv_add_text(string & str);
   virtual bool simple_csv_get_text(string & str);

   virtual void simple_csv_on_load();
   virtual void simple_csv_on_save();




   //virtual void simple_csv_add_line(const string & str);

   //virtual void simple_csv_on_before_load(::count c);
   //virtual void simple_csv_set_line(index iLine, stringa & strLine);
   //virtual void simple_csv_get_line(index iLine, stringa & strLine);
   //virtual ::count simple_csv_get_line_count();

   virtual void prepare_field(string & strText);

};