/* utility definitions for RTD Serial Recovery (rtdsr)
*
* copyright (c) 2011 Pete B. <xtreamerdev@gmail.com>
*
* based on util.h from Realtek bootloader set_pll, not copyrighted
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
#pragma once
#ifndef __SERIAL_UTIL_H__
#define __SERIAL_UTIL_H__

#define SERIAL_MODE_DEFAULT 0
#define SERIAL_MODE_QUASAR 1

struct serial_serial
{

   void * m_popaque;
   const char * m_pszPort;
   int  m_iMode;

};

#define REG8( addr )    (*(volatile u8 *) (addr))
#define REG16( addr )      (*(volatile u16 *)(addr))
#define REG32( addr )      (*(volatile u32 *)(addr))
#define REG64( addr )      (*(volatile u64 *)(addr))


/************************************************************************
*  Public function
************************************************************************/
void serial_sync();

void serial_write(struct serial_serial * pserial, u8 *p_param);
int  serial_read(struct serial_serial * pserial);

void serial_msleep(unsigned long ms);
void serial_ssleep(unsigned long seconds);
void serial_putchar(struct serial_serial * pserial, int c);
int  serial_getchar(struct serial_serial * pserial, int timeout);
void serial_putc(struct serial_serial * pserial, void* p, char c);
int  serial_getchar(struct serial_serial * pserial, int timeout);
void serial_memset(void *dst, UINT8 value, UINT32 size);
void serial_memcpy(void *dst, void *src, UINT32 size);
size_t serial_strlen(const char* s);
int serial_strncmp(const char *s1, const char *s2, size_t n);
char *serial_strncpy(char *dest, const char *src, size_t n);
unsigned long serial_strtoul(const char *nptr, char **endptr, int base);

#endif // __SERIAL_UTIL_H__
