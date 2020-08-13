#include "framework.h"

#include <X11/X.h>
extern "C"
{
#include <X11/Xos.h>
#include <X11/Xfuncs.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/extensions/dpms.h>
//sudo apt-get install libxss-dev
#include <X11/extensions/scrnsaver.h>
}
#include "aura/aura/os/linux/linux_xdisplay.h"
Display * x11_get_display();

namespace image_display
{


   window ::window(::aura::application * papp) :
   ::object(papp)
   {

      m_pt.x = 0;
      m_pt.y = 0;
      m_cx =-1;
      m_cy = -1;

   }


   window ::~window()
   {

   }


   void window::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      rect r;

      GetClientRect(r);

      pgraphics->fill_solid_rect(r, ARGB(255, 0, 0, 0));

      if(m_dib.is_set() && m_dib->area() > 0)
      {

         rect rDst;

         rDst.top_left() = m_pt;

         if(m_cx <= 0)
         {

            rDst.right = r.right;

         }
         else
         {

            rDst.right = rDst.left + m_cx;

         }

         if(m_cy <= 0)
         {

            rDst.bottom = rDst.top + m_dib->m_size.cy * rDst.width() / m_dib.m_size.cx;

         }
         else
         {

            rDst.bottom = rDst.top + m_cy;

         }

         pgraphics->StretchBlt(rDst, m_dib->g(), rect(m_dib->m_size));

      }

   }


} // namespace image_display



#ifdef LINUX

void screen_saver_suspend(bool bSuspend)
{

   xdisplay d(x11_get_display());

   XScreenSaverSuspend(d, bSuspend ? 1 : 0);
   
   int dummy;

   if(!DPMSQueryExtension(d, &dummy, &dummy))
   {
      
      fprintf(stderr, "server does not have extension for DPMS option\n");

   }
   else if(bSuspend)
   {

		DPMSDisable(d);
      
   }
   else
   {

		DPMSEnable(d);
	}

}


#endif
