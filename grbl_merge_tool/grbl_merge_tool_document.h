#pragma once


namespace grbl_merge_tool
{


   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL document :
      public ::user::document
   {
   public:




      document(::aura::application * papp);
      virtual ~document();


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

      virtual bool on_new_document();
      virtual bool on_open_document(const var & varFile) override;
      virtual bool on_save_document(var varFile);

      virtual bool on_filemanager_open(::filemanager::manager * pmanager, var varFile);
      virtual bool on_filemanager_save(::filemanager::manager * pmanager, var varFile, bool bReplace = true);

      virtual i64 add_ref();

      virtual i64 dec_ref();





   };


} // namespace grbl_merge_tool















