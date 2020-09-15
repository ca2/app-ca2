#include "pgm.h"
#include <stdio.h>


int stm8_programmer_memtype(unsigned int * pstart, memtype_t memtype, int * pstart_addr_specified, int * pbytes_count, bool bytes_count_specified, const stm8_device_t * part)
{

         // Selecting start addr depending on
         // specified part and memtype
         switch(memtype) {
            case RAM:
                   if(!*pstart_addr_specified) {
                       *pstart = part->ram_start;
                   }
                   if(!bytes_count_specified || *pbytes_count > part->ram_size) {
                       *pbytes_count = part->ram_size;
                   }
                   //fprintf(stderr, "Determine RAM area\r\n");
               break;
            case EEPROM:
                   if(!*pstart_addr_specified) {
                       *pstart = part->eeprom_start;
                   }
                   if(!bytes_count_specified || *pbytes_count > part->eeprom_size) {
                      *pbytes_count = part->eeprom_size;
                   }
                   //fprintf(stderr, "Determine EEPROM area\r\n");
               break;
            case FLASH:
                   if(!*pstart_addr_specified) {
                       *pstart = part->flash_start;
                   }
                   if(!bytes_count_specified || *pbytes_count > part->flash_size) {
                       *pbytes_count = part->flash_size;
                   }
                   //fprintf(stderr, "Determine FLASH area\r\n");
               break;
            case OPT:
                   if(!*pstart_addr_specified) {
                       *pstart = 0x4800;
                   }
                   size_t opt_size = (part->flash_size <= 8*1024 ? 0x40 : 0x80);
                   if(!bytes_count_specified || *pbytes_count > opt_size) {
                       *pbytes_count = opt_size;
                   }
                   //fprintf(stderr, "Determine OPT area\r\n");
                   break;
         }
         *pstart_addr_specified = true;

   return 1;

}



