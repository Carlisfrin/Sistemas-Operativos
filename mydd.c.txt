#include <u.h>
#include <libc.h>

static void
usage(void)
{
	print("Uso: mydd [origen] [destino] [offset] [size]\n");
	exits("usage");
}

char*
leefichero(char* fichero, int delay, int cantidad)
{
	char* buff=malloc(cantidad);
	int nr;
	int fd;
	
	fd=open(fichero, OREAD);
	if (fd==-1)
		sysfatal("El fichero %s no se pudo abrir: %r", fichero);
	nr=pread(fd, buff, cantidad, delay);
	if (nr==-1)
		sysfatal("No se pudo leer: %r");
	close(fd);
	return buff;
}

void
escribefichero(char* fichero, char* datos, int cantidad)
{
	int fd;
	int nr;

	fd=open(fichero, OWRITE);
	if (fd==-1)
		sysfatal("\nEl fichero %s no se pudo abrir: %r", fichero);
	nr=write(fd, datos, cantidad);
	if (nr==-1)
		sysfatal("No se pudo escribir: %r");
	close(fd);
}

char*
leeteclado(int cantidad)
{
	char* buff=malloc(cantidad);
	int nr=0;
	
	nr=read(nr, buff, cantidad);
	if (nr==0)
		sysfatal("No se pudo leer: %r");
	return buff;
}

void
escribeteclado(char* datos, int cantidad)
{
	int fd=1;
	int nr;

	print("\n");
	nr=write(fd, datos, cantidad);
	print("\n");
	if (nr==-1)
		sysfatal("No se pudo escribir: %r");
	close(fd);
}

void
main(int argc, char* argv[])
{
	if (argc!=5)
		usage();
	char* inf=argv[1];
	char* outf=argv[2];
	char* off=argv[3];
	char* sz=argv[4];
	int offset=atoi(off);
	int size=atoi(sz);
	char* leido=malloc(size);
	
	if (leido==nil)
		sysfatal("No se pudo reservar memoria: %r");
	if (strlen(inf)==1 && inf[0]=='-')
		leido=leeteclado(size);
	else
		leido=leefichero(inf, offset, size);
	if (strlen(outf)==1 && outf[0]=='-')
		escribeteclado(leido, size);
	else
		escribefichero(outf, leido, size);
	free(leido);
	exits(nil);
}