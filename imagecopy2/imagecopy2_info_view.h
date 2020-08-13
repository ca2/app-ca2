#pragma once


namespace imagecopy2
{


   class CLASS_DECL_APP_CA2_IMAGECOPY2 info_view :
      virtual public ::user::impact
   {
   public:


      main_view *           m_pmainview;


      info_view(::aura::application * papp);
      virtual ~info_view();


      virtual void assert_valid() const;

      virtual void dump(dump_context & dumpcontext) const;

      void _001OnDraw(::draw2d::graphics_pointer & pgraphics);


   };


} // namespace imagecopy2




