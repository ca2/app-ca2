#pragma once



namespace grbl_merge_tool
{


   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL application :
      virtual public ::core::application,
      virtual public ::double_scalar_source
   {
   public:


      string                                 m_strCode;

      __pointer(control)                            m_pcontrol;

      ::user::single_document_template *     m_ptemplateGrblMergeToolTop;
      ::user::single_document_template *     m_ptemplateGrblMergeToolMain;

      string                                 m_strStatus;


      document *                             m_pdoc;
      ::user::plain_edit *                   m_pedit;
      ::user::plain_edit *                   m_pedit1;
      ::user::plain_edit *                   m_pedit2;
      main_form_list *                       m_plist;
      pane_view *                            m_ppaneview;
      top_pane_view *                        m_ptoppaneview;
      control_view *                         m_pcontrolview;
      ::html_form *                          m_pmainview;

      e_mode                                 m_emode;

      array < ::serial::PortInfo >           m_portinfoa;


      string                                 m_straCoord[19];

      array < command >                      m_commanda;
      bool                                   m_bInvalidCharsCount;

      ::file::path                           m_filepath;
      double_scalar                          m_scalar;

      application();
      virtual ~application();

      virtual bool open_serial_port(string strPort = "", int iBaud = -1);
      virtual void list_serial_ports();

      virtual bool init_instance();
      virtual void term_instance() override;

      void update_all_views(LONG lHint);
      virtual void on_request(::create * pcreatecontext);

      virtual i64 add_ref();
      virtual i64 dec_ref();

      virtual string preferred_experience();

      virtual void set_mode(e_mode emode);

      bool on_new_document();
      bool on_open_document(const var & varFile) override;

      virtual void on_start();
      virtual bool update_commands(bool bInteractive = true);
      virtual bool insert(string strFile, bool bCharCount = false);
      virtual void on_transfer_success();

      DECL_GEN_SIGNAL(_001OnFileStart);
      DECL_GEN_SIGNAL(_001OnUpdateFileStart);
      DECL_GEN_SIGNAL(_001OnFileStop);
      DECL_GEN_SIGNAL(_001OnUpdateFileStop);
      DECL_GEN_SIGNAL(_001OnFileReset);
      DECL_GEN_SIGNAL(_001OnUpdateFileReset);
      DECL_GEN_SIGNAL(_001OnViewMain);
      DECL_GEN_SIGNAL(_001OnFileOpenFile);
      DECL_GEN_SIGNAL(_001OnUpdateFileOpenFile);
      DECL_GEN_SIGNAL(_001OnFileOpenLibrary);
      DECL_GEN_SIGNAL(_001OnUpdateFileOpenLibrary);
      DECL_GEN_SIGNAL(_001OnToolsOptions);
      DECL_GEN_SIGNAL(_001OnFileBarcode);

      virtual void get_scalar_minimum(e_scalar escalar, double & d);
      virtual void get_scalar(e_scalar escalar, double & d);
      virtual void get_scalar_maximum(e_scalar escalar, double & d);
   };


} // namespace grbl_merge_tool



