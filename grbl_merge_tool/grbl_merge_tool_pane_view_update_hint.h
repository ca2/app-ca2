#pragma once


namespace grbl_merge_tool
{


   enum e_pane
   {

      pane_none,
      pane_main,

   };


   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL ::impact_update :
      public object
   {
   public:


      enum e_type
      {

         type_none,

      };

      e_pane   m_epane;
      e_type   m_etype;


      ::impact_update();
      virtual ~::impact_update();


      void set_type(e_type e_type);

      bool is_type_of(e_type e_type);


   };


} // namespace grbl_merge_tool















