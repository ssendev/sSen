
#include <stddef.h>
#include <stdio.h>
#include <print.h>
#include <string.h>
#include <io.h>
#include <acpi.h>
#include <stdarg.h>
#include <time.h>


typedef struct aout_symbol_table
     {
       unsigned long tabsize;
       unsigned long strsize;
       unsigned long addr;
       unsigned long reserved;
     } aout_symbol_table_t;

typedef struct elf_section_header_table
     {
       unsigned long num;
       unsigned long size;
       unsigned long addr;
       unsigned long shndx;
     } elf_section_header_table_t;
     
typedef struct memory_map
     {
       unsigned long size;
       unsigned long base_addr_low;
       unsigned long base_addr_high;
       unsigned long length_low;
       unsigned long length_high;
       unsigned long type;
     } memory_map_t;

typedef struct multiboot_info
     {
       unsigned long flags;
       unsigned long mem_lower;
       unsigned long mem_upper;
       unsigned long boot_device;
       unsigned long cmdline;
       unsigned long mods_count;
       unsigned long mods_addr;
       union
       {
         aout_symbol_table_t aout_sym;
         elf_section_header_table_t elf_sec;
       } u;
       unsigned long mmap_length;
       unsigned long mmap_addr;
     } multiboot_info_t;

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

int main(int magic, char **addr)
{
	initScreen();
	initAcpi();
	//initGdt();


//Memory
//	memory size (see INT 12,INT 15/AH=88h,INT 15/AX=E820h)
//	SDRAM/DIMM configuration data (see #I0047 at I2C A0h)

multiboot_info_t *mbi = (multiboot_info_t *) addr;

if (CHECK_FLAG (mbi->flags, 0))
	kprintf("mem lower: %8x, mem upper: %8x\n", mbi->mem_lower, mbi->mem_upper);

char *memtype[] = {"0x00", "RAM", "SYSTEM", "ACPI", "ACPI-2"};

if (CHECK_FLAG (mbi->flags, 6))
         {
           memory_map_t *mmap;
     
           kprintf ("mmap_addr = 0x%x, mmap_length = 0x%x\n", (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);

           for (mmap = (memory_map_t *) mbi->mmap_addr;
                (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
                mmap = (memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size)))

             kprintf (" size = 0x%x, base_addr = 0x%x%8x,"
                     " length = 0x%0x%8x, type = %s\n",
                     (unsigned) mmap->size,
                     (unsigned) mmap->base_addr_high,
                     (unsigned) mmap->base_addr_low,
                     (unsigned) mmap->length_high,
                     (unsigned) mmap->length_low,
                     (unsigned) memtype[mmap->type]);
         }





	setattr(BLACK, WHITE);
	kprintf("welcome to sSen");
	setattr(BLACK, GREY);
	kprintf("...\n");

	setattr(BLACK, DRED);
	kprintf("poweroff\n");
	setattr(BLACK, YELLOW);
	int i=80;
	while(i-->0) {
		sleep(30000/80);
		kprintf(".");
	}
	acpiPowerOff();
	setattr(BLACK, GREY);

	kprintf("\nsSen load complete\n");
	for (;;);
}



