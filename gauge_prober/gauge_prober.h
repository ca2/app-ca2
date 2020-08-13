#pragma once


#include "core/core/core/_.h"
#include "core/user/user/user.h"
#include "core/user/html/html/html.h"
#include "core/user/userex/userex.h"
#include "core/filesystem/filemanager/filemanager.h"
#include "core/user/simple/simple.h"


#ifdef _APP_CA2_GAUGE_PROBER_LIBRARY
#define CLASS_DECL_APP_CA2_GAUGE_PROBER  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CA2_GAUGE_PROBER  CLASS_DECL_IMPORT
#endif

#include "exif.h"
namespace user
{

   typedef ::user::show < ::user::button > button_view;

} // namespace user


namespace gauge_prober
{

   class application;
   class document;
   class task_array;
   class flight_viewer;
   class group;
   class line_chart;
   class flight;

   enum e_engine : unsigned char
   {
      engine_none = 0,
      engine_MERCURY = 1,
      engine_PEGASUS = 2,
      engine_OLYMPUS = 3,
      engine_TITAN = 4,
      engine_NIKE = 5,
      engine_LYNX = 6

   };


   struct CLASS_DECL_APP_CA2_GAUGE_PROBER E
   {
      // three-bit unsigned field,
      // allowed values are 0...7
      byte bL : 3;
      e_engine engine : 5;
   };

   struct CLASS_DECL_APP_CA2_GAUGE_PROBER S
   {
      // three-bit unsigned field,
      // allowed values are 0...7
      byte bL : 3;
      union
      {
         byte bH : 5;
         e_engine engine;
      };
   };

   struct CLASS_DECL_APP_CA2_GAUGE_PROBER D1
   {
      union
      {
         byte  data1;
         E     e;
      };
      byte data[4];
   };

   CLASS_DECL_APP_CA2_GAUGE_PROBER string engine_name(e_engine e);

   enum e_impact
   {

      impact_flight_viewer,
      impact_flight_simulator,
      impact_control_terminal


   };


} // namespace flag


#define APP_TYPE ::gauge_prober::application

#include "gauge_prober_serial.h"

#include "gauge_prober_button.h"
#include "gauge_prober_edit_box.h"

#include "gauge_prober_line_chart.h"
#include "gauge_prober_gauge.h"
#include "gauge_prober_throttle.h"
#include "gauge_prober_group.h"


#include "gauge_prober_info.h"


#include "gauge_prober_flight.h"
#include "gauge_prober_flight_simulator.h"
#include "gauge_prober_flight_viewer.h"
#include "gauge_prober_control_terminal.h"

#include "gauge_prober_document.h"
#include "gauge_prober_frame.h"
#include "gauge_prober_main_frame.h"

#include "gauge_prober_pane_view.h"

#include "gauge_prober_application.h"

