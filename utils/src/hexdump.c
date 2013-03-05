/*
 * Copyright placeholder
 */

#include "hexdump.h"
void hexdump(void *buffer, unsigned int size) 
{

  unsigned char *buf = (unsigned char*)buffer;
  int i, j;

  for (i=0; i<size; i+=16) {
    printf("%08x  ", i);
    dump_hex(buf, i, size);
    dump_ascii(buf, i, size);
  }

}

/*
 * hexdump dumps two blocks of 8 bytes (hex equivalent) per line
 * The bytes are counted from "whence"
 *
 */

void dump_hex(unsigned char *buf, unsigned int whence, unsigned int size) 
{


      int i,j,offset;
      for (i=0; i<2; i++) {
	for (j=0; j<8; j++) {
	   offset = whence + i*8 + j;
	   if (offset < size) {
	      printf("%02x ", buf[offset]);
           }
	   else 
	      printf("   ");
	 }
	putchar(' ');
      }
}

/*
 * print the ASCII equivalent of the dump_hex function. If a character is
 * unprintable, print "." instead.
 */

void dump_ascii(unsigned char *buf, unsigned int whence, unsigned int size) 
{
	
	char c;
	int i;
	printf("|");
	for (i=0; i<16; i++)  {
	     if (i+whence < size) {
		c = buf[i+whence];
		putchar(isprint(c)?c:'.');	
	     }    
	}
	printf("|\n");
}
