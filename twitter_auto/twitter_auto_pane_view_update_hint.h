#pragma once


namespace twitter_auto
{


   enum EPaneView
   {
      impact_none,
      PaneViewTwitterAutomator,
   };


   class CLASS_DECL_APP_CORE_TWITTER_AUTO ::impact_update :
      public object
   {
   public:


      enum e_type
      {

         TypeNone,
         TypeGetView,
         TypeSetView,
         TypeOnShowKaraoke,
         TypeOnShowView,

      };

      EPaneView m_eview;
      e_type m_etype;


      ::impact_update();
      virtual ~::impact_update();


      void set_type(e_type e_type);

      bool is_type_of(e_type e_type);


   };


} // namespace twitter_auto















