#include "core/core/core/_.h"

#undef CoreApplication

[MTAThread]
int main(Array<String^>^)
{

   return app_core_main("m_app.exe : app=app-core/hellomultiverse wndfrm=rootkiller client_only");

}


#include "include/applink.cpp"


/*[MTAThread]
int main(Array<String^>^)
{
   
   // No Depends
   
   return 0;

}
*/