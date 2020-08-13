#include "framework.h"


namespace user
{


   namespace wndfrm
   {


      namespace wndfrm_bockengineering
      {


         interaction::interaction(::aura::application * papp) :
            object(papp),
            ::user::wndfrm::interaction(papp)
         {
            ::fork(get_context_application(), [&]()
            {

               System.visual().set_cursor_set_system_default();

            });

         }


         interaction::~interaction()
         {
         }


         void interaction::update()
         {
         }

         void interaction::get_frame_schema_list(stringa & stra)
         {

            stra.add("001");
            stra.add("002");
            stra.add("005");
            stra.add("008");
            stra.add("011");
            stra.add("013");
            stra.add("Minimal001");

         }


         __pointer(::user::wndfrm::frame::frame) interaction::get_frame_schema(const char * pszSchema)
         {

            string strSchema(pszSchema);

            if(strSchema == "001")
            {
               return  canew(::user::wndfrm::frame::wndfrm_bockengineering::FrameSchemaHardCoded001(get_context_application()));
            }
            else if(strSchema == "002")
            {
               return  canew(::user::wndfrm::frame::wndfrm_bockengineering::FrameSchemaHardCoded002(get_context_application()));
            }
            else if(strSchema == "005")
            {
               return  canew(::user::wndfrm::frame::wndfrm_bockengineering::FrameSchemaHardCoded005(get_context_application()));
            }
            else if(strSchema == "008")
            {
               return  canew(::user::wndfrm::frame::wndfrm_bockengineering::FrameSchemaHardCoded008(get_context_application()));
            }
            else if(strSchema == "011")
            {
               return  canew(::user::wndfrm::frame::wndfrm_bockengineering::FrameSchemaHardCoded011(get_context_application()));
            }
            else if(strSchema == "013")
            {
               return  canew(::user::wndfrm::frame::wndfrm_bockengineering::FrameSchemaHardCoded013(get_context_application()));
            }
            else if(strSchema == "Minimal001")
            {
               return  canew( ::user::wndfrm::frame::wndfrm_bockengineering::FrameSchemaHardCodedMinimal001(get_context_application()));
            }

            return NULL;

         }


      } // namespace wndfrm


   } // namespace wndfrm


} // namespace user




