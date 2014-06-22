#include <u.h>
#include <libc.h>

void
imprimetxt(char* fichero, int tamanobytes)
{
	char* buf=malloc(tamanobytes);
	int fd;

	fd = open(fichero, OREAD);
	if (fd<0)
		sysfatal("open: %r");
	if (read(fd, buf, tamanobytes)<1)
		sysfatal("read: %r");
	close(fd);
	if (write(1, buf, tamanobytes)<1)
		sysfatal("write: %r");
	free(buf);
}

char*
estxt(char* nombre)
{
	return strstr(nombre, ".txt");
}

int
kilobytizer(int bytes)
{
	if (bytes%1024)
		return bytes/1024+1;
	else
		return bytes/1024;
}

void
printfich(char* carpeta, Dir* archivo, int pos)
{
	int tamano=kilobytizer(archivo[pos].length);
	if (!(strcmp(carpeta, ".")))
		print("%5d %s\n", tamano, archivo[pos].name);
	else
		print("%5d %s/%s\n", tamano, carpeta, archivo[pos].name);
}

char*
formaruta(char* anterior, char* actual)
{
	char* duplicado=malloc(1024);
	if (strcmp(anterior, "."))
	{
		duplicado=strcpy(duplicado, anterior);
		duplicado=strcat(duplicado, "/");
		duplicado=strcat(duplicado, actual);
	}
	else
		duplicado=actual;
	return duplicado;
}

void
leedirectorio(char* direct)
{
	Dir* dents;
	int ndents, fd, j;
	char* nuevodir, *nuevotxt;

	fd=open(direct, OREAD);
	if (fd<0)
		sysfatal("open: %r");
	for(;;)
	{
		ndents=dirread(fd, &dents);
		if (ndents==0)
			break;
		for(j=0; j<ndents; j++)
		{
			printfich(direct, dents, j);
			if (estxt(dents[j].name))
			{
				nuevotxt=formaruta(direct, dents[j].name);
				imprimetxt(nuevotxt, dents[j].length);
			}
			else if (dents[j].qid.type & QTDIR)
			{
				nuevodir=formaruta(direct, dents[j].name);
				leedirectorio(nuevodir);
			}
		}
		free(dents);
	}
}

void
main(int argc, char* argv[])
{
	int i;
	
	if (argc>1)
		for (i=1; i<argc; i++)
			leedirectorio(argv[i]);
	else
		leedirectorio(".");
	exits(nil);
}