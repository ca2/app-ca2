#include "stm8.h"
#include "pgm.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

int stm8_write(int * psent, const char * filename, fileformat_t fileformat, int bytes_count,
const stm8_device_t * part, int start, bool bytes_count_specified, programmer_t * pgm,
memtype_t memtype)
{

FILE * f = NULL;

		if(!(f = fopen(filename, (fileformat == RAW_BINARY) ? "rb" : "r")))
		{

         //return "Failed to open file";

         return 0;

      }

		int bytes_count_align = ((bytes_count-1)/part->flash_block_size+1)*part->flash_block_size;
		unsigned char *buf = malloc(bytes_count_align);
		if(!buf)
		{

         //return "malloc failed";

         return 0;

      }


		memset(buf, 0, bytes_count_align); // Clean aligned buffer
		int bytes_to_write;

		/* reading bytes to RAM */
		switch(fileformat)
		{
		case INTEL_HEX:
			bytes_to_write = ihex_read(f, buf, start, start + bytes_count);
			break;
		case MOTOROLA_S_RECORD:
			bytes_to_write = srec_read(f, buf, start, start + bytes_count);
			break;
		default:
			fseek(f, 0L, SEEK_END);
			bytes_to_write = ftell(f);
			if(bytes_count_specified)
				bytes_to_write = bytes_count;
			else if(bytes_count < bytes_to_write)
				bytes_to_write = bytes_count;
			fseek(f, 0, SEEK_SET);
			fread(buf, 1, bytes_to_write, f);
		}
		fprintf(stderr, "%d bytes at 0x%x... ", bytes_to_write, start);

		/* flashing MCU */
		int sent = pgm->write_range(pgm, part, buf, start, bytes_to_write, memtype);
		if(pgm->reset) {
			// Restarting core (if applicable)
			pgm->reset(pgm);
		}
		//fprintf(stderr, "OK\n");
		//fprintf(stderr, "Bytes written: %d\n", sent);
		fclose(f);
*psent = sent;
   return 1;

}
