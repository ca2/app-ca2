#include "stm8.h"
#include "pgm.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>


int stm8_verify(char * pszMessage, int iCount, int * pverify, const char * filename, fileformat_t fileformat, int bytes_count,
const stm8_device_t * part, int start, bool bytes_count_specified, programmer_t * pgm,
memtype_t memtype)
{

   FILE * f = NULL;

		int bytes_count_align = ((bytes_count-1)/256+1)*256; // Reading should be done in blocks of 256 bytes
		unsigned char *buf = malloc(bytes_count_align);
		if(!buf)
		{

         ///spawn_error("Failed to malloc");

         snprintf(pszMessage, iCount, "Failed to malloc");
         return 0;

		}

		int recv = pgm->read_range(pgm, part, buf, start, bytes_count_align);

		if(recv < bytes_count_align)
		{

         if(pszMessage)
         {

            snprintf(pszMessage, iCount, "Failed to read MCU\nRequested %d bytes but received only %d.\n", bytes_count_align, recv);

            //spawn_error("Failed to read MCU");

            return;

         }

         }

         if(!(f = fopen(filename, (fileformat == RAW_BINARY) ? "rb" : "r")))
         {

            //spawn_error("Failed to open file");
            snprintf(pszMessage, iCount, "Failed to open file");
            return 0;

         }
		unsigned char *buf2 = malloc(bytes_count);
		if(!buf2)
		{
         //spawn_error("malloc failed");

         snprintf(pszMessage, iCount, "malloc failed (2)");

         return 0;

		}
		//int bytes_to_verify;
		/* reading bytes to RAM */
		switch(fileformat)
		{
		case INTEL_HEX:
			*pverify = ihex_read(f, buf2, start, start + bytes_count);
			break;
		case MOTOROLA_S_RECORD:
			*pverify = srec_read(f, buf2, start, start + bytes_count);
			break;
		default:
			fseek(f, 0L, SEEK_END);
			*pverify = ftell(f);
			if(bytes_count_specified)
				*pverify = bytes_count;
			else if(bytes_count < *pverify)
				*pverify = bytes_count;
			fseek(f, 0, SEEK_SET);
			fread(buf2, 1, *pverify, f);
		}
		fclose(f);

		if(memcmp(buf, buf2, *pverify) != 0)
		{

			snprintf(pszMessage, iCount, "FAILED\n");

			return 0;

		}

   snprintf(pszMessage, iCount, "OK\nBytes verified: %d\n", *pverify);

	return 1;

 }
