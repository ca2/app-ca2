#include "framework.h"






//static int open_serial(const char *path, int baud)
//{
//   int fd;
//   struct termios tty;
//
//   fd = open(path, O_RDWR | O_SYNC);
//   if (fd < 0)
//   {
//      perror("open");
//      return -errno;
//   }
//
//   memset(&tty, 0, sizeof(tty));
//   if (tcgetattr(fd, &tty) != 0)
//   {
//      perror("tcgetattr");
//      return -errno;
//   }
//
//   cfsetospeed(&tty, baud);
//   cfsetispeed(&tty, baud);
//
//   tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
//   tty.c_iflag &= ~IGNBRK;                         // disable break processing
//   tty.c_lflag = 0;                                // no signaling chars, no echo,
//   // no canonical processing
//   tty.c_oflag = 0;                                // no remapping, no delays
//   tty.c_cc[VMIN] = 1;                            // read doesn't block
//   tty.c_cc[VTIME] = 5;                            // 0.5 seconds read timeout
//
//   tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // shut off xon/xoff ctrl
//
//   tty.c_cflag |= (CLOCAL | CREAD);                // ignore modem controls,
//   // enable reading
//   tty.c_cflag &= ~(PARENB | PARODD);              // shut off parity
//   tty.c_cflag &= ~CSTOPB;
//   tty.c_cflag &= ~CRTSCTS;
//
//   if (tcsetattr(fd, TCSANOW, &tty) != 0)
//   {
//      perror("tcsetattr");
//      return -errno;
//   }
//
//   return fd;
//}

//static void dump_serial(int serial_fd)
//{
//   char in;
//
//   for (;;) {
//      read(serial_fd, &in, sizeof(in));
//      logprintf("%c", in);
//      fflush(stdout);
//   }
//}

//int main(int argc, char **argv)
//{
//   int a, ret, serial_fd;
//
//   serial_fd = open_serial("/dev/ttyUSB0", 115200);
//   if (serial_fd < 0)
//      return -errno;

//ret = xymodem_send(serial_fd, argv[1], PROTOCOL_XMODEM, 1);
//if (ret < 0)
//   return ret;

//ret = xymodem_send(serial_fd, argv[2], PROTOCOL_YMODEM, 1);
//if (ret < 0)
//   return ret;
//
//   sleep(4);
//
//   ret = xymodem_send(serial_fd, argv[1], PROTOCOL_YMODEM, 0);
//   if (ret < 0)
//      return ret;
//
//   sleep(3);
//
//   ret = xymodem_send(serial_fd, argv[2], PROTOCOL_YMODEM, 0);
//   if (ret < 0)
//      return ret;
//
//   dump_serial(serial_fd);
//
//   return 0;
//}



void set_dlg_item_font(HWND h, int iItem, HFONT hfont)
{


}


string load_resource_string(int iId)
{

   LPWSTR    lpBuffer;
   int iRes = LoadStringW(::aura::system::g_p->m_hinstance, iId, (LPWSTR)&lpBuffer, 0);

   if (iRes <= 0)
   {

      return "";

   }

   string str = wstring(lpBuffer, iRes);

   return str;

}



