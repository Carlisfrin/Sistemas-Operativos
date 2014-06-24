#include <u.h>
#include <libc.h>

char*
getvar(char* variable)
{
	return getenv(variable);
}

void
otra(char** lista)
{
	char* env;
	while (*lista != nil)
	{
		env=getvar(*lista);
		print("%s: ", *lista++);
		if (env != '\0')
			print("%s\n", env);
	}
}

void
main(int argc, char* argv[])
{
	int sflag=0;
	int uflag=0;
	int cflag=0;
	int vflag=0;
	int error=0;

	ARGBEGIN{
	case 's':
		sflag=1;
		break;
	case 'u':
		uflag=1;
		break;
	case 'c':
		cflag=1;
		break;
	case 'v':
		vflag=1;
		break;
	default:
		error=1;
		break;
	}ARGEND
	
	if (error)
		sysfatal("uso -> sysinfo -sucv [var1] ... [varN]");
	if (sflag)
		print("%s\n", getvar("sysname"));
	if (uflag)
		print("%s\n", getvar("user"));
	if (cflag)
		print("%s\n", getvar("cputype"));
	if (vflag)
		otra(argv);
	exits(nil);
}