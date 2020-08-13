#include "framework.h"



namespace twitter_automator
{


   pane_view::pane_view(::aura::application * papp) :
      object(papp),
      ::user::tab_view(papp),
      ::userex::pane_tab_view(papp),
      place_holder_container(papp)
   {
      m_pdocMain = NULL;
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


   void pane_view::_001OnCreate(message::message * pobj)
   {
      if(pobj->previous())
         return;

      add_tab("Twitter Automator", ::twitter_automator::PaneViewTwitterAutomator, true, true);
      add_tab("Import", "file_manager:import", true, true);
      add_tab("Export", "file_manager:export", true, true);

      set_cur_tab_by_id(::twitter_automator::PaneViewTwitterAutomator);

   }

   void pane_view::on_show_view()
   {

      ::userex::pane_tab_view::on_show_view();

      if (m_pviewdata->m_id == "file_manager:import")
      {

         filemanager_manager().FileManagerImport(m_pdocMain);

      }
      else if (m_pviewdata->m_id == "file_manager:export")
      {

         m_pdocMain->m_filepath = "Twitter Automator.csv";

         filemanager_manager().FileManagerExport(m_pdocMain);

      }


   }


   void pane_view::on_create_impact(::user::impact_creator_data * pcreatordata)
   {

      switch(pcreatordata->m_id)
      {
      case PaneViewTwitterAutomator:
      {

         m_pdocMain=dynamic_cast < document * > (Application.m_ptemplateTwitterAutomatorFormList->open_document_file(get_context_application(), var(), true, pcreatordata->m_pholder));

      }

      break;
      }

      ::userex::pane_tab_view::on_create_impact(pcreatordata);

      pcreatordata->m_eflag.add(::user::impact_creator_data::flag_hide_all_others_on_show);

   }


   void pane_view::on_control_event(::user::control_event * pevent)
   {



   }




} // namespace twitter_automator





