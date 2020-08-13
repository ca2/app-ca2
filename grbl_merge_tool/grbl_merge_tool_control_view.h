#pragma once


namespace grbl_merge_tool
{


   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL control_view :
      virtual public ::user::impact
   {
   public:


      enum e_element
      {

         element_wx,
         element_wy,
         element_wz,
         element_mx,
         element_my,
         element_mz,
         element_xminus,
         element_xplus,
         element_yminus,
         element_yplus,
         element_zminus,
         element_zplus,
         element_step,
         element_preset001,
         element_preset01,
         element_preset1,
         element_preset5,
         element_preset10,
         element_preset100,
         element_none = -1
      };

      __pointer(main_form_list)               m_plist;
      __pointer(html_document)                m_pcontrolform;

      control_view(::aura::application * papp);
      virtual ~control_view();


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

      bool get_element_rect(LPRECT lprect, e_element eelement);

      void install_message_routing(::channel * pchannel);

      virtual void on_update(::user::impact * pimpact, e_update eupdate, ::object * pupdate) override;

      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics);

      DECL_GEN_SIGNAL(_001OnLButtonDown);

      e_element hit_test(const ::point & point);

   };



} // namespace formlist




