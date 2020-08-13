#include "framework.h"


namespace gauge_prober
{


   serial::serial(::aura::application * papp, string strCom, group * pgroup) :
      ::object(papp)
   {

      m_pthread = NULL;
      m_iBaud = 2400;

      m_strCom = strCom;

      m_pgroup = pgroup;

      defer_create_mutex();

   }


   serial::~serial()
   {


   }

   void serial::start()
   {

      synch_lock sl(mutex());

      if (m_pthread != NULL)
      {

         return;

      }

      m_strError.Empty();

      m_pthread = fork([=]()
      {

         serial_run();

         m_pthread = NULL;

      });



   }


   void serial::serial_run()
   {

      __pointer(::serial::Serial) pserial;

      try
      {

         {

            synch_lock sl(&Application.m_mutexSerial);

            pserial = Application.m_mapSerial[m_strCom];

            if (pserial.is_null())
            {

               pserial = canew(::serial::Serial(get_context_application(), "", m_iBaud, ::serial::Timeout(0, 0, 0, 100, 6), ::serial::eightbits,
                                                ::serial::parity_none, ::serial::stopbits_one, ::serial::flowcontrol_none));

               Application.m_mapSerial[m_strCom] = pserial;

               pserial->setPort(m_strCom);

            }


         }


         pserial->open();

         while (::thread_get_run())
         {

            bool bOk = false;

            byte sz[6];

            sz[0] = 255;

            if (m_pgroup->m_pflight->m_eview == impact_control_terminal)
            {

               sz[1] = 0;
               sz[2] = m_pgroup->m_gaugeRpm.m_b;
               sz[3] = m_pgroup->m_gaugeEgt.m_b;
               sz[4] = m_pgroup->m_gaugeThrottle.m_b;
               sz[5] = m_pgroup->m_gaugeVbatt.m_b;

               bOk = true;

            }
            else if (m_pgroup->m_iTime >= 0 && m_pgroup->m_iTime < m_pgroup->m_gaugeEgt.m_pointda.get_size())
            {

               pointd_array * pegt = NULL;
               pointd_array * prpm = NULL;
               pointd_array * pvbatt = NULL;
               pointd_array * pthrottle = NULL;


               prpm = &m_pgroup->m_gaugeRpm.m_pointda;
               pegt = &m_pgroup->m_gaugeEgt.m_pointda;
               pthrottle = &m_pgroup->m_gaugeThrottle.m_pointda;
               pvbatt = &m_pgroup->m_gaugeVbatt.m_pointda;

               int iByte = m_pgroup->m_mem[m_pgroup->m_iTime / 100 * 6 + 269 - 1];
               sz[1] = (byte)iByte;
               sz[2] = (byte)(prpm->get_data()[m_pgroup->m_iTime].y / 500.0);
               sz[3] = (byte)((pegt->get_data()[m_pgroup->m_iTime].y + 50.0) / 4.6);
               sz[4] = (byte)(pthrottle->get_data()[m_pgroup->m_iTime].y *2.0);
               sz[5] = (byte)((pvbatt->get_data()[m_pgroup->m_iTime].y - 7.0) * 255.0 / 9.03);

               bOk = true;

            }

            if (bOk)
            {

               pserial->write(sz, 6);

            }

            pserial->waitByteTimes(24);

         }



      }
      catch (::serial::IOException * e)
      {

         esp671 esp(e);

         m_strError = e->what();

         if (m_strError.has_char() && m_strError.find_ci("does not exist") < 0)
         {

            m_strError += " The COM port seems to be already opened.";

         }

         pserial->close();

         TRACE("Error serial_run(%s) %s", m_strCom, m_strError);

         return;

      }
      catch (...)
      {


      }

      TRACE("Finished serial_run(%s)", m_strCom);

      pserial->close();

   }

   void serial::stop()
   {

      synch_lock sl(mutex());

      if (m_pthread != NULL)
      {

         ::multithreading::set_finish(m_pthread);

      }

   }



} // namespace gauge_prober




