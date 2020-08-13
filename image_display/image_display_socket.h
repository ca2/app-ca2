#pragma once


namespace image_display
{


   class CLASS_DECL_AURA socket :
      virtual public ::netserver::socket
   {
   public:


      bool                             m_bSetCookie;

      socket(::sockets::base_socket_handler & h);
      ~socket();

//      virtual void OnExecute();
//      virtual void OnResponseComplete();
//
//      virtual void send_response();

      virtual void on_send_response();



   };


} // namespace image_display

