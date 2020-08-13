#include "framework.h"


main_interface::main_interface()
{

   m_bError = false;
   m_bDetecting = false;
   m_bUpdating = false;
   m_bCancel = false;
   m_bInternetConnected = false;
   m_bMultiPort = false;

}


int main_interface::get_max_port_count()
{

   if(m_bMultiPort)
   {

      return 4;

   }
   else
   {

      return 1;

   }

}


void main_interface::get_firmware_version_label(char * pszChar, int iSize)
{
   
}
