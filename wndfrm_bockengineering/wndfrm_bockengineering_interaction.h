#pragma once


namespace user
{


   namespace wndfrm
   {


      namespace wndfrm_bockengineering
      {


         class CLASS_DECL_APP_CORE_WNDFRM_BOCKENGINEERING interaction:
            public ::user::wndfrm::interaction
         {
         public:


            


            interaction(::aura::application * papp);
            virtual ~interaction();

            virtual void update();


            virtual void get_frame_schema_list(stringa & stra);


            virtual __pointer(::user::wndfrm::frame::frame) get_frame_schema(const char * pszSchema);


         };


      } // namespace wndfrm_bockengineering


   } // namespace wndfrm


} // namespace user





