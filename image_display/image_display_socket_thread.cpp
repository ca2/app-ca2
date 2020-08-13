#include "framework.h"



namespace image_display
{


   socket_thread::socket_thread(::aura::application * papp) :
      ::object(papp),
      thread(papp),
      ::netserver::socket_thread(papp)
   {


   }


   socket_thread::~socket_thread()
   {

   }


   ::sockets::listen_socket_base * socket_thread::create_listen_socket()
   {

      return new ::sockets::listen_socket < socket > (*m_psockethandler);

   }


} // namespace netnode



