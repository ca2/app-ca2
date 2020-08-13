#pragma once

void show_main_dialog();

namespace nativeui
{


   class CLASS_DECL_APP_CA2_NATIVEUI application :
      virtual public ::aura::application
   {
   public:


      application();
      virtual ~application();

      virtual bool init_instance() override;
      virtual void term_instance() override;

      virtual void on_request(::create * pcreate) override;

      virtual i64 add_ref() override;
      virtual i64 dec_ref() override;



   };


} // namespace nativeui




