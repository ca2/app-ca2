/* stlink/v2 stm8 memory programming utility
   (c) Valentin Dudouyt, 2012 - 2014 */

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


#ifdef __APPLE__
extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;
#endif

#define VERSION_RELASE_DATE "20190501"
#define VERSION "1.2"
#define VERSION_NOTES "DEBUG3"



void print_help_and_exit(const char *name, int err);

void print_version_and_exit( int err) {
	FILE *stream = err ? stderr : stdout;
	fprintf(stream, "%s-%s\n%s",VERSION_RELASE_DATE, VERSION, VERSION_NOTES );
	exit(-err);
}


void stm8_get_version(char * szMessage, int iCount)
{

	snprintf(szMessage, iCount, "app-ca2.libstm8flash %s-%s\n%s", VERSION_RELASE_DATE, VERSION, VERSION_NOTES);

}



void spawn_error(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(-1);
}

void dump_pgms(programmer_t *pgms) {
	// Dump programmers list in stderr
	int i;
	for(i = 0; pgms[i].name; i++)
		fprintf(stderr, "%s\n", pgms[i].name);
}

void serialno_to_hex(const char *serialno, char *serialno_hex) {
	for(int i=0;i<strlen(serialno);i++)
	{
		serialno_hex += sprintf(serialno_hex, "%02X", serialno[i]);
	}
}

int stm8_usb_init(char * szMessage, int iCount, programmer_t *pgm, int pgm_serialno_specified, char *pgm_serialno) {
	if (!pgm->usb_vid && !pgm->usb_pid) return(true);
   memset(szMessage, 0, iCount);
	char sz[2048];
	libusb_device **devs;
	libusb_context *ctx = NULL;
	int numOfProgrammers = 0;
	char vendor[32];
	char device[32];
	char serialno[32];
	char serialno_hex[64];


	int r;
	ssize_t cnt;
	r = libusb_init(&ctx);
	if(r < 0) return(false);

	{
#ifdef STM8FLASH_LIBUSB_QUIET
		const int usb_debug_level = 0;
#else
		const int usb_debug_level = 3;
#endif
#if defined(LIBUSB_API_VERSION) && (LIBUSB_API_VERSION >= 0x01000106)
		libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, usb_debug_level);
#else
		libusb_set_debug(ctx, usb_debug_level);
#endif
	}

	cnt = libusb_get_device_list(ctx, &devs);
	if(cnt < 0) return(false);

   snprintf(sz, sizeof(sz), "Searching vid=%d pid=%d\n", pgm->usb_vid, pgm->usb_pid);
   strcat(szMessage, sz);

	// count available programmers
	for(int i=0; i<cnt; i++) {
		struct libusb_device_descriptor desc;
		libusb_get_device_descriptor(devs[i], &desc);
      snprintf(sz, sizeof(sz), "Device=%d vid=%d pid=%d\n", i, desc.idVendor, desc.idProduct);
      strcat(szMessage, sz);
		if(desc.idVendor == pgm->usb_vid && desc.idProduct == pgm->usb_pid) {
			numOfProgrammers++;
		}

	}

	if(numOfProgrammers > 1 || pgm_serialno_specified){

		// no serialno given
		if(!pgm_serialno_specified) {
			snprintf(sz, sizeof(sz), "WARNING: More than one programmer found but no serial number given. Programmer 1 will be used:\n");
         strcat(szMessage, sz);
			pgm->dev_handle = libusb_open_device_with_vid_pid(ctx, pgm->usb_vid, pgm->usb_pid);
		}

		numOfProgrammers = 0;
		int i=0;
		for(i=0; i<cnt; i++) {
			struct libusb_device_descriptor desc;
			libusb_device_handle *tempHandle;

			libusb_get_device_descriptor(devs[i], &desc);
			if(desc.idVendor == pgm->usb_vid && desc.idProduct == pgm->usb_pid) {
				numOfProgrammers++;

				libusb_open(devs[i], &tempHandle);

				libusb_get_string_descriptor_ascii(tempHandle, desc.iManufacturer, (unsigned char*)vendor, sizeof(vendor));
				libusb_get_string_descriptor_ascii(tempHandle, desc.iProduct, (unsigned char*)device, sizeof(device));
				libusb_get_string_descriptor_ascii(tempHandle, desc.iSerialNumber, (unsigned char*)serialno, sizeof(serialno));
				serialno_to_hex(serialno, serialno_hex);

				// print programmer data if no serial number specified
				if(!pgm_serialno_specified) {
					snprintf(szMessage, iCount, "Programmer %d: %s %s, Serial Number:%s\n", numOfProgrammers, vendor, device, serialno_hex);
               strcat(szMessage, sz);
				}
				else
				{
					// otherwise check if it's the correct one
					if(0==strcmp(serialno_hex, pgm_serialno)) {
						pgm->dev_handle = tempHandle;
						break;
					}
				}
				libusb_close(tempHandle);
			}

		}
		if(pgm_serialno_specified && i==cnt) {
			snprintf(sz, sizeof(sz), "ERROR: No programmer with serial number %s found.\n", pgm_serialno);
         strcat(szMessage, sz);
			return(false);
		}
	}
	else
	{
		pgm->dev_handle = libusb_open_device_with_vid_pid(ctx, pgm->usb_vid, pgm->usb_pid);
	}




	pgm->ctx = ctx;
	// assert(pgm->dev_handle);

	libusb_free_device_list(devs, 1); //free the list, unref the devices in it

   if (!pgm->dev_handle)
	{

      snprintf(sz, sizeof(sz), "Could not find any programmer.");

      strcat(szMessage, sz);

      return false;

   }

	 //find out if kernel driver is attached
	if(libusb_kernel_driver_active(pgm->dev_handle, 0) == 1)
	{

		int r = libusb_detach_kernel_driver(pgm->dev_handle, 0);
		if(r != 0);
		{

	      snprintf(sz, sizeof(sz), "failed to detach USB driver.");

         strcat(szMessage, sz);

         return false;

		}

	}

	r = libusb_claim_interface(pgm->dev_handle, 0);

   if(r != 0)
	{

      snprintf(sz, sizeof(sz), "failed to claim USB interface.");

      strcat(szMessage, sz);

      return false;

   }

	return(true);

}


const stm8_device_t *get_part(const char *name)
{
	for(unsigned int i = 0; stm8_devices[i].name; i++)
	{
		const char *e = stm8_devices[i].name;
		const char *s = name;
		for(e = stm8_devices[i].name, s = name; *s && (*e == *s || toupper(*e) == *s || *e == '?'); e++, s++);
		if(!*e)
			return(&stm8_devices[i]);
	}
	return(0);
}

int stm8_main(int argc, char **argv) {
		char szMessage[4096];


	unsigned int start;
	int bytes_count = 0;
	char filename[256];
	memset(filename, 0, sizeof(filename));
	char pgm_serialno[64];
	memset(pgm_serialno, 0, sizeof(pgm_serialno));
	// Parsing command line
	char c;
	action_t action = NONE;
	fileformat_t fileformat = RAW_BINARY;
	int start_addr_specified = false,
		pgm_specified = false,
		pgm_serialno_specified = false,
		part_specified = false,
        bytes_count_specified = false;
	memtype_t memtype = FLASH;
	const char * port = NULL;
	int i;
	programmer_t *pgm = NULL;
	const stm8_device_t *part = NULL;
	while((c = getopt (argc, argv, "r:w:v:nc:S:p:d:s:b:luV")) != (char)-1) {
		switch(c) {
			case 'c':
				pgm_specified = true;
				pgm = stm8_get_programmer(optarg);
				break;
			case 'S':
				pgm_serialno_specified = true;
				if(NULL != optarg)
					strncpy(pgm_serialno, optarg, sizeof(pgm_serialno));
				break;
			case 'p':
				part_specified = true;
				part = get_part(optarg);
				break;
			case 'd':
				port = strdup(optarg);
				break;
			case 'l':
				for(i = 0; stm8_devices[i].name; i++)
					printf("%s ", stm8_devices[i].name);
				printf("\n");
				exit(0);
			case 'r':
				action = READ;
				strcpy(filename, optarg);
				break;
			case 'w':
				action = WRITE;
				strcpy(filename, optarg);
				break;
			case 'v':
				action = VERIFY;
				strcpy(filename, optarg);
				break;
                        case 'u':
				action = UNLOCK;
				start  = 0x4800;
				memtype = OPT;
				strcpy(filename, "Workaround");
				break;
			case 's':
			{
                // Start addr is depending on MCU type
            memtype_t memtype = stm8_get_memtype(optarg);

            if(memtype >= 0)
            {

               //memtype = iMemType;

				} else {
					// Start addr is specified explicitely
					memtype = UNKNOWN;
					if(sscanf(optarg, "%x", (unsigned*)&start) != 1)
						spawn_error("Invalid memory type or location specified");
					start_addr_specified = true;
				}

				}
				break;
			case 'b':
				bytes_count = atoi(optarg);
                bytes_count_specified = true;
				break;
			case 'V':
                                print_version_and_exit( (int)0);
				break;
			case '?':
                                print_help_and_exit(argv[0], false);
			default:
				print_help_and_exit(argv[0], true);
		}
	}
	if(argc <= 1)
		print_help_and_exit(argv[0], true);
	if(pgm_specified && !pgm) {
		fprintf(stderr, "No valid programmer specified. Possible values are:\n");
		dump_pgms( stm8_get_programmers());
		exit(-1);
	}
	if(!pgm)
		spawn_error("No programmer has been specified");
	pgm->port = port;
	if(part_specified && !part) {
		fprintf(stderr, "No valid part specified. Use -l to see the list of supported devices.\n");
		exit(-1);
	}
	if(!part)
		spawn_error("No part has been specified");

    // Try define memory type by address
	if(memtype == UNKNOWN) {
        if((start >= 0x4800) && (start < 0x4880)) {
            memtype = OPT;
        }
        if((start >= part->ram_start) && (start < part->ram_start + part->ram_size)) {
            memtype = RAM;
        }
        else if((start >= part->flash_start) && (start < part->flash_start + part->flash_size)) {
            memtype = FLASH;
        }
        else if((start >= part->eeprom_start) && (start < part->eeprom_start + part->eeprom_size)) {
            memtype = EEPROM;
        }
    }

	if(memtype != UNKNOWN)
	{

      stm8_programmer_memtype(&start, memtype, &start_addr_specified, &bytes_count, bytes_count_specified, part);

	}
	if(!action)
		spawn_error("No action has been specified");
	if(!start_addr_specified)
		spawn_error("No memtype or start_addr has been specified");
	if (!strlen(filename))
		spawn_error("No filename has been specified");
	if(!action || !start_addr_specified || !strlen(filename))
		print_help_and_exit(argv[0], true);
	if(!stm8_usb_init(szMessage, sizeof(szMessage), pgm, pgm_serialno_specified, pgm_serialno))
	{
      strcat(szMessage, "Couldn't initialize stlink");
		spawn_error(szMessage);

		}
	if(!pgm->open(pgm))
		spawn_error("Error communicating with MCU. Please check your SWIM connection.");

	if(is_ext(filename, ".ihx") || is_ext(filename, ".hex") || is_ext(filename, ".i86"))
		fileformat = INTEL_HEX;
	else if(is_ext(filename, ".s19") || is_ext(filename, ".s8") || is_ext(filename, ".srec"))
		fileformat = MOTOROLA_S_RECORD;
	fprintf(stderr, "Due to its file extension (or lack thereof), \"%s\" is considered as %s format!\n", filename, fileformat == INTEL_HEX ? "INTEL HEX" : (fileformat == MOTOROLA_S_RECORD ? "MOTOROLA S-RECORD" : "RAW BINARY"));

	FILE *f;
	if(action == READ) {
		fprintf(stderr, "Reading %d bytes at 0x%x... ", bytes_count, start);
		fflush(stderr);
		int bytes_count_align = ((bytes_count-1)/256+1)*256; // Reading should be done in blocks of 256 bytes
		unsigned char *buf = malloc(bytes_count_align);
		if(!buf) spawn_error("malloc failed");
		int recv = pgm->read_range(pgm, part, buf, start, bytes_count_align);
		if(recv < bytes_count_align) {
			fprintf(stderr, "\r\nRequested %d bytes but received only %d.\r\n", bytes_count_align, recv);
			spawn_error("Failed to read MCU");
		}
		if(!(f = fopen(filename, (fileformat == RAW_BINARY) ? "wb" : "w")))
			spawn_error("Failed to open file");
		switch(fileformat)
		{
		case INTEL_HEX:
			ihex_write(f, buf, start, start+bytes_count);
			break;
		case MOTOROLA_S_RECORD:
			srec_write(f, buf, start, start+bytes_count);
			break;
		default:
			fwrite(buf, 1, bytes_count, f);
		}
		fclose(f);
		fprintf(stderr, "OK\n");
		fprintf(stderr, "Bytes received: %d\n", bytes_count);
	} else if (action == VERIFY) {
		fprintf(stderr, "Verifing %d bytes at 0x%x... ", bytes_count, start);

		fflush(stderr);

		int verify = 0;

		int iResult = stm8_verify(szMessage, sizeof(szMessage),&verify, filename, fileformat, bytes_count, part, start, bytes_count_specified, pgm, memtype );

      fprintf(stderr, "%s", szMessage);

	}
	else if (action == WRITE)
	{

      int sent = 0;

      stm8_write(&sent, filename, fileformat, bytes_count, part, start, bytes_count_specified, pgm, memtype);

	} else if (action == UNLOCK) {
		int bytes_to_write=part->option_bytes_size;

		if (part->read_out_protection_mode == ROP_UNKNOWN) spawn_error("No unlocking mode defined for this device. You may need to edit the file stm8.c");

		unsigned char *buf=malloc(bytes_to_write);
		if(!buf) spawn_error("malloc failed");

		if (part->read_out_protection_mode == ROP_STM8S) {
			for (int i=0; i<bytes_to_write;i++) {
				buf[i]=0;
				if ((i>0)&&((i&1)==0)) buf[i]=0xff;
			}
		}
		else if (part->read_out_protection_mode == ROP_STM8L) {
			buf[0] = 0xAA;
			for (int i=1; i<bytes_to_write;i++) {
				buf[i]=0;
			}
			buf[10] = 0x00;
		}
		else spawn_error("Unimplemented unlocking mode");
		/* flashing MCU */
		int sent = pgm->write_range(pgm, part, buf, start, bytes_to_write, memtype);
		if(pgm->reset) {
			// Restarting core (if applicable)
			pgm->reset(pgm);
		}
		fprintf(stderr, "Unlocked device. Option bytes reset to default state.\n");
		fprintf(stderr, "Bytes written: %d\n", sent);
	}
	return(0);
}



const stm8_device_t * stm8_get_part(const char * optarg)
{

   return get_part(optarg);

}
