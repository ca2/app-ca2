#include "framework.h"

double range_rate(double d1, double d2, double dRate)
{

   return (d2 - d1) * dRate + d1;

}
namespace gauge_prober
{


   group::group(::aura::application * papp) :
      ::object(papp),
      m_chartEgt(papp),
      m_chartRpm(papp),
      m_chartThrottle(papp),
      m_gaugeRpm(papp),
      m_gaugeEgt(papp),
      m_gaugeVout(papp),
      m_gaugeVbatt(papp),
      m_gaugeThrottle(papp),
      m_throttle(papp)
   {

      m_iTimeMouse = -1;

      m_sizeBorder = 40;
      m_pflight = NULL;
      m_peditBrowse = NULL;
      m_pbuttonBrowse = NULL;

      defer_create_mutex();

   }


   group::~group()
   {


   }


   void group::create(flight * pflight, index iIndex)
   {

      m_pflight = pflight;

      m_iIndex = iIndex;

      if (pflight->m_eview == impact_flight_viewer)
      {

         m_chartEgt.create(this);
         m_chartRpm.create(this);
         m_chartThrottle.create(this);

      }
      else
      {

         m_gaugeRpm.create(this);
         m_gaugeEgt.create(this);
         m_gaugeVout.create(this);
         m_gaugeVbatt.create(this);
         m_gaugeThrottle.create(this);

      }

      if (pflight->m_eview == impact_control_terminal)
      {

         m_throttle.create(this);

         m_gaugeRpm.m_bControl = true;
         m_gaugeEgt.m_bControl = true;
         m_gaugeVout.m_bControl = true;
         m_gaugeVbatt.m_bControl = true;
         m_gaugeThrottle.m_bControl = true;

      }

      if (pflight->m_eview != impact_control_terminal)
      {

         m_strIndex = ::str::from(iIndex);

         pflight->get_document()->data_get("file" + m_strIndex, m_setFile);

         string strFileName;

         strFileName = m_setFile["file_name"];

         {

            m_peditBrowse = new edit_box(get_context_application());

            ::user::control_descriptor d;

            d.m_id = "edit_browse" + m_strIndex;
            d.m_puiParent = m_pflight;
            d.m_etype = ::user::control_type_edit_plain_text;

            m_peditBrowse->create_control(&d);

            m_peditBrowse->_001SetText(m_setFile["file_name"], ::source_initialize);

         }

         {

            m_pbuttonBrowse = new button(get_context_application());

            ::user::control_descriptor d;

            d.m_id = "button_browse" + m_strIndex;
            d.m_puiParent = m_pflight;
            d.m_etype = ::user::control_type_button;

            m_pbuttonBrowse->create_control(&d);

            m_pbuttonBrowse->set_window_text("Browse");

         }

      }

   }



   void group::on_draw(::draw2d::graphics_pointer & pgraphics)
   {

      if (m_pflight == NULL)
      {

         return;

      }

      if (m_pflight->m_eview == impact_flight_viewer)
      {

         m_chartEgt.on_draw(pgraphics);

         m_chartRpm.on_draw(pgraphics);

         m_chartThrottle.on_draw(pgraphics);

      }
      else
      {

         m_gaugeRpm.on_draw(pgraphics);

         m_gaugeEgt.on_draw(pgraphics);

         // Vout is not shown

         m_gaugeVbatt.on_draw(pgraphics);

         m_gaugeThrottle.on_draw(pgraphics);

      }

      if (m_pflight->m_eview == impact_control_terminal)
      {

         m_throttle.on_draw(pgraphics);

      }

      //pgraphics->set_text_color(ARGB(255, 128, 128, 128));

      string strError = m_pflight->get_group_error_message(this);

      if (strError.has_char())
      {

         //pgraphics->FillSolidRect(m_rectMessage, ARGB(255, 128, 128, 128));

         rect rMessage(m_rectMessage);

         //rMessage.deflate(2, 2);

         ::draw2d::font_pointer font(allocer());

         font->create_pixel_font("Verdana", MIN(18, rMessage.height()/2));

         pgraphics->selectFont(font);

         pgraphics->set_text_color(ARGB(255, 255, 100, 100));

         pgraphics->text_out(rMessage.left, rMessage.top, strError);


      }

   }



   bool group::on_control_event(::user::control_event * pevent)
   {

      if (pevent->m_eevent == ::user::event_after_change_text)
      {

         if (pevent->m_context.is_user_source())
         {

            if (pevent->m_puie->m_id == ("edit_browse" + m_strIndex))
            {

               {

                  synch_lock sl(mutex());

                  m_peditBrowse->_001GetText(m_setFile["file_name"]);

                  m_pflight->get_document()->data_set("file" + m_strIndex, m_setFile);

               }

               m_pflight->update_file();

            }

         }

      }
      else if (pevent->m_eevent == ::user::event_button_clicked
               || pevent->m_eevent == ::user::event_enter_key)
      {

         if (pevent->m_puie->m_id == ("button_browse" + m_strIndex))
         {

            property_set & set = m_setFile;

            set["file_filter_specs"].stra().add("*.log");
            set["file_filter_specs"].stra().add("*.*");
            set["file_filter_names"].stra().add("Log Files");
            set["file_filter_names"].stra().add("All Files");
            set["default_file_extension"].stra().add("log");

            if (Context.os().browse_file_open(m_pflight->get_handle(), set))
            {

               m_pflight->get_document()->data_set("file"+m_strIndex, m_setFile);

               m_peditBrowse->_001SetText(m_setFile["file_name"], ::source_sync);

               m_pflight->update_file();

            }

            m_peditBrowse->SetFocus();

            return true;

         }

      }

      return false;

   }


   void group::update_file()
   {



      synch_lock sl(mutex());

      synch_lock sl1(m_chartEgt.mutex());
      synch_lock sl2(m_chartRpm.mutex());
      synch_lock sl3(m_chartThrottle.mutex());
      synch_lock sl4(m_gaugeRpm.mutex());
      synch_lock sl5(m_gaugeEgt.mutex());
      synch_lock sl6(m_gaugeThrottle.mutex());
      synch_lock sl7(m_gaugeVout.mutex());
      synch_lock sl8(m_gaugeVbatt.mutex());

      if (m_pflight == NULL)
      {

         return;

      }

      Context.file().as_memory(m_setFile["file_name"], m_mem);

      bool bLast255 = false;
      bool bLast5 = false;

      m_eengine = engine_none;

      pointd_array * pegt = NULL;
      pointd_array * prpm = NULL;
      pointd_array * pvout = NULL;
      pointd_array * pvbatt = NULL;
      pointd_array * pthrottle = NULL;

      if (m_pflight->m_eview == impact_flight_viewer)
      {

         m_chartEgt.m_pointda.remove_all();
         m_chartRpm.m_pointda.remove_all();
         m_chartThrottle.m_pointda.remove_all();

         pegt = &m_chartEgt.m_pointda;
         prpm = &m_chartRpm.m_pointda;
         pthrottle = &m_chartThrottle.m_pointda;

      }
      else if (m_pflight->m_eview == impact_flight_simulator)
      {
         m_gaugeRpm.m_pointda.remove_all();
         m_gaugeEgt.m_pointda.remove_all();
         m_gaugeVout.m_pointda.remove_all();
         m_gaugeVbatt.m_pointda.remove_all();
         m_gaugeThrottle.m_pointda.remove_all();

         prpm = &m_gaugeRpm.m_pointda;
         pegt = &m_gaugeEgt.m_pointda;
         pvout = &m_gaugeVout.m_pointda;
         pvbatt = &m_gaugeVbatt.m_pointda;
         pthrottle = &m_gaugeThrottle.m_pointda;

      }

      //if (pegt && pegt->is_empty())     pegt = NULL;
      //if (prpm && prpm->is_empty())     prpm = NULL;
      //if (pvolt && pvolt->is_empty())     pvolt = NULL;
      //if (pthrottle && pthrottle->is_empty())     pthrottle = NULL;

      double dDataSize = 0.0;

      double dLast1 = 0.0;
      double dLast2 = 0.0;
      double dLast3 = 0.0;
      double dLast5 = 0.0;
      double dLast6 = 0.0;
      double d1 = 0.0;
      double d2 = 0.0;
      double d3 = 0.0;
      double d5 = 0.0;
      double d6 = 0.0;

      if (prpm || pegt || pvout || pvbatt || pthrottle)
      {

         int iTime = 0;

         bool bTrack = false;

         int iError = 0;

         bool bFile25 = m_setFile["file_name"].get_string().find("25.log") > 0;

         bFile25 = true;

         auto iSize = m_mem.get_size();

         u64 i = 0;

         if (bFile25)
         {

            bTrack = true;

            //i += atoi(file_as_string_dup("C:\\a\\1.txt"));
            i += 269;

         }

         for (; i < iSize; )
         {

            byte b = m_mem[i];

            bool b255 = b == 255;
            bool b5 = bLast255 && b == 25;

            ::datetime::time time;

            if (bLast5)
            {

               i += 5;

               char * psz = (char *)&m_mem[i];

               string str(psz);

               strsize s = str.find('.');

               if (s > 0)
               {

                  time = ::datetime::time::time(0);

               }

               i += 12;

            }
            else if (bLast255 && !b255)
            {

               D1 * pdata = (D1 *)&m_mem[i];

               if (pdata->e.bL == 0)
               {

                  m_eengine = pdata->e.engine;

               }

               //m_memData1.allocate_add_up(1);
               //m_memData1[m_memData1.get_size() -1] = pdata->data[-1];

               dLast1 = pdata->data[1] * 4.6 - 50.0;
               dLast2 = pdata->data[0] * 500.0;
               dLast3 = pdata->data[2] / 2.0;
               dLast5 = pdata->data[3] * 8.3 / 255;
               dLast6 = 7.0 + pdata->data[4] * 9.03 / 255.0;

               if (pegt) pegt->add(pointd(iTime, dLast1));
               if (prpm) prpm->add(pointd(iTime, dLast2));
               if (pthrottle) pthrottle->add(pointd(iTime, dLast3));
               if (pvout) pvout->add(pointd(iTime, dLast5));
               if (pvbatt) pvbatt->add(pointd(iTime, dLast6));

               iTime++;

               bTrack = true;

               i += 5;

            }
            else if (b255)
            {

               bTrack = false;
               i++;

            }
            else if (bTrack)
            {

               D1 * pdata = (D1 *)&m_mem[i];

               if (pdata->data1 == 0)
               {

                  iError++;
                  if (pdata->data[0] == 0
                        && pdata->data[1] == 'S'
                        && pdata->data[2] == 'T'
                        && pdata->data[3] == 'O'
                        && pdata->data[4] == 'P')
                  {

                     break;

                  }

               }
               else
               {

                  //               m_chartRpm1.m_pointda.add(pointd(iTime, pdata.data[0] * 500.0));

                  //m_memData1.allocate_add_up(1);
                  //m_memData1[m_memData1.get_size() - 1] = pdata->data[-1];
                  d1 = pdata->data[1] * 4.6 - 50.0;
                  d2 = pdata->data[0] * 500.0;
                  d3 = pdata->data[2] / 2.0;
                  d5 = pdata->data[3] * 8.3 / 255.0;
                  d6 = 7.0 + pdata->data[4] * 9.03 / 255.0;

                  for (int j = 1; j <= 99; j ++)
                  {
                     double dRate = (double)j / 100.0;
                     double dTime;
                     dTime = iTime;
                     dTime -= 1.0;
                     dTime += dRate;

                     if (pegt) pegt->add(pointd(dTime, range_rate(dLast1, d1, dRate)));
                     if (prpm) prpm->add(pointd(dTime, range_rate(dLast2, d2, dRate)));
                     if (pthrottle) pthrottle->add(pointd(dTime, range_rate(dLast3, d3, dRate)));
                     if (pvout) pvout->add(pointd(dTime, range_rate(dLast5, d5, dRate)));
                     if (pvbatt) pvbatt->add(pointd(dTime, range_rate(dLast6, d6, dRate)));

                  }

                  if (pegt) pegt->add(pointd(iTime, d1));
                  if (prpm) prpm->add(pointd(iTime, d2));
                  if (pthrottle) pthrottle->add(pointd(iTime, d3));
                  if (pvout) pvout->add(pointd(iTime, d5));
                  if (pvbatt) pvbatt->add(pointd(iTime, d6));


                  dLast1 = d1;
                  dLast2 = d2;
                  dLast3 = d3;
                  dLast5 = d5;
                  dLast6 = d6;


               }

               iTime++;

               i += 6;

            }
            else
            {


               i++;

            }

            bLast5 = b5;
            bLast255 = b255;

         }

         if (pegt && pegt->has_elements() && pegt->last().x > dDataSize) dDataSize = pegt->last().x;
         if (prpm && prpm->has_elements() && prpm->last().x > dDataSize) dDataSize = prpm->last().x;
         if (pthrottle && pthrottle->has_elements() && pthrottle->last().x > dDataSize) dDataSize = pthrottle->last().x;
         if (pvout && pvout->has_elements() && pvout->last().x > dDataSize) dDataSize = pvout->last().x;
         if (pvbatt && pvbatt->has_elements() && pvbatt->last().x > dDataSize) dDataSize = pvbatt->last().x;

      }

      m_dEnd = MAX(1.0, dDataSize);

      m_sizeBorder.cx = 40;
      int iM = (m_rect.right + m_rect.left) / 2;
      int y = m_rect.top;
      int iW = m_rect.width() - m_sizeBorder.cx * 2;
      int x = m_rect.left + m_sizeBorder.cx;
      int x2 = x;
      int iH;
      int iGaugeW = 0;
      int y3 = 0;
      if (m_pflight->m_eview == impact_flight_viewer)
      {

         iH = m_rect.height() / 3;

         {
            synch_lock sl(m_chartEgt.mutex());

            m_chartEgt.m_etype = line_chart::type_egt;
            m_chartEgt.m_dMin = 0 * 4.6 - 50;
            m_chartEgt.m_dMax = 254 * 4.6 - 50;
            m_chartEgt.m_strMin = unitext("-50");
            m_chartEgt.m_strMax = unitext("1118");
            m_chartEgt.m_strTitle = unitext("EGT °C");
            m_chartEgt.m_rect.left = x;
            m_chartEgt.m_rect.right = x + iW;
            m_chartEgt.m_rect.top = y;
            m_chartEgt.m_rect.bottom = y + iH;

         }

         y += iH + 10;

         {
            synch_lock sl(m_chartRpm.mutex());

            m_chartRpm.m_etype = line_chart::type_rpm;
            m_chartRpm.m_dMin = 0 * 500.0;
            m_chartRpm.m_dMax = 254 * 500.0;
            m_chartRpm.m_strMin = unitext("0");
            m_chartRpm.m_strMax = unitext("127k");
            m_chartRpm.m_strTitle = "RPM";
            m_chartRpm.m_rect.left = x;
            m_chartRpm.m_rect.right = x + iW;
            m_chartRpm.m_rect.top = y;
            m_chartRpm.m_rect.bottom = y + iH;
         }

         y += iH + 10;

         {
            synch_lock sl(m_chartThrottle.mutex());

            m_chartThrottle.m_etype = line_chart::type_throttle;
            m_chartThrottle.m_dMin = 0.0;
            m_chartThrottle.m_dMax = 100.0;
            m_chartThrottle.m_strMin = unitext("0");
            m_chartThrottle.m_strMax = unitext("100");
            m_chartThrottle.m_strTitle = "% Throttle";
            m_chartThrottle.m_rect.left = x;
            m_chartThrottle.m_rect.right = x + iW;
            m_chartThrottle.m_rect.top = y;
            m_chartThrottle.m_rect.bottom = y + iH;
         }

         y += iH + 10;

      }
      else
      {

         int iH2 = m_rect.height();

         double dAspect = 1.1;

         int iWMax = (int) ((iH2 - m_sizeBorder.cx) / (2 * dAspect));

         iGaugeW = MIN((iW - m_sizeBorder.cx) / 2, iWMax);



         if (m_pflight->m_eview == impact_flight_simulator)
         {
            x2 = m_rect.right - iGaugeW - m_sizeBorder.cx;

            int y2 = y;

            iH = (LONG) (iGaugeW * dAspect);

            int iVSpace = iH2 - iH * 2;




            {
               synch_lock sl(m_gaugeRpm.mutex());

               m_gaugeRpm.m_etype = gauge::type_rpm;
               m_gaugeRpm.m_dMin = 0 * 500.0;
               m_gaugeRpm.m_dMax = 254 * 500.0;
               m_gaugeRpm.m_dScaleMin = 000;
               m_gaugeRpm.m_dScaleMax = 130000;
               m_gaugeRpm.m_iCount = 13;
               m_gaugeRpm.m_strTitle = unitext("RPM");
               m_gaugeRpm.m_strTitle1 = unitext("RPM");
               if (m_iIndex <= 1)
               {
                  m_gaugeRpm.m_rect.left = x;
                  m_gaugeRpm.m_rect.right = x + iGaugeW;
               }
               else
               {
                  m_gaugeRpm.m_rect.left = x2;
                  m_gaugeRpm.m_rect.right = x2 + iGaugeW;
               }
               m_gaugeRpm.m_rect.top = y2;
               m_gaugeRpm.m_rect.bottom = y2 + iH;

            }

            y3 = y2 + iH / 3;

            {
               synch_lock sl(m_gaugeThrottle.mutex());

               m_gaugeThrottle.m_etype = gauge::type_throttle;
               m_gaugeThrottle.m_dMin = 0;
               m_gaugeThrottle.m_dMax = 100.0;
               m_gaugeThrottle.m_dScaleMin = 0;
               m_gaugeThrottle.m_dScaleMax = 100;
               m_gaugeThrottle.m_iCount = 100;
               m_gaugeThrottle.m_strTitle = unitext("0%");
               m_gaugeThrottle.m_strTitle1 = unitext("% Throttle");
               if (m_iIndex <= 1)
               {
                  m_gaugeThrottle.m_rect.left = x2;
                  m_gaugeThrottle.m_rect.right = x2 + iGaugeW;
               }
               else
               {
                  m_gaugeThrottle.m_rect.left = x;
                  m_gaugeThrottle.m_rect.right = x + iGaugeW;
               }
               m_gaugeThrottle.m_rect.bottom = y2 + iH;
               m_gaugeThrottle.m_rect.top = m_gaugeThrottle.m_rect.bottom - iH / 3;
               m_gaugeThrottle.m_bDigital = true;

            }

            y2 += iH + iVSpace;

            {
               synch_lock sl(m_gaugeEgt.mutex());

               m_gaugeEgt.m_etype = gauge::type_egt;
               m_gaugeEgt.m_dMin = 0 * 4.6 - 50;
               m_gaugeEgt.m_dMax = 254 * 4.6 - 50;
               m_gaugeEgt.m_dScaleMin = -100;
               m_gaugeEgt.m_dScaleMax = 1200;
               m_gaugeEgt.m_iCount = 13;
               m_gaugeEgt.m_strTitle = unitext("EGT °C");
               m_gaugeEgt.m_strTitle1 = unitext("EGT");
               if (m_iIndex <= 1)
               {
                  m_gaugeEgt.m_rect.left = x;
                  m_gaugeEgt.m_rect.right = x + iGaugeW;
               }
               else
               {
                  m_gaugeEgt.m_rect.left = x2;
                  m_gaugeEgt.m_rect.right = x2 + iGaugeW;
               }
               m_gaugeEgt.m_rect.top = y2;
               m_gaugeEgt.m_rect.bottom = y2 + iH;
               m_gaugeEgt.m_bMilli = false;
               m_gaugeEgt.m_bKilo = false;
            }

            {
               synch_lock sl(m_gaugeVbatt.mutex());

               m_gaugeVbatt.m_etype = gauge::type_volt;
               m_gaugeVbatt.m_dMin = 7.0;
               m_gaugeVbatt.m_dMax = 7.0 + 9.03 * 254 / 255;
               m_gaugeVbatt.m_dScaleMin = 7;
               m_gaugeVbatt.m_dScaleMax = 17.0;
               m_gaugeVbatt.m_iCount = 10;
               m_gaugeVbatt.m_strTitle1 = "Volt";
               m_gaugeVbatt.m_strTitle = "V";
               if (m_iIndex <= 1)
               {
                  m_gaugeVbatt.m_rect.left = x2;
                  m_gaugeVbatt.m_rect.right = x2 + iGaugeW;
               }
               else
               {
                  m_gaugeVbatt.m_rect.left = x;
                  m_gaugeVbatt.m_rect.right = x + iGaugeW;
               }
               m_gaugeVbatt.m_rect.top = y2;
               m_gaugeVbatt.m_rect.bottom = y2 + iH;
            }

            y += iH2;

         }
         else // impact_control_terminal
         {

            iGaugeW = (LONG)(iGaugeW * 0.9);

            x2 = m_rect.right - iGaugeW - m_sizeBorder.cx;

            int y2 = y;

            iH = (LONG) (iGaugeW * dAspect);

            int iVSpace = iH2 - iH * 2;

            {
               synch_lock sl(m_gaugeRpm.mutex());

               m_gaugeRpm.m_etype = gauge::type_rpm;
               m_gaugeRpm.m_dMin = 0 * 500.0;
               m_gaugeRpm.m_dMax = 254 * 500.0;
               m_gaugeRpm.m_dScaleMin = 000;
               m_gaugeRpm.m_dScaleMax = 130000;
               m_gaugeRpm.m_iCount = 13;
               m_gaugeRpm.m_strTitle = unitext("RPM");
               m_gaugeRpm.m_strTitle1 = unitext("RPM");
               m_gaugeRpm.m_rect.left = iM - iGaugeW / 2;
               m_gaugeRpm.m_rect.right = iM + iGaugeW / 2;
               m_gaugeRpm.m_rect.top = y2;
               m_gaugeRpm.m_rect.bottom = y2 + iH;

            }

            y2 += iGaugeW / 2;

            {
               synch_lock sl(m_gaugeEgt.mutex());

               m_gaugeEgt.m_etype = gauge::type_egt;
               m_gaugeEgt.m_dMin = 0 * 4.6 - 50;
               m_gaugeEgt.m_dMax = 254 * 4.6 - 50;
               m_gaugeEgt.m_dScaleMin = -100;
               m_gaugeEgt.m_dScaleMax = 1200;
               m_gaugeEgt.m_iCount = 13;
               m_gaugeEgt.m_strTitle = unitext("EGT °C");
               m_gaugeEgt.m_strTitle1 = unitext("EGT");
               m_gaugeEgt.m_rect.left = x + iGaugeW / 2;
               m_gaugeEgt.m_rect.right = m_gaugeEgt.m_rect.left + iGaugeW;
               m_gaugeEgt.m_rect.top = y2;
               m_gaugeEgt.m_rect.bottom = y2 + iH;
               m_gaugeEgt.m_bMilli = false;
               m_gaugeEgt.m_bKilo = false;
            }

            {
               synch_lock sl(m_gaugeVbatt.mutex());

               m_gaugeVbatt.m_etype = gauge::type_volt;
               m_gaugeVbatt.m_dMin = 7.0;
               m_gaugeVbatt.m_dMax = 7.0 + 9.03 * 254 / 255;
               m_gaugeVbatt.m_dScaleMin = 7.0;
               m_gaugeVbatt.m_dScaleMax = 17.0;
               m_gaugeVbatt.m_iCount = 10;
               m_gaugeVbatt.m_strTitle1 = "Volt";
               m_gaugeVbatt.m_strTitle = "V";
               m_gaugeVbatt.m_rect.right = x + iW - iGaugeW / 2;
               m_gaugeVbatt.m_rect.left = m_gaugeVbatt.m_rect.right - iGaugeW;
               m_gaugeVbatt.m_rect.top = y2;
               m_gaugeVbatt.m_rect.bottom = y2 + iH;
            }

            y2 += iGaugeW * 2 / 3;

            {
               synch_lock sl(m_gaugeThrottle.mutex());

               m_gaugeThrottle.m_etype = gauge::type_throttle;
               m_gaugeThrottle.m_dMin = 0;
               m_gaugeThrottle.m_dMax = 100.0;
               m_gaugeThrottle.m_dScaleMin = 0;
               m_gaugeThrottle.m_dScaleMax = 100;
               m_gaugeThrottle.m_iCount = 100;
               m_gaugeThrottle.m_strTitle = unitext("0%");
               m_gaugeThrottle.m_strTitle1 = unitext("% Throttle");
               m_gaugeThrottle.m_rect.left = iM - iGaugeW / 2;
               m_gaugeThrottle.m_rect.right = iM + iGaugeW / 2;
               m_gaugeThrottle.m_rect.top = y2;
               m_gaugeThrottle.m_rect.bottom = y2 + iH / 3;
               m_gaugeThrottle.m_bDigital = true;

            }

            y2 += iH / 3;
            y2 += 50;


            {


               y3 = y2;
               int iTH = (int)(m_rect.bottom - y2);
               int iTW = (int)(iTH * 0.32);


               synch_lock sl(m_throttle.mutex());

               m_throttle.m_etype = throttle::type_throttle;
               m_throttle.m_dMin = 0;
               m_throttle.m_dMax = 100.0;
               m_throttle.m_dScaleMin = 0;
               m_throttle.m_dScaleMax = 100;
               m_throttle.m_iCount = 100;
               m_throttle.m_strTitle = unitext("0%");
               m_throttle.m_strTitle1 = unitext("% Throttle");
               m_throttle.m_rect.left = iM - iTW / 2;
               m_throttle.m_rect.right = iM + iTW / 2;
               m_throttle.m_rect.top = m_rect.bottom - iTH;
               m_throttle.m_rect.bottom = m_rect.bottom;
               m_throttle.m_bDigital = true;

            }


            y2 += iH + iVSpace;



         }

      }


      if (m_pflight->m_eview == impact_control_terminal)
      {

         m_rectMessage.top = y3;
         m_rectMessage.bottom = m_rect.bottom;
         m_rectMessage.left = x;
         m_rectMessage.right = x + iW;

      }
      else
      {

         int iButtonSize = 200;

         int iExtraW = 180;

         int iCalcW = iW - iExtraW;

         int iOffsetY = 35;

         if (m_iIndex <= 1)
         {


            if(m_pbuttonBrowse)
               m_pbuttonBrowse->SetWindowPos(ZORDER_TOP, x, m_rect.bottom + iOffsetY, iButtonSize, 50, SWP_SHOWWINDOW);

            if (m_peditBrowse)
               m_peditBrowse->SetWindowPos(ZORDER_TOP, x + iButtonSize + 10, m_rect.bottom + iOffsetY, iCalcW - iButtonSize - 10, 50, SWP_SHOWWINDOW);


         }
         else
         {

            if (m_peditBrowse)
               m_peditBrowse->SetWindowPos(ZORDER_TOP, iExtraW + x, m_rect.bottom + iOffsetY, iCalcW - iButtonSize - 10, 50, SWP_SHOWWINDOW);

            if (m_pbuttonBrowse)
               m_pbuttonBrowse->SetWindowPos(ZORDER_TOP, iExtraW + x + iCalcW - iButtonSize, m_rect.bottom + iOffsetY, iButtonSize, 50, SWP_SHOWWINDOW);


         }


         m_rectMessage.top = y3;
         m_rectMessage.bottom = y3 + iGaugeW / 3;
         if (m_iIndex <= 1)
         {
            m_rectMessage.left = x2;
            m_rectMessage.right = x2 + iGaugeW;
         }
         else
         {
            m_rectMessage.left = x;
            m_rectMessage.right = x + iGaugeW;
         }


      }


   }


   void group::on_mouse_move()
   {

      if (m_pflight == NULL)
      {

         return;

      }

      if (m_pflight->m_eview != impact_flight_viewer)
      {

         m_dX = -1.0;

         return;

      }

      //if (m_pflight->m_bStart)
      //{

      //   m_dX = -1.0;

      //   return;

      //}

      if (m_pflight->m_iX >= m_rect.left + m_sizeBorder.cx && m_pflight.m_iX <= m_rect.right - m_sizeBorder.cx)
      {

         m_dX = (double)(m_pflight->m_iX - (m_rect.left + m_sizeBorder.cx)) / (double) (m_rect.width() - m_sizeBorder.cx * 2);

      }
      else
      {

         m_dX = -1.0;

      }


   }


} // namespace gauge_prober




