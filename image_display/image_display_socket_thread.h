#pragma once



namespace image_display
{



   class CLASS_DECL_AURA socket_thread :
      virtual public ::netserver::socket_thread
   {
   public:





      socket_thread(::aura::application * papp);
      ~socket_thread();


      virtual ::sockets::listen_socket_base * create_listen_socket() override;


   };


} // namespace netserver




