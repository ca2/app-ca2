#pragma once


namespace gauge_prober
{


   enum e_impact
   {
      impact_none,
      impact_flight_viewer,
      impact_flight_simulator,
      impact_control_terminal,
      impact_info,
   };


   class CLASS_DECL_APP_CA2_GAUGE_PROBER ::impact_update :
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

      e_impact m_eview;
      e_type m_etype;


      ::impact_update();
      virtual ~::impact_update();


      void set_type(e_type e_type);

      bool is_type_of(e_type e_type);


   };


} // namespace gauge_prober















