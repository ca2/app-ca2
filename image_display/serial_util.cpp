#include "framework.h"
#include "serial_util.h"


void serial_putchar(struct serial_serial * pserial, int c)
{

   ::serial::Serial * ps = (::serial::Serial *) pserial->m_popaque;

   u8 ui = c;

   ps->write(&ui, sizeof(ui));

}


int  serial_getchar(struct serial_serial * pserial, int timeout)
{

   ::serial::Serial * ps = (::serial::Serial *) pserial->m_popaque;

   u8 ui = 0;

   if (timeout < 0)
   {

      while (true)
      {

         if (ps->read(&ui, sizeof(ui)) >= 1)
         {

            return ui;

         }

      }

   }
   else
   {

      do
      {

         int iRes = (int)ps->read(&ui, sizeof(ui));

         if (iRes >= 1)
         {

            return ui;

         }

         timeout--;

      }
      while (timeout > 0);

   }

   return -1;

}


int  serial_read(struct serial_serial * pserial)
{

   //::serial::Serial * ps = (::serial::Serial *) pserial->m_popaque;

   int iChar = serial_getchar(pserial, 2);

   if (iChar > 0)
   {

      simple_log_no_new_line(string((char)iChar));

   }

   return iChar;

}

void serial_ssleep(unsigned long iSeconds)
{

   ::Sleep((u32) (iSeconds * 1000));
}

