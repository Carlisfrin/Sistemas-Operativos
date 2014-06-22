#include <u.h>
#include <libc.h>

static void
usage(char* mynameis)
{
	print("Uso: %s [blocksize] [archivo1.gz] ... [archivoN.gz]\n", mynameis);
	exits("usage");
}

void
creaficheros(char* nombres[], int tope)
{
	int cr;
	int j;
	
	for(j=0; j<=tope; j++)
	{
		cr=create(nombres[j], OWRITE, 0664);
		if (cr<0)
			sysfatal("No se pudo crear el fichero: %r");
		close(cr);
	}
}

void
matahijos(int hijos[40][2], int numhijos)
{
	int son;
	for (son=0;son<numhijos; son++)
	{
		close(hijos[son][1]);
		wait();
	}
}

void
cierrapipes(int tuberias[40][2], int numtubos)
{
	int t;
	for(t=0; t<numtubos; t++)
		close(tuberias[t][0]);
}

void
demultiplexa(char* arch[], int sz, int max)
{
	char palabra[1024];
	int br, k, l, df, turno=0;
	int pipes[40][2];
	char* args[]={"/bin/gzip", "-c", nil};

	creaficheros(arch, max);
	for (k=0; k<=max; k++)
		if (pipe(pipes[k])<0)
			sysfatal("pipe: %r");
	for (k=0; k<=max; k++)
		switch(fork())
		{
		case -1:
			sysfatal("fork: %r");
		case 0:
			df=open(arch[k], OWRITE);
			dup(pipes[k][0], 0);
			dup(df, 1);
			for (l=0;l<max; l++)
			{
				close(pipes[l][0]);
				close(pipes[l][1]);
			}
			close(df);
			exec("/bin/gzip", args);
			sysfatal("exec: %r");
		}
	cierrapipes(pipes, max);
	for(;;)
	{
		br=read(0, palabra, sz);
		if (br<=0)
			break;
		write(pipes[turno][1], palabra, br);
		if (turno==max)
			turno=-1;
		turno++;
	}
	matahijos(pipes, max);
}

void
main(int argc, char* argv[])
{
	int size, i;
	char* fich[];
	
	if (argc<3)
		usage(argv[0]);
	size=atoi(argv[1]);
	for (i=2; i<argc; i++)
		fich[i-2]=argv[i];
	demultiplexa(fich, size, argc-3);
	exits(nil);
}