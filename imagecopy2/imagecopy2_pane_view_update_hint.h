#pragma once


namespace imagecopy2
{


   enum EPaneView
   {
      impact_none,
      PaneViewImageCopy,
   };


   class CLASS_DECL_APP_CA2_IMAGECOPY2 ::impact_update :
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


} // namespace imagecopy2















