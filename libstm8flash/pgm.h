#ifndef __PGM_H
#define __PGM_H

//#if defined(RASPBIAN)
 #include <libusb-1.0/libusb.h>
//#else
// #include <libusb.h>
//#endif

#include "stm8.h"
#include "libespstlink.h"

typedef enum {
    UNKNOWN,
    RAM,
    EEPROM,
    FLASH,
    OPT,
} memtype_t;

typedef enum {
    NONE = 0,
    READ,
    WRITE,
    VERIFY,
    UNLOCK
} action_t;

typedef enum {
	STLinkV1,
	STLinkV2,
	STLinkV21,
	STLinkV3,
	ESP_STLink
} programmer_type_t;

typedef struct programmer_s {
	/* Info */
	const char *name;
	programmer_type_t type;
	unsigned int usb_vid;
	unsigned int usb_pid;

	/* Methods */
	bool (*open) (struct programmer_s *pgm);
	void (*close) (struct programmer_s *pgm);
	void (*reset) (struct programmer_s *pgm);
	int (*read_range) (struct programmer_s *pgm, const stm8_device_t *device, unsigned char *buffer, unsigned int start, unsigned int length);
	int (*write_range) (struct programmer_s *pgm, const stm8_device_t *device, unsigned char *buffer, unsigned int start, unsigned int length, const memtype_t memtype);

	/* Private */
	libusb_device_handle *dev_handle;
	libusb_context *ctx;

	unsigned int msg_count; // debugging only
	unsigned int out_msg_size; // stlink/stlinkv2

	/* Data for espstlink module. */
        espstlink_t * espstlink;
	const char *port;
} programmer_t;

typedef bool (*pgm_open_cb)(programmer_t *);
typedef void (*pgm_close_cb)(programmer_t *);
typedef int (*pgm_read_range_cb)(programmer_t *, unsigned char *, unsigned int, unsigned int);
typedef int (*pgm_write_range_cb)(programmer_t *, unsigned char *, unsigned int, unsigned int);

programmer_t * stm8_get_programmers();

programmer_t * stm8_get_programmer(const char * psz);

memtype_t stm8_get_memtype(const char * optarg);

int stm8_programmer_memtype(unsigned int * pstart, memtype_t memtype, int * pstart_addr_specified, int * pbytes_count, bool bytes_count_specified, const stm8_device_t * part);

int stm8_write(int * psent, const char * filename, fileformat_t fileformat, int bytes_count,
const stm8_device_t * part, int start, bool bytes_count_specified, programmer_t * pgm,
memtype_t memtype);

int stm8_verify(char * pszMessage, int iCount, int * pverify, const char * filename, fileformat_t fileformat, int bytes_count,
const stm8_device_t * part, int start, bool bytes_count_specified, programmer_t * pgm,
memtype_t memtype);


int stm8_usb_init(char * szMessage, int iCount, programmer_t *pgm, int pgm_serialno_specified, char *pgm_serialno);


#endif
