#pragma once


namespace twitter_automator
{


   class domain_find_replace
   {
   public:

      string      m_strDomain;
      string      m_strFind;
      string      m_strReplace;

      bool apply(string & strLink);

   };

   class domain_find_replace_array :
      virtual public ::array < domain_find_replace >
   {
   public:

      bool apply(string & strLink);

   };

   class prefix_before_after
   {
   public:

      string      m_strPrefix;
      string      m_strBefore;
      string      m_strAfter;

      bool apply(string & strLink);

   };

   class prefix_before_after_array :
      virtual public ::array < prefix_before_after >
   {
   public:

      bool apply(string & strLink);

   };


   class prefix_stop_before_after
   {
   public:

      string      m_strPrefix;
      string      m_strStopChars;
      string      m_strBefore;
      string      m_strAfter;

      bool apply(string & strLink);

   };

   class prefix_stop_before_after_array :
      virtual public ::array < prefix_stop_before_after >
   {
   public:

      bool apply(string & strLink);

   };


   class expand_link
   {
   public:

      DWORD       m_dwStart;
      string      m_strExpanded;

      expand_link()
      {
         m_dwStart = 0;
      }

   };

   class cache_path
   {
   public:

      DWORD       m_dwStart;
      string      m_strTime;

      cache_path()
      {
         m_dwStart = 0;
      }

   };


} // namespace twitter_automator


