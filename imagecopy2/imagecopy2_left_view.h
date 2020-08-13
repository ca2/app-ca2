#pragma once


namespace imagecopy2
{


   class CLASS_DECL_APP_CA2_IMAGECOPY2 left_view :
      virtual public ::user::split_view
   {
   public:


      ::html_form *              m_pform;
      __pointer(html_document)          m_phtmldoc;
      info_view *                m_pinfo;
      main_view *                m_pmainview;


      left_view(::aura::application * papp);
      virtual ~left_view();


      virtual void assert_valid() const;

      virtual void dump(dump_context & dumpcontext) const;

      void on_create_impacts();

      virtual void on_update(::user::impact * pSender, e_update eupdate, object* pHint);


   };


} // namespace imagecopy2




