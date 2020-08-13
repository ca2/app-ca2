#include "framework.h"


namespace imagecopy2
{


   info_view::info_view(::aura::application * papp) :
      object(papp)
   {


   }


   info_view::~info_view()
   {

   }


   void info_view::assert_valid() const
   {

      ::user::impact::assert_valid();

   }


   void info_view::dump(dump_context & dumpcontext) const
   {

      ::user::impact::dump(dumpcontext);

   }






   void info_view::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::user::impact::_001OnDraw(pgraphics);

      select_font(pgraphics, ::user::font_static);
      pgraphics->set_text_color(ARGB(255, 0, 0, 0));
      {
         synch_lock sl(m_pmainview->m_plist->mutex());
         string str1;
         str1.Format("%d Files", m_pmainview->m_plist->m_ls.get_count());
         pgraphics->text_out(20, 20, str1);
      }
      {
         synch_lock sl(&m_pmainview->m_plist->m_mutexOk);
         string str2;
         str2.Format("%d Files Selected", m_pmainview->m_plist->m_pathaOk.get_count());
         pgraphics->text_out(20, 40, str2);

      }

      int y = 80;
      {
         synch_lock sl(m_pmainview->m_plist->mutex());
         if (m_pmainview->m_plist->m_pathCopy.has_char())
         {
            string str1;
            str1.Format("Copying file %s", m_pmainview->m_plist->m_pathCopy.name());
            pgraphics->text_out(20, 80, str1);
            y += 20;
         }
      }
      {
         synch_lock sl(m_pmainview->m_plist->mutex());
         if (m_pmainview->m_plist->m_iProcessTotal == 0)
         {
            string str1;
            str1 = "Processing files " + ::str::block('.', (get_tick_count() / 500) % 4);
            pgraphics->text_out(20, 80, str1);
            y += 20;
         }
         else if (m_pmainview->m_plist->m_iProcessTotal > 0 && m_pmainview->m_plist->m_iProcess < m_pmainview->m_plist->m_iProcessTotal)
         {
            string str1;
            str1 = "Processing file " + ::str::from(m_pmainview->m_plist->m_iProcess +1) + " of "+::str::from(m_pmainview->m_plist->m_iProcessTotal) +" " +::str::block('.', (get_tick_count() / 500) % 4);
            pgraphics->text_out(20, 80, str1);
            y += 20;
         }
      }


   }





} // namespace imagecopy2



