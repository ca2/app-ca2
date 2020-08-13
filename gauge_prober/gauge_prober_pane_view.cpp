#include "framework.h"



namespace gauge_prober
{

   pane_view::pane_view(::aura::application * papp) :
      object(papp),
      ::user::tab(papp),
      ::user::tab_view(papp),
      ::userex::pane_tab_view(papp),
      place_holder_container(papp)
   {

   }


   pane_view::~pane_view()
   {

   }


   void pane_view::assert_valid() const
   {

      ::user::impact::assert_valid();

   }


   void pane_view::dump(dump_context & dumpcontext) const
   {

      ::user::impact::dump(dumpcontext);

   }


   void pane_view::install_message_routing(::channel * pchannel)
   {

      ::userex::pane_tab_view::install_message_routing(pchannel);

      IGUI_MSG_LINK(WM_CREATE, pchannel, this, &pane_view::_001OnCreate);

   }


   void pane_view::_001OnCreate(::message::message * pmessage)
   {

      if(pmessage->previous())
         return;
      double dSize = 18.0;
      get_data()->m_font->create_pixel_font("Verdana", dSize);
      get_data()->m_fontBold->create_pixel_font("Verdana", dSize);
      get_data()->m_fontUnderline->create_pixel_font("Verdana", dSize);
      get_data()->m_brushText->create_solid(ARGB(255, 120, 120, 112));
      get_data()->m_brushTextHover->create_solid(ARGB(255, 208, 85, 90));
      get_data()->m_brushTextSel->create_solid(ARGB(255, 255, 255, 255));
      get_data()->m_penBorder->create_solid(1.0, ARGB(255, 128, 128, 128));
      get_data()->m_penBorderSel->create_solid(5.0, ARGB(255, 0, 0, 0));
      get_data()->m_penBorderHover->create_solid(5.0, ARGB(255, 192, 192, 192));
      //get_data()->m_rectMargin.left = 32;
      int iMarginBottom= get_data()->m_rectMargin.bottom;
      get_data()->m_rectMargin.bottom = 0;
      add_tab("Flight Viewer", ::gauge_prober::impact_flight_viewer, true, true);
      add_tab("Flight Simulator", ::gauge_prober::impact_flight_simulator, true, true);
      add_tab("Control Terminal", ::gauge_prober::impact_control_terminal, true, true);
      add_tab("Info", ::gauge_prober::impact_info, true, true);

      set_cur_tab_by_id(::gauge_prober::impact_flight_viewer);

   }

   void pane_view::on_show_view()
   {

      ::userex::pane_tab_view::on_show_view();


      //if (m_pviewdata->m_id == "file_manager:import")
      //{

      //   filemanager_manager().FileManagerImport(m_pdocMain);

      //}
      //else if (m_pviewdata->m_id == "file_manager:export")
      //{

      //   filemanager_manager().FileManagerExport(m_pdocMain);

      //}


   }



   void pane_view::on_create_impact(::user::impact_creator_data * pcreatordata)
   {

      switch(pcreatordata->m_id)
      {
      case impact_flight_viewer:
      {
         create_view < flight_viewer >(pcreatordata);
      }

      break;
      case impact_flight_simulator:
      {
         create_view < flight_simulator >(pcreatordata);
      }

      break;
      case impact_control_terminal:
      {
         create_view < control_terminal >(pcreatordata);
      }

      break;
      case impact_info:
      {
         create_view < info >(pcreatordata);
      }
      break;
      }

      ::userex::pane_tab_view::on_create_impact(pcreatordata);
      pcreatordata->m_eflag.add(::user::impact_creator_data::flag_hide_all_others_on_show);

   }


   bool pane_view::on_control_event(::user::form_window * pview,::user::control_event * pevent)
   {


      return false;
   }




} // namespace gauge_prober





