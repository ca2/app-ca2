#include "framework.h"

namespace imagecopy2
{


   left_view::left_view(::aura::application * papp) :
      object(papp),
      ::user::split_layout(papp),
      ::user::split_view(papp),
      place_holder_container(papp)
   {


   }


   left_view::~left_view()
   {

   }


   void left_view::assert_valid() const
   {

      ::user::split_view::assert_valid();

   }


   void left_view::dump(dump_context & dumpcontext) const
   {

      ::user::split_view::dump(dumpcontext);

   }


   void left_view::update(::update * pupdate)
   {
      ::user::split_view::on_update(pSender, eupdate, pupdate);

   }




   void left_view::on_create_impacts()
   {

      if (get_pane_count() > 0)
         return;

      SetPaneCount(2);

      SetSplitOrientation(orientation_horizontal);

      set_position(0, 500);

      initialize_split_layout();

      m_pmainview=GetTypedParent<main_view>();

      m_phtmldoc = Application.create_child_form(m_pmainview, get_pane_holder(0));

      m_pform = m_phtmldoc->get_typed_view <::html_form>();
      //pleftview->create_views();

      m_pinfo = create_pane_view < info_view >(1);

      if (m_pinfo == NULL)
      {

         System.simple_message_box(NULL, "Could not create file info ::user::impact");

      }

   }







} // namespace imagecopy2



