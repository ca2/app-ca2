#pragma once


namespace grbl_merge_tool
{


   class CLASS_DECL_APP_CORE_GRBL_MERGE_TOOL command
   {
   public:

      enum e_state
      {

         state_queue,
         state_sent,
         state_processed,
         state_skipped,

      };

      int         m_iIndex;
      string      m_strCommand;
      e_state     m_estate;
      string      m_strResponse;
      int         m_iLength;

      command();
      command(const command & command);

      command & operator = (const command & command);


      string get_state_string();
      strsize length();

   };


} // namespace grbl_merge_tool



