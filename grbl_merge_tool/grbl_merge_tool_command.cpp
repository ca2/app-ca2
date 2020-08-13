#include "framework.h"


namespace grbl_merge_tool
{


   command::command()
   {

      m_iIndex = -1;
      m_estate = state_queue;
      m_iLength = -1;


   }


   command::command(const command & command)
   {

      operator =(command);

   }

   command & command::operator = (const command & command)
   {

      if (this != &command)
      {

         m_iIndex = command.m_iIndex;
         m_strCommand = command.m_strCommand;
         m_estate = command.m_estate;
         m_strResponse = command.m_strResponse;
         m_iLength = command.m_iLength;

      }

      return *this;

   }


   string command::get_state_string()
   {

      switch (m_estate)
      {
      case state_queue:
         return "In queue";
      case state_sent:
         return "Sent";
      case state_processed:
         return "Processed";
      case state_skipped:
         return "Skipped";
      default:
         return "";
      };

   }

   strsize command::length()
   {

      return m_strCommand.length();

   }

} // namespace grbl_merge_tool



