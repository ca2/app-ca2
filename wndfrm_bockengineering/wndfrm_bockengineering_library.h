#pragma once


namespace user
{


   namespace wndfrm
   {

      namespace wndfrm_bockengineering
      {


         class CLASS_DECL_APP_CORE_WNDFRM_BOCKENGINEERING library:
            virtual public ::aura::library
         {
         public:


            library(::aura::application * papp);
            virtual ~library();


//            virtual bool is_uinteraction_library() override;
            virtual __pointer(::object) create_object(::aura::application * papp,const char * pszClass, object * p) override;


         };


      } // namespace wndfrm_bockengineering


   } // namespace wndfrm


} // namespace user






