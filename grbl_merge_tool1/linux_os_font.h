#pragma once



namespace os
{

   class font :
      virtual public object
   {
      public:



#ifdef WINDOWS
         HFONT m_hfont;
#else
         PangoFontDescription *  m_pdescription;
#endif

         font();
         virtual ~font();

         bool create_point_font(int iPoint, const char * pszFontFamily, int iWeight = FW_NORMAL);
         void destroy();

#ifdef WINDOWS
         HFONT get_os_data() { return m_hfont; }
#endif

   };



} // namespace os


