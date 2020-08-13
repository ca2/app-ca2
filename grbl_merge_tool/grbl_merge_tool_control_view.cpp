#include "framework.h"


namespace grbl_merge_tool
{


   control_view::control_view(::aura::application * papp) :
      object(papp)
   {

   }

   control_view::~control_view()
   {
   }


   void control_view::install_message_routing(::channel * pchannel)
   {

      ::user::impact::install_message_routing(pchannel);

      IGUI_MSG_LINK(WM_LBUTTONDOWN, pchannel, this, &control_view::_001OnLButtonDown);

   }


   void control_view::assert_valid() const
   {

      ::user::impact::assert_valid();

   }


   void control_view::dump(dump_context & dumpcontext) const
   {

      ::user::impact::dump(dumpcontext);

   }


   void control_view::update(::update * pupdate)
   {

      ::user::impact::on_update(pSender, eupdate, pupdate);

   }


   bool control_view::get_element_rect(LPRECT lprect, e_element eelement)
   {

      int iCol = eelement % 3;
      if (eelement >= element_wx && eelement <= element_mz)
      {

         lprect->left = 10 + 70 * iCol;
         lprect->right = lprect->left + 60;

         if (eelement >= element_wx && eelement <= element_wz)
         {
            lprect->top = 30;
            lprect->bottom = 60;
         }
         else
         {
            lprect->top = 100;
            lprect->bottom = 130;
         }
         return true;
      }
      iCol = (eelement-1) % 3;
      if (eelement >= element_preset001 && eelement <= element_preset100)
      {

         lprect->left = 10 + 70 * iCol;
         lprect->right = lprect->left + 60;

         if (eelement >= element_preset001 && eelement <= element_preset1)
         {
            lprect->top = 380;
            lprect->bottom = 420;
         }
         else
         {
            lprect->top = 440;
            lprect->bottom = 480;
         }
         return true;
      }

      int iSize = 60;
      if (eelement == element_xminus)
      {

         lprect->left = 10;
         lprect->top = 220;
         lprect->right = lprect->left + iSize;
         lprect->bottom = lprect->top + iSize;

      }
      else if (eelement == element_xplus)
      {

         lprect->left = 150;
         lprect->top = 220;
         lprect->right = lprect->left + iSize;
         lprect->bottom = lprect->top + iSize;

      }
      else if (eelement == element_yminus)
      {

         lprect->left = 80;
         lprect->top = 150;
         lprect->right = lprect->left + iSize;
         lprect->bottom = lprect->top + iSize;

      }
      else if (eelement == element_step)
      {

         lprect->left = 80;
         lprect->top = 220;
         lprect->right = lprect->left + iSize;
         lprect->bottom = lprect->top + iSize;

      }
      else if (eelement == element_yplus)
      {

         lprect->left = 80;
         lprect->top = 290;
         lprect->right = lprect->left + iSize;
         lprect->bottom = lprect->top + iSize;

      }
      else if (eelement == element_zminus)
      {

         lprect->left = 260;
         lprect->top = 290;
         lprect->right = lprect->left + iSize;
         lprect->bottom = lprect->top + iSize;

      }
      else if (eelement == element_zplus)
      {

         lprect->left = 260;
         lprect->top = 150;
         lprect->right = lprect->left + iSize;
         lprect->bottom = lprect->top + iSize;

      }
      else
      {

         return false;

      }

      return true;

   }

   void control_view::_001OnLButtonDown(::message::message * pmessage)
   {

      SCAST_PTR(::message::mouse, pmouse, pmessage);

      ::point point = pmouse->m_pt;

      ScreenToClient(point);

      e_element eelement = hit_test(point);

      if (eelement >= element_preset001 && eelement <= element_preset100)
      {

         Application.m_straCoord[element_step] = Application.m_straCoord[eelement];

      }
      else if(eelement == element_yplus)
      {
         // Query parser state to restore coordinate system, hide from table and console
         //Application.m_pcontrol->send_command("$G", -2);
         Application.m_pcontrol->send_command("G91G0Y" + Application.m_straCoord[element_step], -1);
      }
      else if (eelement == element_yminus)
      {
         //sendCommand("$G", -2, m_settings->showUICommands());
         Application.m_pcontrol->send_command("G91G0Y-" + Application.m_straCoord[element_step], -1);
      }
      else if (eelement == element_xplus)
      {
         // Query parser state to restore coordinate system, hide from table and console
         //Application.m_pcontrol->send_command("$G", -2);
         Application.m_pcontrol->send_command("G91G0X" + Application.m_straCoord[element_step], -1);
      }
      else if (eelement == element_xminus)
      {
         //sendCommand("$G", -2, m_settings->showUICommands());
         Application.m_pcontrol->send_command("G91G0X-" + Application.m_straCoord[element_step], -1);
      }
      else if (eelement == element_zplus)
      {
         // Query parser state to restore coordinate system, hide from table and console
         //Application.m_pcontrol->send_command("$G", -2);
         Application.m_pcontrol->send_command("G91G0Z" + Application.m_straCoord[element_step], -1);
      }
      else if (eelement == element_zminus)
      {
         //sendCommand("$G", -2, m_settings->showUICommands());
         Application.m_pcontrol->send_command("G91G0Z-" + Application.m_straCoord[element_step], -1);
      }

      pmouse->m_bRet = true;

   }

   control_view:: e_element control_view::hit_test(const ::point & point)
   {

      rect r;
      for (e_element eelement = element_wx; eelement <= element_preset100; ((int &)eelement)++)
      {
         if (get_element_rect(r, eelement))
         {
            if (r.contains(point))
            {
               return eelement;
            }
         }
      }
      return element_none;
   }

   void control_view::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      rect rectClient;

      GetClientRect(rectClient);

      pgraphics->FillSolidRect(rectClient, ARGB(255, 220, 220, 220));

      rect r;

      select_font(pgraphics);

      ::draw2d::font_pointer font(allocer());

      font->create_pixel_font("Tahoma", 16);

      pgraphics->SelectObject(font);

      pgraphics->set_text_color(ARGB(255, 0, 0, 0));

      pgraphics->text_out(10, 10, "Work Coordinates");

      pgraphics->text_out(10, 80, "Machine Coordinates");

      for (e_element eelement = element_wx; eelement <= element_mz; ((int &)eelement)++)
      {

         if (get_element_rect(r, eelement))
         {

            pgraphics->FillSolidRect(r, ARGB(255, 240, 240, 240));

            pgraphics->Draw3dRect(r, ARGB(255, 128, 128, 128), ARGB(255, 128, 128, 128));

            pgraphics->set_text_color(ARGB(255, 80, 80, 80));

            pgraphics->draw_text(Application.m_straCoord[eelement], r, DT_CENTER | DT_VCENTER);

         }

      }

      font->create_pixel_font("Arial", 20, 800);

      pgraphics->SelectObject(font);

      for (e_element eelement = element_xminus; eelement <= element_preset100; ((int &)eelement)++)
      {

         if (eelement == element_step)
         {

            font->create_pixel_font("Tahoma", 16);

            pgraphics->SelectObject(font);

         }

         if (get_element_rect(r, eelement))
         {

            if (eelement != element_step)
            {

               pgraphics->FillSolidRect(r, ARGB(255, 170, 170, 170));

               pgraphics->Draw3dRect(r, ARGB(255, 128, 128, 128), ARGB(255, 128, 128, 128));

            }

            pgraphics->set_text_color(ARGB(255, 80, 80, 80));

            pgraphics->draw_text(Application.m_straCoord[eelement], r, DT_CENTER | DT_VCENTER);

         }

      }

      pgraphics->set_text_color(ARGB(255, 80, 80, 80));

      pgraphics->text_out(10, 500, Application.m_strStatus);

   }


} // namespace grbl_merge_tool
