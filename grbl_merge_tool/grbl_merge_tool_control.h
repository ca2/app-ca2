#pragma once



namespace grbl_merge_tool
{


   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL control :
      virtual public ::object
   {
   public:


      array < command > *                    m_pcommandaFile;
      array < command >                      m_commanda;

      bool                                   m_bSend;
      int                                    m_iFileCommandIndex;
      int                                    m_iFileProcessedCommandIndex;

      ::mutex                                  m_mutexFree;
      manual_reset_event                     m_evFree;

      int                                    m_iMaxBufferSize;
      bool                                   m_bResetting;
      bool                                   m_bResetCompleted;
      stringa                                m_straStatus;
      __pointer(::thread)                           m_pthreadStateQuery;
      bool                                   m_bStatusReceived;
      int                                    m_iLastGrblStatus;
      bool                                   m_bUpdateParserStatus;
      __pointer(::serial::Serial)                   m_pserial;
      __pointer(::thread)                           m_pthreadRead;
      __pointer(::thread)                           m_pthreadStatus;
      ::mutex                                  m_mutexSerial;



      control(::aura::application * papp);
      virtual ~control();

      virtual bool open_serial_port(string strPort, int iBaud);

      virtual bool start_sending_file(array < command > * pcommanda);

      virtual bool stop_sending_file();

      virtual bool is_sending_file();

      virtual void assert_valid() const;

      virtual void dump(dump_context & dumpcontext) const;

      virtual bool send_command(string strCommand, int iIndex);

      virtual void send_next_file_commands();

      virtual void read();
      virtual void updateControlsState();
      int bufferLength();

      void grblReset();
   };


} // namespace grbl_merge_tool













