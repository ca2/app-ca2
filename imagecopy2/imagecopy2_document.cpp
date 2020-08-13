#include "framework.h"


namespace imagecopy2
{


   document::document(::aura::application * papp) :
      object(papp),
      ::data::data_container_base(papp),
	   ::user::controller(papp),
      ::user::document(papp)
   {
      m_bOpen = false;
   }


   document::~document()
   {

   }


   void document::assert_valid() const
   {

	   ::user::document::assert_valid();

   }


   void document::dump(dump_context & dumpcontext) const
   {

	   ::user::document::dump(dumpcontext);

   }


   bool document::on_new_document()
   {

      string str;

      data_get("open_folder", str);

      if (str.has_char())
      {

         m_filepath = str;

         update_all_views(NULL, document::update_import);

      }


      return ::user::document::on_new_document();

   }


   bool document::on_open_document(const var & varFile)
   {

      m_filepath = varFile;

      update_all_views(NULL, update_import);

      return true;

   }

   bool document::on_save_document(var varFile)
   {

      m_filepath = varFile;

      update_all_views(NULL, update_export);

      return true;

   }

   
   bool document::on_filemanager_open(::filemanager::manager * pmanager, var varFile)
   {

      if (::user::document::on_filemanager_open(pmanager, varFile))
      {

         get_typed_view < main_form_list>()->GetTypedParent<pane_view>()->set_cur_tab_by_id(PaneViewImageCopy);

         return true;

      }

      return false;

   }


   bool document::on_filemanager_save(::filemanager::manager * pmanager, var varFile, bool bReplace)
   {

      if (::user::document::on_filemanager_save(pmanager, varFile, bReplace))
      {

         get_typed_view < main_form_list>()->GetTypedParent<pane_view>()->set_cur_tab_by_id(PaneViewImageCopy);

         return true;

      }

      return false;

   }


   i64 document::add_ref()
   {

      return  ::user::document::add_ref();

   }

   i64 document::dec_ref()
   {

      return  ::user::document::dec_ref();

   }



} // namespace imagecopy2

















