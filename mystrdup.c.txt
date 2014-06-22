#include <u.h>
#include <libc.h>

char*
copia(char* s0)
{
	char* espacio;
	long sz=strlen(s0);
	
	espacio=malloc(sz);
	while (*s0++=*espacio++)
		*espacio=*s0;
	return espacio;
}

void
main(int, char* argv[])
{
	char* str0=argv[1];
	char* str1;

	print("Direccion de memoria inicial: %p\n", str0);
	str1=copia(str0);
	print("Direccion de memoria final: %p\n", str1);

	exits(nil);
}