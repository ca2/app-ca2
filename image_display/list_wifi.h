#pragma once


class wifi_internal;


class wifi :
    virtual public object
{
public:


   wifi_internal *      m_pinternal;
   stringa              m_straSSID;
   string_map < int >   m_mapStrength;
   string               m_strSSID;
   string               m_strPass;
   ::thread *           m_pthreadScan;
   bool                 m_bScan;


   wifi(::aura::application * papp);
   virtual ~wifi();

   virtual void on_wifi_update(stringa straDel, stringa straNew);

   // returns true if attempting to connect
   virtual bool connect_wifi(bool bTry);

   virtual void wifi_activating();

   virtual void wifi_activated();

   virtual void wifi_on_activate();

   virtual void wifi_on_activation_error();


};
