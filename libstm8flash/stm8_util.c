
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>

#include <unistd.h>

#include "pgm.h"
#include "espstlink.h"
#include "stlink.h"
#include "stlinkv2.h"
#include "stm8.h"
#include "ihex.h"
#include "srec.h"




programmer_t g_pgms[] = {
	{ 	"stlink",
		STLinkV1,
		0x0483, // USB vid
		0x3744, // USB pid
		stlink_open,
		stlink_close,
		stlink_swim_srst,
		stlink_swim_read_range,
		stlink_swim_write_range,
	},
	{
		"stlinkv2",
		STLinkV2,
		0x0483,
		0x3748,
		stlink2_open,
		stlink_close,
		stlink2_srst,
		stlink2_swim_read_range,
		stlink2_swim_write_range,
	},
	{
		"stlinkv21",
		STLinkV21,
		0x0483,
		0x374b,
		stlink2_open,
		stlink_close,
		stlink2_srst,
		stlink2_swim_read_range,
		stlink2_swim_write_range,
	},
	{
		"stlinkv3",
		STLinkV3,
		0x0483,
		0x374f,
		stlink2_open,
		stlink_close,
		stlink2_srst,
		stlink2_swim_read_range,
		stlink2_swim_write_range,
	},
	{
		"espstlink",
		ESP_STLink,
		0,
		0,
		espstlink_pgm_open,
		espstlink_pgm_close,
		espstlink_srst,
		espstlink_swim_read_range,
		espstlink_swim_write_range,
	},
	{ NULL },
};

programmer_t * stm8_get_programmers()
{

   return g_pgms;

}

programmer_t *  stm8_get_programmer(const char * optarg)
{

   programmer_t * pgm = NULL;

   programmer_t * pgms = stm8_get_programmers();

   for(int i = 0; pgms[i].name; i++)
   {

      if(!strcmp(optarg, pgms[i].name))
      {

         pgm = &pgms[i];

         break;

      }

   }

   return pgm;

}


int is_ext(const char *filename, const char *ext) {
	char *ext_begin = strrchr(filename, '.');
	return(ext_begin && strcmp(ext_begin, ext) == 0);
}



void print_help_and_exit(const char *name, int err)
{
	int i = 0;
	FILE *stream = err ? stderr : stdout;
	fprintf(stream, "Usage: %s [-c programmer] [-S serialno] [-p partno] [-s memtype] [-b bytes] [-r|-w|-v] <filename>\n", name);
	fprintf(stream, "Options:\n");
	fprintf(stream, "\t-?             Display this help\n");
	fprintf(stream, "\t-c programmer  Specify programmer used (");
	const programmer_t * pgms = stm8_get_programmers();
	while (1) {
		if (pgms[i].name == NULL)
			break;

		if (i) {
			if (pgms[i+1].name == NULL)
				fprintf(stream, " or ");
			else
				fprintf(stream, ", ");
		}

		fprintf(stream, "%s", pgms[i].name);
		i++;
	}
	fprintf(stream, ")\n");
	fprintf(stream, "\t-S serialno    Specify programmer's serial number. If not given and more than one programmer is available, they'll be listed.\n");
	fprintf(stream, "\t-d port        Specify the serial device for espstlink (default: /dev/ttyUSB0)\n");
	fprintf(stream, "\t-p partno      Specify STM8 device\n");
	fprintf(stream, "\t-l             List supported STM8 devices\n");
	fprintf(stream, "\t-s memtype     Specify memory type (flash, eeprom, ram, opt or explicit address)\n");
	fprintf(stream, "\t-b bytes       Specify number of bytes\n");
	fprintf(stream, "\t-r <filename>  Read data from device to file\n");
	fprintf(stream, "\t-w <filename>  Write data from file to device\n");
	fprintf(stream, "\t-v <filename>  Verify data in device against file\n");
	fprintf(stream, "\t-V             Print Date(YearMonthDay-Version) and Version format is IE: 20171204-1.0\n");
	fprintf(stream, "\t-u             Unlock. Reset option bytes to factory default to remove write protection.\n");
	exit(-err);
}








memtype_t stm8_get_memtype(const char * optarg)
{
	if(strcasecmp(optarg, "flash") == 0)
	{

      return FLASH;

   }
   else if(strcasecmp(optarg, "eeprom") == 0)
   {

      return EEPROM;

   }
   else if(strcasecmp(optarg, "ram") == 0)
   {

      return RAM;

   }
   else if(strcasecmp(optarg, "opt") == 0)
   {

      return OPT;

   }

	return -1;

}
