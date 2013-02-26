#include "hexdump.h"
#include <stdio.h>
#include <string.h>
main() 
{

	char *c = "This is a \n \t  hexdump test";
	hexdump(c,strlen(c));

}
