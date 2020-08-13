#pragma once


namespace gauge_prober
{


   class CLASS_DECL_APP_CA2_GAUGE_PROBER serial :
      virtual public object
   {
   public:


      ::thread *              m_pthread;
      string                  m_strCom;
      int                     m_iBaud;
      group *                 m_pgroup;
      string                  m_strError;

      serial(::aura::application * papp, string strCom, group * pgroup);
      virtual ~serial();


      virtual void start();

      virtual void serial_run();

      virtual void stop();


   };


} // namespace gauge_prober



