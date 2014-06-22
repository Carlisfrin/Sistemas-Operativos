#include <u.h>
#include <libc.h>

int
longitud(char s0[])
{
	int sz=0;
	
	while (s0[sz] != '\0')
		sz+=1;
	return sz;
}

void
main(int, char* argv[])
{
	char* str0=argv[1];
	int tamano;

	print("%s\n", str0);
	tamano=longitud(str0);
	print("%d\n", tamano);

	exits(nil);
}