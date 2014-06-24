#include <u.h>
#include <libc.h>
#include <bio.h>

char*
formaruta(char* actual)
{
	char* duplicado=malloc(1024);
	duplicado=strcpy(duplicado, "/bin/");
	duplicado=strcat(duplicado, actual);
	return duplicado;
}

void
ejecuta(char* argus[])
{
	Waitmsg* espera;
	char* ruta;

	switch(fork())
	{
	case -1:
		sysfatal("fork: %r");
	case 0:
		ruta=formaruta(argus[0]);
		argus[0]=ruta;
		exec(ruta, argus);
		sysfatal("exec: %r");
	default:
		espera=wait();
		print("Tiempo transcurrido: %uld milisegundos\n", espera->time[2]);
		free(espera);
	}
}

void
readarchivo(char* archivo)
{
	Biobuf* bin;
	char* line;
	char* argumentos[1024];
	int szarg;

	bin=Bopen(archivo, OREAD);
	if (bin==nil)
		sysfatal("bopen: %r");
	while (line=Brdstr(bin, '\n', 0))
	{
		szarg=tokenize(line, argumentos, 10);
		argumentos[szarg]=nil;
		ejecuta(argumentos);
	}
	Bterm(bin);
}

void
readestandar(int fd)
{
	Biobuf bin;
	char* line;
	int szarg;
	char* argumentos[1024];

	Binit(&bin, fd, OREAD);
	if (&bin==nil)
		sysfatal("bopen: %r");
	while (line=Brdstr(&bin, '\n', 0))
	{
		szarg=tokenize(line, argumentos, 10);
		argumentos[szarg]=nil;
		ejecuta(argumentos);
	}
	Bterm(&bin);
}

void
main(int argc, char* argv[])
{
	if (argc==1)
		readestandar(0);
	else
		readarchivo(argv[1]);
	exits(nil);
}