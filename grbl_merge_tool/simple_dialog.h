#pragma once


class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL simple_dialog :
   virtual public ::user::form_callback,
   virtual public ::user::impact_creator
{
public:

   string                                    m_strForm;
   __pointer(::html_document)                       m_pdoc;
   __pointer(::user::single_document_template)      m_pdoctemplate;
   __pointer(::user::frame_window)                  m_pframe;
   __pointer(::html_view)                           m_pform;

   simple_dialog(::aura::application * papp, const char * pszForm);
   virtual ~simple_dialog();

   virtual void show();
   
   virtual bool on_control_event(::user::form_window * pview, ::user::control_event * pevent);


};

