#include "framework.h"


simple_dialog::simple_dialog(::aura::application * papp, const char * pszForm) :
   ::object(papp)
{

   m_pdoc = NULL;
   m_strForm = pszForm;

   m_pdoctemplate = new ::user::single_document_template(
   get_context_application(),
   "form",
   System.type_info < html_document >(),
   System.type_info < simple_frame_window >(),
   System.type_info < html_view >());

   m_pdoctemplate->m_bQueueDocumentOpening = false;

}

simple_dialog::~simple_dialog()
{

   //m_pdoctemplate->close_all_documents(false);

}


void simple_dialog::show()
{

   if (m_pdoc == NULL)
   {

      m_pdoc = m_pdoctemplate->open_document_file(get_context_application(), m_strForm, false);

      if (m_pdoc.is_null())
      {

         return;

      }

   }

   m_pform = m_pdoc->get_typed_view< ::html_view>();

   if (m_pform.is_null())
   {

      return;

   }

   m_pform->m_pcallback = this;

   m_pframe = m_pform->GetTopLevelFrame();

   m_pframe->InitialFramePosition();

   m_pframe->ShowWindow(SW_SHOW);

   if (m_pform->get_html_data()->m_bImplemented)
   {

      ::user::control_event ev;
      ev.m_puie = m_pform;
      ev.m_eevent = ::user::event_form_initialize;
      on_control_event(&ev);

   }

}




void simple_dialog::on_control_event(::user::control_event * pevent)
{
   if (pevent->m_eevent == ::user::event_form_initialize)
   {

   }
   else if (pevent->m_eevent == ::user::event_button_clicked
            || pevent->m_eevent == ::user::event_enter_key)
   {
      if (pevent->m_puie->m_id == "submit" ||
            pevent->m_eevent == ::user::event_enter_key)
      {
         //__pointer(::user::interaction) pinteraction = m_pviewAuth->get_child_by_name("pin");
         //__pointer(::user::elemental) ptext = (pinteraction.m_p);
         //ptext->_001GetText(m_strPin);
         //m_ptabview->get_wnd()->EndModalLoop(IDOK);
         //m_ptabview->GetParentFrame()->ShowWindow(SW_HIDE);
      }
   }



}

