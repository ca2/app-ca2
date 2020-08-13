#pragma once


namespace imagecopy2
{

   class main_view;


   class CLASS_DECL_APP_CA2_IMAGECOPY2 main_form_list :
      virtual public ::simple_form_list_view
   {
      public:

         class image_info :
            virtual public ::object
         {
            public:



               ::file::path      m_path;
               ::datetime::time  m_time;

               image_info() {}
               virtual ~image_info() {}

         };

         //enum e_hint
         //{

         //   update_load,
         //   update_save,

         //};
         bool                       m_bChangedFilter;
         index                      m_iProcess;
         index                      m_iProcessTotal;
         bool                       m_bThumbnails;
         bool                       m_bThumbnailsPrevious;

         string                     m_strStartDateTime;
         string                     m_strEndDateTime;
         string                     m_strInterval;
         string                     m_strFolder;


         ::datetime::time           m_timeStart;
         ::datetime::time           m_timeEnd;
         index                      m_iInterval;
         ::file::path               m_pathFolder;


         ::image_list            m_i;
         __pointer(visual::icon)        m_piconUndefined;
         __pointer(visual::icon)        m_piconRunning;
         __pointer(visual::icon)        m_piconError;
         __pointer(visual::icon)        m_piconFullFail;
         __pointer(task_dialog)         m_pdialog;
         ::draw2d::font_pointer       m_fontUsername;

         ::file::listing         m_ls;

         ::file::patha           m_pathaOk;
         ::mutex                   m_mutexOk;

         ::mutex                   m_mutexParameters;
         __pointera(image_info)         m_imageinfoa;
         //array < ::datetime::time >    m_datetimea;
         //string_map < int > m_mapImage;

         ::manual_reset_event    m_evChange;
         ::file::path            m_pathSource;
         ::file::path            m_pathCopy;
         main_view *             m_pmainview;

         __pointer(::thread)            m_pthreadThumb;

         main_form_list(::aura::application * papp);
         virtual ~main_form_list();

         //::datetime::time get_image_time(string str);
         bool touch_file(string str, __pointer(image_info)& time);

         virtual bool is_item_ok(index iItem);
         virtual void update_ok_list();
         virtual bool save_parameters(bool bChangedFilter = true, bool bChangedThumbnail = true);
         virtual bool parse_parameters();

         virtual void on_layout();

         void ensure_image_visible();

         virtual bool get_color(COLORREF & cr, ::user::e_color ecolor, ::user::interaction * pinteraction) override;

         virtual void _001OnShowControl(__pointer(control) pcontrol);

         virtual void _001InsertColumns() override;

         void install_message_routing(::channel * pchannel);

         void on_update(::user::impact * pSender, LPARAM lHint, ::object * pupdate);

         virtual void _001OnAfterChangeText(const ::action_context & action_context);

         virtual void _001OnTimer(timer * ptimer);


         virtual void open_folder(::file::path pathFolder);


         void _001OnInitializeForm(__pointer(control) pcontrol);
         DECL_GEN_SIGNAL(_001OnCreate);



         DECL_GEN_SIGNAL(_001OnOpenFolder);
         DECL_GEN_SIGNAL(_001OnUpdateOpenFolder);
         DECL_GEN_SIGNAL(_001OnFileCopy);
         DECL_GEN_SIGNAL(_001OnUpdateFileCopy);
         DECL_GEN_SIGNAL(_001OnParameters);
         DECL_GEN_SIGNAL(_001OnUpdateParameters);
         DECL_GEN_SIGNAL(_001OnStopAllTasks);
         DECL_GEN_SIGNAL(_001OnUpdateStopAllTasks);

         DECL_GEN_SIGNAL(_001OnContextMenu);
         DECL_GEN_SIGNAL(_001OnAddTask);
         DECL_GEN_SIGNAL(_001OnUpdateAddTask);
         DECL_GEN_SIGNAL(_001OnDeleteTask);
         DECL_GEN_SIGNAL(_001OnUpdateDeleteTask);
         DECL_GEN_SIGNAL(_001OnUpdateEditUndo);
         DECL_GEN_SIGNAL(_001OnUpdateEditRedo);
         DECL_GEN_SIGNAL(_001OnUpdateMediaTransfer);

         DECL_GEN_SIGNAL(_001OnEditUndo);
         DECL_GEN_SIGNAL(_001OnEditRedo);
         DECL_GEN_SIGNAL(_001OnMediaTransfer);



         virtual bool keyboard_focus_OnSetFocus() override;

         document * get_document();

         virtual count _001GetItemCount();

         virtual void _001GetItemImage(::user::mesh_item * pitem) override;

         virtual void _001GetItemColor(::user::mesh_item * pitem) override;

         virtual void _001GetItemText(::user::mesh_item * pitem) override;

         virtual void _001SetItemText(::user::mesh_item * pitem) override;

         virtual void _001DrawSubItem(::user::draw_list_item * pdrawitem) override;

         virtual bool on_click(const ::user::item & item) override;

         virtual i32 _001CalcItemHeight(int iBaseHeight) override;

         i32 _001GetDrawTextFlags(::user::list::EView eview);

         virtual void _001DrawItem(::user::draw_list_item * pdrawitem);

         virtual void defer_create_thumbnails();


   };


} // namespace formlist




