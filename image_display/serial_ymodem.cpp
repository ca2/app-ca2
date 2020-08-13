/* serial for RTD Serial Recovery (rtdsr)
*
* copyright (c) 2011 Pete B. <xtreamerdev@gmail.com>
*
* based on serial.c for bootldr, copyright (c) 2001 John G Dorsey
* baded on serial.c for reimage, copyright (c) 2009 Rich M Legrand
* crc16 function from PIC CRC16, by Ashley Roll & Scott Dattalo
* crc32 function from crc32.c by Craig Bruce
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/>.
*
*/

/*
* These Ymodem calls are aimed at embedded software and tailored to
* work against Microsoft's HyperTerminal. Some of the Ymodem protocol
*  operations have been voluntarily left out.
*
* To be able to use these functions, you must provide:
* o int _getchar(int timeout): A serial getchar() call, with a
*   timeout expressed in seconds. Negative means infinite timeout.
*   should return the read character, as an int, or negative on
*   error/timeout.
* o void serial_putchar(int c): A serial putchar() call
* o logprintf(), thought all the printfs can be removed if needed.
*/
#include "framework.h"
#include "serial_util.h"
#include "logprintf.h"

#ifdef WITH_CRC32
/* http://csbruce.com/~csbruce/software/crc32.c */
static unsigned long crc32(const unsigned char* buf, unsigned long count)
{
   unsigned long crc = 0xFFFFFFFF;
   unsigned long i;

   /* This static table adds 1K */
   static const unsigned long crc_table[256] =
   {
      0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,
      0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,
      0xE7B82D07,0x90BF1D91,0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,
      0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,
      0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,0x3B6E20C8,0x4C69105E,0xD56041E4,
      0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,0x35B5A8FA,0x42B2986C,
      0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,0x26D930AC,
      0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
      0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,
      0xB6662D3D,0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,
      0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,
      0x086D3D2D,0x91646C97,0xE6635C01,0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,
      0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,
      0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,0x4DB26158,0x3AB551CE,
      0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,0x4369E96A,
      0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
      0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,
      0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,
      0xB7BD5C3B,0xC0BA6CAD,0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,
      0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,
      0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,0xF00F9344,0x8708A3D2,0x1E01F268,
      0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,
      0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,0xD6D6A3E8,
      0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
      0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,
      0x4669BE79,0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,
      0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,
      0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,
      0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,
      0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,0x86D3D2D4,0xF1D4E242,
      0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,
      0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
      0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,
      0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,
      0x47B2CF7F,0x30B5FFE9,0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,
      0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,
      0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D
   };

   for (i = 0; i<count; i++)
   {

      crc = (crc >> 8) ^ crc_table[(crc ^ buf[i]) & 0xFF];

   }

   return(crc ^ 0xFFFFFFFF);

}


#endif


/* http://www.ccsinfo.com/forum/viewtopic.php?t=24977 */
static unsigned short crc16(const unsigned char *buf, unsigned long count)
{

   unsigned short crc = 0;

   int i;

   while (count--)
   {

      crc = crc ^ *buf++ << 8;

      for (i = 0; i<8; i++)
      {

         if (crc & 0x8000)
         {

            crc = crc << 1 ^ 0x1021;

         }
         else
         {

            crc = crc << 1;

         }

      }

   }

   return crc;

}


string u32_to_str(unsigned int val)
{

   return ::str::from(val);

}



static unsigned long str_to_u32(char* str)
{

   const char *s = str;

   unsigned long acc;

   int c;

   /* strip leading spaces if any */
   do
   {

      c = *s++;

   }
   while (c == ' ');

   for (acc = 0; (c >= '0') && (c <= '9'); c = *s++)
   {

      c -= '0';

      acc *= 10;

      acc += c;

   }

   return acc;

}


/* Returns 0 on success, 1 on corrupt packet, -1 on error (timeout): */
static int receive_packet(struct serial_serial * pserial, char *data, int *length)
{

   int i, c;

   unsigned int packet_size;

   *length = 0;

   c = serial_getchar(pserial, PACKET_TIMEOUT);

   if (c < 0)
   {

      return -1;

   }

   switch (c)
   {
   case SOH:

      packet_size = PACKET_SIZE;

      break;

   case STX:

      packet_size = PACKET_1K_SIZE;

      break;

   case EOT:

      return 0;

   case CAN:

      c = serial_getchar(pserial, PACKET_TIMEOUT);

      if (c == CAN)
      {

         *length = -1;

         return 0;

      }

   default:

      /* This case could be the result of corruption on the first octet
      * of the packet, but it's more likely that it's the user banging
      * on the terminal trying to abort a transfer. Technically, the
      * former case deserves a NAK, but for now we'll just treat this
      * as an abort case.
      */

      *length = -1;

      return 0;

   }

   *data = (char)c;

   for (i = 1; i < (packet_size + PACKET_OVERHEAD); ++i)
   {

      c = serial_getchar(pserial, PACKET_TIMEOUT);

      if (c < 0)
      {

         return -1;

      }

      data[i] = (char)c;

   }

   /* Just a sanity check on the sequence number/complement value.
   * Caller should check for in-order arrival.
   */
   if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
   {

      return 1;

   }

   if (crc16((unsigned char *)(data + PACKET_HEADER), packet_size + PACKET_TRAILER) != 0)
   {

      return 1;

   }

   *length = packet_size;

   return 0;

}


/* Returns the length of the file received, or 0 on error: */
unsigned long ymodem_receive(struct serial_serial * pserial, unsigned char * buf, unsigned long length)
{

   unsigned char packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD];

   int packet_length, i, file_done, session_done, crc_nak = 1;

   unsigned int packets_received, errors, first_try = 1;

   char file_name[FILE_NAME_LENGTH], file_size[FILE_SIZE_LENGTH], *file_ptr;

   unsigned char *buf_ptr;

   unsigned long size = 0;

   logprintf("%s > Ymodem rcv:\n", pserial->m_pszPort);

   file_name[0] = 0;

   for (session_done = 0, errors = 0; ; )
   {

      if (!first_try)
      {

         serial_putchar(pserial, CRC);

      }

      first_try = 0;

      for (packets_received = 0, file_done = 0, buf_ptr = buf; ; )
      {

         switch (receive_packet(pserial, (char *)packet_data, &packet_length))
         {

         case 0:

            errors = 0;

            switch (packet_length)
            {
            case -1:  /* abort */

               serial_putchar(pserial, ACK);

               return 0;

            case 0:   /* end of transmission */

               serial_putchar(pserial, ACK);

               /* Should add some sort of sanity check on the number of
               * packets received and the advertised file length.
               */

               file_done = 1;

               break;

            default:  /* normal packet */

               if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
               {

                  serial_putchar(pserial, NAK);

               }
               else
               {

                  if (packets_received == 0)
                  {

                     /* The spec suggests that the whole data section should
                     * be zeroed, but I don't think all senders do this. If
                     * we have a NULL filename and the first few digits of
                     * the file length are zero, we'll call it empty.
                     */
                     for (i = PACKET_HEADER; i < PACKET_HEADER + 4; i++)
                     {

                        if (packet_data[i] != 0)
                        {

                           break;

                        }

                     }

                     if (i < PACKET_HEADER + 4)    /* filename packet has data */
                     {

                        for (file_ptr = (char *)(packet_data + PACKET_HEADER), i = 0; *file_ptr && i < FILE_NAME_LENGTH; )
                        {

                           file_name[i++] = *file_ptr++;

                        }

                        file_name[i++] = '\0';

                        for (++file_ptr, i = 0; *file_ptr != ' ' && i < FILE_SIZE_LENGTH; )
                        {

                           file_size[i++] = *file_ptr++;

                        }

                        file_size[i++] = '\0';

                        size = str_to_u32(file_size);

                        if (size > length)
                        {

                           serial_putchar(pserial, CAN);

                           serial_putchar(pserial, CAN);

                           serial_ssleep(1);

                           logprintf("%s > rcv buffer too small (0x%08x vs 0x%08x)\n", pserial->m_pszPort, length, size);

                           return 0;

                        }

                        serial_putchar(pserial, ACK);

                        serial_putchar(pserial, crc_nak ? CRC : NAK);

                        crc_nak = 0;

                     }
                     else    /* filename packet is empty; end session */
                     {

                        serial_putchar(pserial, ACK);

                        file_done = 1;

                        session_done = 1;

                        break;

                     }

                  }
                  else
                  {

                     /* This shouldn't happen, but we check anyway in case the
                     * sender lied in its filename packet:
                     */

                     if ((buf_ptr + packet_length) - buf > length)
                     {

                        serial_putchar(pserial, CAN);

                        serial_putchar(pserial, CAN);

                        serial_ssleep(1);

                        logprintf("%s > buffer overflow: %d\n", pserial->m_pszPort, length);

                        return 0;

                     }

                     for (i = 0; i<packet_length; i++)
                     {

                        buf_ptr[i] = packet_data[PACKET_HEADER + i];

                     }

                     buf_ptr += packet_length;

                     serial_putchar(pserial, ACK);

                  }

                  ++packets_received;

               }  /* sequence number ok */

            }

            break;

         default:

            if (packets_received != 0)
            {

               if (++errors >= MAX_ERRORS)
               {

                  serial_putchar(pserial, CAN);

                  serial_putchar(pserial, CAN);

                  serial_ssleep(1);

                  logprintf("%s > too many errors - aborted.\n", pserial->m_pszPort);

                  return 0;

               }

            }

            serial_putchar(pserial, CRC);

         }

         if (file_done)
         {

            break;

         }

      }  /* receive packets */

      if (session_done)
      {

         break;

      }

   }  /* receive files */

   if (size > 0)
   {

      logprintf("%s > read: %s\n", pserial->m_pszPort, file_name);

#ifdef WITH_CRC32

      logprintf("%s > crc32: 0x%08x\n", pserial->m_pszPort, crc32(buf, size));

#endif

      logprintf("%s > len: 0x%08x\n", pserial->m_pszPort, size);

   }

   return size;

}


static void send_packet(struct serial_serial * pserial,unsigned char *data, int block_no)
{

   int count, crc, packet_size;

   /* We use a short packet for block 0 - all others are 1K */
   if (block_no == 0)
   {

      packet_size = PACKET_SIZE;

   }
   else
   {

      packet_size = PACKET_1K_SIZE;

   }

   crc = crc16(data, packet_size);

   /* 128 byte packets use SOH, 1K use STX */
   serial_putchar(pserial, (block_no == 0) ? SOH : STX);

   serial_putchar(pserial, block_no & 0xFF);

   serial_putchar(pserial, ~block_no & 0xFF);

   logprintf("%s > block no.: %d\n", pserial->m_pszPort, block_no);

   logprintf("%s > packet size: %d\n", pserial->m_pszPort, packet_size);

   for (count = 0; count<packet_size; count++)
   {

      serial_putchar(pserial, data[count]);

   }

   serial_putchar(pserial, (crc >> 8) & 0xFF);

   serial_putchar(pserial, crc & 0xFF);

}


/* Send block 0 (the filename block). filename might be truncated to fit. */
static void send_packet0(struct serial_serial * pserial,char* filename, unsigned long size)
{

   unsigned long count = 0;

   unsigned char block[PACKET_SIZE];

   logprintf("send_packet0\n");

   if (filename)
   {

      logprintf("%s > filename: \"%s\"\n", pserial->m_pszPort, filename);

      while (*filename && (count < PACKET_SIZE - FILE_SIZE_LENGTH - 2))
      {

         block[count++] = *filename++;

      }

      block[count++] = 0;

      logprintf("%s > sizenum: %d bytes\n", pserial->m_pszPort, size);

      string strNum = u32_to_str((unsigned int)size);

      logprintf("%s > sizechar: \"%s\"\n", pserial->m_pszPort, strNum.c_str());

      const char * pszNum = strNum;

      while (*pszNum)
      {

         block[count++] = *pszNum++;

      }

   }

   logprintf("%s > (count = %d)\n", pserial->m_pszPort, count);

   while (count < PACKET_SIZE)
   {

      block[count++] = 0;

   }

   send_packet(pserial, block, 0);

}


static int send_data_packets(struct serial_serial * pserial, unsigned char* data, unsigned long size, void (*pfnBytesSent)(struct serial_serial * pserial, int))
{

   int blockno = 1;

   unsigned long send_size;

   unsigned long total = 0;

   int ch;

   while (size > 0)
   {

      if (size > PACKET_1K_SIZE)
      {

         send_size = PACKET_1K_SIZE;

      }
      else
      {

         send_size = size;

      }

      send_packet(pserial, data, blockno);

      ch = serial_getchar(pserial, PACKET_TIMEOUT);

      if (ch == ACK)
      {

         logprintf("%s > ACK\n", pserial->m_pszPort);

         blockno++;

         data += send_size;

         size -= send_size;

         total += send_size;

         pfnBytesSent(pserial, (int) total);

      }
      else
      {

         if (ch == CAN)
         {

            logprintf("%s > CAN\n", pserial->m_pszPort);

            return -2;

         }
         else if (ch < 0)
         {

            logprintf("%s > Timeout\n", pserial->m_pszPort);

            return -1;

         }

      }

   }

   do
   {

      logprintf("%s > EOT\n", pserial->m_pszPort);

      serial_putchar(pserial, EOT);

      ch = serial_getchar(pserial, PACKET_TIMEOUT);

      if (ch == ACK)
      {

         logprintf("%s > ACK\n", pserial->m_pszPort);

      }
      else
      {

         logprintf("%s > %c", pserial->m_pszPort, ch);

      }

   }
   while ((ch != ACK) && (ch >= 0));

   /* Send last data packet */
   if (ch == ACK)
   {

      logprintf("%s > Getting char\n", pserial->m_pszPort);

      ch = serial_getchar(pserial, PACKET_TIMEOUT);

      if (ch == CRC)
      {

         logprintf("%s > Got CRC\n", pserial->m_pszPort);

         do
         {

            logprintf("%s > send_packet0 0 0\n", pserial->m_pszPort);

            send_packet0(pserial, 0, 0);

            ch = serial_getchar(pserial, PACKET_TIMEOUT);

            if (ch == ACK)
            {

               logprintf("%s > ACK\n", pserial->m_pszPort);

            }
            else
            {

               logprintf("%s > %c", pserial->m_pszPort, ch);

            }

         }
         while ((ch != ACK) && (ch >= 0));

      }

   }

   return 0;

}


unsigned long ymodem_send(struct serial_serial * pserial, unsigned char* buf, unsigned long size, char* filename, void (*pfnBytesSent)(struct serial_serial * pserial, int))
{

   int ch = 0, crc_nak = 20;

   logprintf("%s > Ymodem send:\n", pserial->m_pszPort);
   
   if(pserial->m_iMode == SERIAL_MODE_QUASAR)
   {

	   /* Flush the RX FIFO, after a cool off delay */
	   serial_ssleep(1);

	   int iCrcCount = 0;

	   int iTry = 8;

	   while (iCrcCount < 3 && iTry > 0)
	   {

		  ch = serial_getchar(pserial, 2);

		  if (ch == CRC)
		  {

			 iCrcCount++;

		  }
		  else
		  {

			 iCrcCount = 0;

			 if (ch < 0)
			 {

				iTry--;

			 }

		  }

		  if (ch >= 0)
		  {

			 logprintf("%s > %c\n", pserial->m_pszPort, (char)ch);

		  }

	   }

	   ///* Not in the specs, just for balance */
	   //do
	   //{
		  //serial_putchar(CRC);
	   
	   //}
	   //while (ch < 0);
	   
	}
	else
	{


		/* Flush the RX FIFO, after a cool off delay */




		serial_ssleep(1);


		while (serial_read(pserial) >= 0);



		/* Not in the specs, just for balance */


		do {


			serial_putchar(pserial, CRC);


			ch = serial_getchar(pserial, 1);


		} while (ch < 0);



	}




   if (ch == CRC)
   {

      do
      {
         send_packet0(pserial, filename, size);
         /* When the receiving program receives this block and successfully
         * opened the output file, it shall acknowledge this block with an ACK
         * character and then proceed with a normal XMODEM file transfer
         * beginning with a "C" or NAK tranmsitted by the receiver.
         */
         ch = serial_getchar(pserial, PACKET_TIMEOUT * 5);

         if (ch == ACK)
         {

            logprintf("%s > ACK (1)\n", pserial->m_pszPort);

            ch = serial_getchar(pserial, PACKET_TIMEOUT);

            if (ch == CRC)
            {

               logprintf("%s > CRC (1)\n", pserial->m_pszPort);

               int iRes = send_data_packets(pserial, buf, size, pfnBytesSent);

               if (iRes < 0)
               {

                  logprintf("%s > failure: %s\n", pserial->m_pszPort, filename);

                  return iRes;

               }

               logprintf("%s > sent: %s\n", pserial->m_pszPort, filename);

#ifdef WITH_CRC32

               logprintf("%s > crc32: 0x%08x\n", pserial->m_pszPort, crc32(buf, size));

#endif

               logprintf("%s > len: 0x%08x\n", pserial->m_pszPort, size);

               return size;

            }

         }
         else if ((ch == CRC) && (crc_nak))
         {

            logprintf("%s > %c", pserial->m_pszPort, (char)ch);

            crc_nak--;

            continue;

         }
         else if (ch == NAK && crc_nak <= 0)
         {

            logprintf("%s > NAK\n", pserial->m_pszPort);

         }
         else
         {

            logprintf("%s > Unexpected %c\n", pserial->m_pszPort, ch);

            break;

         }

      }
      while (1);

   }

   serial_putchar(pserial, CAN);

   serial_putchar(pserial, CAN);

   serial_ssleep(1);

   logprintf("%s > aborted.\n", pserial->m_pszPort);

   return 0;

}
