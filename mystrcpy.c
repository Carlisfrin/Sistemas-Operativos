#include <u.h>
#include <libc.h>

char*
copia(char* s0, char* s1)
{
	while (*s0++=*s1++)
		*s0=*s1;
	return s0;
}

void
main(int argc, char* argv[])
{
	char* str0;
	char* str1;

	ARGBEGIN{
	default:
		break;
	}ARGEND

	str0=*argv++;
	str1=*argv;
	print("%s\n", str0);
	str0=copia(str0, str1);
	print("%s\n", str0);

	exits(nil);
}