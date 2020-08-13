#pragma once


namespace grbl_merge_tool
{


   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL split_view1 :
      virtual public ::user::split_view
   {
   public:


      __pointer(main_form_list)               m_plist;
      //__pointer(html_document)                m_pcontrolform;
      __pointer(control_view)                 m_pcontrol;


      split_view1(::aura::application * papp);
      virtual ~split_view1();


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      void install_message_routing(::channel * pchannel);

      virtual void on_update(::user::impact * pimpact, e_update eupdate, ::object * pupdate) override;

      virtual void on_create_impacts();


   };


   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL main_view :
      virtual public ::user::split_view
   {
   public:


      __pointer(::user::simple_view)          m_ptext;
      __pointer(::userex::top_edit_view)      m_pedit;
      __pointer(split_view1)                  m_psplit;


      main_view(::aura::application * papp);
      virtual ~main_view();


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;


      void install_message_routing(::channel * pchannel);

      virtual void on_update(::user::impact * pimpact, e_update eupdate, ::object * pupdate) override;

      virtual void on_create_impacts();


   };


} // namespace formlist




