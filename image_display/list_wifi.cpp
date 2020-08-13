#include "framework.h"
#include "list_wifi.h"
#include <glib.h>
#include <dbus/dbus-glib.h>
#include <stdio.h>


#include <NetworkManager.h>
#include <nm-utils.h>


class wifi_internal :
   virtual public object
{
public:


   stringa                 m_straSSID;
   wifi *                  m_pwifi;
   GCancellable *          m_pcancellable;
   GCancellable *          m_pcancellableActivating;
   NMClient *              m_pclient;
   NMDevice *              m_pdevice;
   DBusGConnection *       m_bus;
   GError *                m_perror;
   NMActiveConnection *    m_pactive;

   ::thread *              m_pthreadActivating;


   wifi_internal(wifi * pwifi);
   virtual ~wifi_internal();




   virtual void on_wifi_update();

   void show_wifi_device_info (NMDevice *device);
   string get_access_point_security_info (NMAccessPoint *ap);
   char * ap_wpa_rsn_flags_to_string (guint32 flags);
   NMAccessPoint * get_access_point(string str, NMDevice *device);

   static void s_on_activate_client(GObject *source_object, GAsyncResult *res, gpointer user_data);

   void on_activate_client(GObject *source_object, GAsyncResult *res);

   static void s_on_new_client(GObject *source_object, GAsyncResult *res, gpointer user_data);

   void on_new_client(GObject *source_object, GAsyncResult *res);

   // returns true if attempting to connect
   bool connect_wifi(bool bTry);


};


wifi_internal::wifi_internal(wifi * pwifi) :
   object(pwifi->get_context_application()),
   m_pwifi(pwifi)
{

   m_pthreadActivating = NULL;

   defer_create_mutex();

   m_perror = NULL;

   /* Get system bus */
	m_bus = dbus_g_bus_get (DBUS_BUS_SYSTEM, NULL);

	m_pcancellableActivating = NULL;

	/* Get NMClient object */
	m_pcancellable = g_cancellable_new ();

   nm_client_new_async (m_pcancellable, &s_on_new_client, (gpointer) this);

}


wifi_internal::~wifi_internal()
{


}


wifi::wifi(::aura::application * papp) :
   object(papp),
   m_pinternal(new wifi_internal(this))
{

   m_bScan = false;
   m_pthreadScan = NULL;

}

wifi::~wifi()
{

   ::aura::del(m_pinternal);

}

void wifi::on_wifi_update(stringa straDel, stringa straNew)
{

}


bool wifi::connect_wifi(bool bTry)
{

   return m_pinternal->connect_wifi(bTry);

}

void wifi::wifi_activating()
{


}

void wifi::wifi_activated()
{


}

void wifi::wifi_on_activate()
{


}


void wifi::wifi_on_activation_error()
{



}




void wifi_internal::s_on_activate_client(GObject *source_object, GAsyncResult *res, gpointer user_data)
{

   wifi_internal * pinternal = (wifi_internal *) user_data;

   pinternal->on_activate_client(source_object, res);

}


void wifi_internal::on_activate_client(GObject *source_object, GAsyncResult *res)
{

   m_perror = NULL;

   m_pactive = nm_client_activate_connection_finish (m_pclient, res, &m_perror);

   if(m_pactive != NULL)
   {

      m_pthreadActivating = get_context_application()->fork([&]()
      {

         NMActiveConnectionState state = NM_ACTIVE_CONNECTION_STATE_UNKNOWN;

         try
         {

            m_pwifi->wifi_activating();

            int i = 10;

            while(thread_get_run() && i > 0)
            {

               state = nm_active_connection_get_state (m_pactive);

               if(state == NM_ACTIVE_CONNECTION_STATE_ACTIVATED)
               {

                  m_pwifi->wifi_activated();

                  //file_put_contents_dup(::dir::home()/".wifissid", m_pwifi->m_strSSID);

                  //file_put_contents_dup(::dir::home()/".wifipass", m_pwifi->m_strPass);

                  string strPass = file_as_string_dup(::dir::home() / ".wifipass");

                  stringa straPass;

                  straPass.add_lines(strPass);

                  straPass._007SetLine(m_pwifi->m_strSSID, m_pwifi->m_strPass);

                  strPass = straPass.implode("\n");

                  file_put_contents_dup(::dir::home() / ".wifipass", strPass);

//::system("sudo systemctl stop wicd");
//::system("sudo systemctl stop wpa_supplicant");
  //                ::system("sudo systemctl restart network-manager");

                  Sleep(5000);

                  m_pwifi->wifi_on_activate();

                  break;

               }

               int j = 10;

               while(j > 0 && thread_get_run())
               {

                  Sleep(100);

                  j--;

               }

               i--;

            }

            if(state != NM_ACTIVE_CONNECTION_STATE_ACTIVATED)
            {

               g_cancellable_cancel(m_pcancellableActivating);

               m_pcancellableActivating = NULL;

               m_pwifi->wifi_on_activation_error();

            }

         }
         catch(...)
         {

         }

         m_pthreadActivating = NULL;

      });

   }

}


bool wifi_internal::connect_wifi(bool bTry)
{

   synch_lock sl(mutex());

   if(m_pcancellableActivating != NULL)
   {

      g_cancellable_cancel(m_pcancellableActivating);

      m_pcancellableActivating = NULL;

   }

   if(m_pthreadActivating != NULL)
   {

      ::multithreading::post_quit_and_wait(m_pthreadActivating, seconds(20));

      m_pthreadActivating = NULL;

   }

   NMAccessPoint * pap = get_access_point(m_pwifi->m_strSSID, m_pdevice);

   const char * path = nm_object_get_path(NM_OBJECT(pap));

   NMConnection * pconnection = NULL;

   string str = get_access_point_security_info(pap);

   str.trim();

   if(str.has_char())
   {

      if(bTry && m_pwifi->m_strPass.is_empty())
      {

         return false;

      }
      else
      {

         pconnection = nm_simple_connection_new();

         NMSetting * psettingwirelesssecurity = nm_setting_wireless_security_new();

         guint32 flags = nm_access_point_get_flags (pap);

         if(str.find_ci("wpa2") >= 0)
         {

            g_object_set (psettingwirelesssecurity, NM_SETTING_WIRELESS_SECURITY_PSK, m_pwifi->m_strPass.c_str(), NULL);

         }
         else if(str.find_ci("wpa") >= 0)
         {

            nm_setting_wireless_security_set_wep_key(NM_SETTING_WIRELESS_SECURITY(psettingwirelesssecurity), 0, m_pwifi->m_strPass);

            g_object_set (G_OBJECT (psettingwirelesssecurity), NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE, NM_WEP_KEY_TYPE_PASSPHRASE, NULL);

         }
         else
         {

            nm_setting_wireless_security_set_wep_key(NM_SETTING_WIRELESS_SECURITY(psettingwirelesssecurity), 0, m_pwifi->m_strPass);

            g_object_set (G_OBJECT (psettingwirelesssecurity), NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE, NM_WEP_KEY_TYPE_KEY, NULL);


         }

         nm_connection_add_setting(pconnection, psettingwirelesssecurity);

      }

   }

   //file_put_contents_dup(::dir::home()/".wifissid", "");

   //file_put_contents_dup(::dir::home()/".wifipass", "");

   m_pcancellableActivating = g_cancellable_new();

   nm_client_add_and_activate_connection_async(m_pclient, pconnection, m_pdevice, path, m_pcancellableActivating,  &s_on_activate_client, (gpointer)this);

//   NMSettingWireless * pwireless = NM_DEVICE_WIFI(m_pdevice);



      /*nm_connection_new(),
       nm_connection_add_setting(), etc) and then calling
        nm_remote_settings_add_connection() and finally
        nm_client_activate_connection() on the returned object.*/
   //g_signal_connect (G_OBJECT (m_buttonConnect.w()), "clicked", G_CALLBACK (screen_download), (gpointer) m_pmain);
   return true;

}


void wifi_internal::s_on_new_client(GObject *source_object, GAsyncResult *res, gpointer user_data)
{

   wifi_internal * pinternal = (wifi_internal *) user_data;

	pinternal->on_new_client(source_object, res);

}
//void 	_nm_device_wifi_set_wireless_enabled (NMDeviceWifi *device, gboolean enabled);

void wifi_internal::on_new_client(GObject *source_object, GAsyncResult *result)
{

   m_pclient = nm_client_new_finish (result, &m_perror);

	if (!m_pclient)
	{

      dbus_g_connection_unref (m_bus);

		g_message ("Error: Could not create NMClient.");

		return;

	}

	GError * perror = NULL;

	nm_client_networking_set_enabled(m_pclient, true, &perror);

	nm_client_wireless_set_enabled(m_pclient, true);

   m_pwifi->m_pthreadScan = get_context_application()->fork([&]()
   {

      int i = 0;

      nm_device_set_autoconnect (m_pdevice, true);

      bool bScan = false;

      while(thread_get_run())
      {

         if(m_pwifi->m_bScan)
         {

            if(!bScan)
            {

               i = 0;

               bScan = true;

            }

            if(i % 15 == 0)
            {

               nm_device_wifi_request_scan (NM_DEVICE_WIFI(m_pdevice), m_pcancellable, &m_perror);

            }

            const GPtrArray *devices;

            int i;

            m_straSSID.remove_all();

            devices = nm_client_get_devices (m_pclient);

            for (i = 0; devices && (i < devices->len); i++)
            {

               NMDevice *device = (NMDevice *) g_ptr_array_index (devices, i);

               if (NM_IS_DEVICE_WIFI (device))
               {

                  m_pdevice = device;

                  show_wifi_device_info (device);

               }

            }

            on_wifi_update();

         }
         else
         {

            bScan = false;

         }

         int j = 10;

         while(j > 0 && thread_get_run())
         {

            Sleep(100);

            j--;

         }

         i++;

      }

      g_object_unref (m_pclient);

      dbus_g_connection_unref (m_bus);

   });

}


void wifi_internal::on_wifi_update()
{

   stringa straNew = m_straSSID - m_pwifi->m_straSSID;

   stringa straDel = m_pwifi->m_straSSID - m_straSSID;

   m_pwifi->on_wifi_update(straDel, straNew);

   m_pwifi->m_straSSID = m_straSSID;

}


char * wifi_internal::ap_wpa_rsn_flags_to_string (guint32 flags)
{

	char * flags_str[16]; /* Enough space for flags and terminating NULL */

	char *ret_str;

	int i = 0;

	if (flags & NM_802_11_AP_SEC_PAIR_WEP40)
		flags_str[i++] = g_strdup ("pair_wpe40");
	if (flags & NM_802_11_AP_SEC_PAIR_WEP104)
		flags_str[i++] = g_strdup ("pair_wpe104");
	if (flags & NM_802_11_AP_SEC_PAIR_TKIP)
		flags_str[i++] = g_strdup ("pair_tkip");
	if (flags & NM_802_11_AP_SEC_PAIR_CCMP)
		flags_str[i++] = g_strdup ("pair_ccmp");
	if (flags & NM_802_11_AP_SEC_GROUP_WEP40)
		flags_str[i++] = g_strdup ("group_wpe40");
	if (flags & NM_802_11_AP_SEC_GROUP_WEP104)
		flags_str[i++] = g_strdup ("group_wpe104");
	if (flags & NM_802_11_AP_SEC_GROUP_TKIP)
		flags_str[i++] = g_strdup ("group_tkip");
	if (flags & NM_802_11_AP_SEC_GROUP_CCMP)
		flags_str[i++] = g_strdup ("group_ccmp");
	if (flags & NM_802_11_AP_SEC_KEY_MGMT_PSK)
		flags_str[i++] = g_strdup ("psk");
	if (flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)
		flags_str[i++] = g_strdup ("802.1X");

	if (i == 0)
		flags_str[i++] = g_strdup ("none");

	flags_str[i] = NULL;

	ret_str = g_strjoinv (" ", flags_str);

	i = 0;
	while (flags_str[i])
		 g_free (flags_str[i++]);

	return ret_str;

}


string wifi_internal::get_access_point_security_info(NMAccessPoint *ap)
{

   synch_lock sl(mutex());

   string strSecurityString;

	guint32 flags, wpa_flags, rsn_flags, freq, bitrate;
	guint8 strength;
	const GByteArray *ssid;
	const char *hwaddr;
	NM80211Mode mode;
	char *freq_str, *ssid_str, *bitrate_str, *strength_str, *wpa_flags_str, *rsn_flags_str;
	GString *security_str;

	/* Get AP properties */
	flags = nm_access_point_get_flags (ap);
	wpa_flags = nm_access_point_get_wpa_flags (ap);
	rsn_flags = nm_access_point_get_rsn_flags (ap);
	ssid = (const GByteArray *)  nm_access_point_get_ssid (ap);
	//hwaddr = nm_access_point_get_hw_address (ap);
	freq = nm_access_point_get_frequency (ap);
	mode = nm_access_point_get_mode (ap);
	bitrate = nm_access_point_get_max_bitrate (ap);
	strength = nm_access_point_get_strength (ap);

	/* Convert to strings */
	ssid_str = (char *)nm_utils_ssid_to_utf8 ( ssid->data, ssid->len);
	freq_str = g_strdup_printf ("%u MHz", freq);
	bitrate_str = g_strdup_printf ("%u MB/s", bitrate/1000);
	strength_str = g_strdup_printf ("%u", strength);
	wpa_flags_str = ap_wpa_rsn_flags_to_string (wpa_flags);
	rsn_flags_str = ap_wpa_rsn_flags_to_string (rsn_flags);

	m_straSSID.add(string(ssid_str));

	m_pwifi->m_mapStrength[string(ssid_str)] = (int) strength;

	security_str = g_string_new (NULL);
	if (   !(flags & NM_802_11_AP_FLAGS_PRIVACY)
	    &&  (wpa_flags != NM_802_11_AP_SEC_NONE)
	    &&  (rsn_flags != NM_802_11_AP_SEC_NONE))
		g_string_append (security_str, "Encrypted: ");

	if (   (flags & NM_802_11_AP_FLAGS_PRIVACY)
	    && (wpa_flags == NM_802_11_AP_SEC_NONE)
	    && (rsn_flags == NM_802_11_AP_SEC_NONE))
		g_string_append (security_str, "WEP ");
	if (wpa_flags != NM_802_11_AP_SEC_NONE)
		g_string_append (security_str, "WPA ");
	if (rsn_flags != NM_802_11_AP_SEC_NONE)
		g_string_append (security_str, "WPA2 ");
	if (   (wpa_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X)
	    || (rsn_flags & NM_802_11_AP_SEC_KEY_MGMT_802_1X))
		g_string_append (security_str, "Enterprise ");

	if (security_str->len > 0)
		g_string_truncate (security_str, security_str->len-1);  /* Chop off last space */


	printf ("SSID:       %s\n", ssid_str);
	printf ("BSSID:      %s\n", hwaddr);
	printf ("Mode:       %s\n", mode == NM_802_11_MODE_ADHOC ? "Ad-Hoc"
	                          : mode == NM_802_11_MODE_INFRA ? "Infrastructure"
	                          : "Unknown");
	printf ("Freq:       %s\n", freq_str);
	printf ("Bitrate:    %s\n", bitrate_str);
	printf ("Strength:   %s\n", strength_str);
	printf ("Security:   %s\n", security_str->str);
	printf ("WPA flags:  %s\n", wpa_flags_str);
	printf ("RSN flags:  %s\n", rsn_flags_str);
	printf ("D-Bus path: %s\n\n", nm_object_get_path (NM_OBJECT (ap)));

	strSecurityString = string(security_str->str, security_str->len);

	g_free (ssid_str);
	g_free (freq_str);
	g_free (bitrate_str);
	g_free (strength_str);
	g_free (wpa_flags_str);
	g_free (rsn_flags_str);
	g_string_free (security_str, TRUE);

	return strSecurityString;

}


void wifi_internal::show_wifi_device_info (NMDevice *device)
{

   synch_lock sl(mutex());

	NMAccessPoint *active_ap = NULL;
	const GPtrArray *aps;
	const char *iface;
	const char *driver;
	guint32 speed;
	const GByteArray *active_ssid;
	char *active_ssid_str = NULL;
	int i;

	/* Get active AP */
	if (nm_device_get_state (device) == NM_DEVICE_STATE_ACTIVATED) {
		if ((active_ap = nm_device_wifi_get_active_access_point (NM_DEVICE_WIFI (device)))) {
			active_ssid = (const GByteArray *)nm_access_point_get_ssid (active_ap);
			active_ssid_str = nm_utils_ssid_to_utf8 (active_ssid->data, active_ssid->len);
		}
	}

        iface = nm_device_get_iface (device);
        driver = nm_device_get_driver (device);
	speed = nm_device_wifi_get_bitrate (NM_DEVICE_WIFI (device));
	speed /= 1000;

	printf ("Device: %s  ----  Driver: %s  ----  Speed: %d MB/s  ----  Active AP: %s\n",
	         iface, driver, speed, active_ssid_str ? active_ssid_str : "none");
	printf ("=================================================================================\n");
	g_free (active_ssid_str);

	/* Get all APs of the Wi-Fi device */
	aps = nm_device_wifi_get_access_points (NM_DEVICE_WIFI (device));

	/* Print AP details */
	for (i = 0; aps && (i < aps->len); i++)
	{

		sl.unlock();

		sl.lock();

		NMAccessPoint *ap = (NMAccessPoint *) g_ptr_array_index (aps, i);

		get_access_point_security_info(ap);

	}

}


NMAccessPoint * wifi_internal::get_access_point(string str, NMDevice *device)
{

   synch_lock sl(mutex());

	const GPtrArray *aps;
	const char *iface;
	const char *driver;
	guint32 speed;
	const GByteArray *ssid;
	char *ssid_str = NULL;
	int i;


	/* Get all APs of the Wi-Fi device */
	aps = nm_device_wifi_get_access_points (NM_DEVICE_WIFI (device));

	/* Print AP details */
	for (i = 0; aps && (i < aps->len); i++)
	{

      NMAccessPoint *ap = (NMAccessPoint *) g_ptr_array_index (aps, i);

   	ssid = (const GByteArray *)nm_access_point_get_ssid (ap);

   	if(ssid != NULL)
   	{

         ssid_str = nm_utils_ssid_to_utf8 (ssid->data, ssid->len);

         if(!strcasecmp(str, ssid_str))
         {

            g_free (ssid_str);

            return ap;

         }

         g_free (ssid_str);

      }

	}

	return NULL;

}
