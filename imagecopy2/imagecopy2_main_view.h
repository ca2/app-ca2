#pragma once


namespace imagecopy2
{


   class CLASS_DECL_APP_CA2_IMAGECOPY2 main_view :
      virtual public ::user::split_view,
      virtual public ::user::form_callback
   {
   public:


      info_view *                m_pinfo;
      left_view *                m_pleft;
      ::html_form *              m_pform;
      __pointer(html_document)          m_phtmldoc;
      main_form_list *           m_plist;


      main_view(::aura::application * papp);
      virtual ~main_view();


      virtual void assert_valid() const;

      virtual void dump(dump_context & dumpcontext) const;

      void on_create_impacts();

      virtual void on_update(::user::impact * pSender, e_update eupdate, object* pHint);

      virtual bool on_control_event(::user::form_window * pview, ::user::control_event * pevent);

      void update_form(bool bSave);

   };


} // namespace imagecopy2




