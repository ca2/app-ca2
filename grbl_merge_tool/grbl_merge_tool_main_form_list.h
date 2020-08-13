#pragma once


namespace grbl_merge_tool
{



   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL main_form_list :
      virtual public ::simple_form_list_view
   {
   public:

      enum e_subitem
      {

         subitem_spacing,
         subitem_line,
         subitem_command,
         subitem_state,
         subitem_response

      };

      ::image_list                  m_i;
      __pointer(visual::icon)              m_piconUndefined;
      __pointer(visual::icon)              m_piconRunning;
      __pointer(visual::icon)              m_piconError;
      __pointer(visual::icon)              m_piconFullFail;
      __pointer(task_dialog)               m_pdialog;


      main_form_list(::aura::application * papp);
      virtual ~main_form_list();

      virtual bool get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction) override;

      virtual void _001OnShowControl(__pointer(control) pcontrol);

      virtual void _001InsertColumns() override;

      void install_message_routing(::channel * pchannel);

      void on_update(::user::impact * pSender, LPARAM lHint, ::object * pupdate);

      virtual void _001OnAfterChangeText(const ::action_context & action_context);

      virtual void _001OnTimer(timer * ptimer);

      void _001OnInitializeForm(__pointer(control) pcontrol);
      DECL_GEN_SIGNAL(_001OnCreate);

      array < command > * commanda();



      virtual bool keyboard_focus_OnSetFocus() override;



      virtual count _001GetItemCount();

      virtual void _001GetItemImage(::user::mesh_item * pitem) override;

      virtual void _001GetItemColor(::user::mesh_item * pitem) override;

      virtual void _001GetItemText(::user::mesh_item * pitem) override;

      virtual void _001SetItemText(::user::mesh_item * pitem) override;

      virtual void _001DrawSubItem(::user::draw_list_item * pdrawitem) override;

      virtual bool on_click(const ::user::item & item) override;

      virtual i32 _001CalcItemHeight(int iBaseHeight) override;

      i32 _001GetDrawTextFlags(::user::list::EView eview);

      virtual void _001DrawItem(::user::draw_list_item * pdrawitem);

      virtual void reset_items();

   };


} // namespace formlist




