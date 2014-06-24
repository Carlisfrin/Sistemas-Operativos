#include <u.h>
#include <libc.h>

static void
usage(char* mynameis)
{
	print("Uso: %s [archivo1] [archivo2] ... [archivoN]\n", mynameis);
	exits("usage");
}

char*
sufijizer(char* original, char* sufijo)
{
	char* nuevo;
	nuevo=smprint("%s%s", original, sufijo);
	return nuevo;
}

void
cierrapipes(int tubo[2])
{
	close(tubo[0]);
	close(tubo[1]);
}

void
creautf(char* archivo)
{
	int fd[2];
	int cr;
	char* argtcs[]={"/bin/tcs", "-f", "8859-1", "-t", "utf", archivo, nil};
	char* argtr[]={"/bin/tr", "a-z", "A-Z", nil};
	char* archivoutf;

	if (pipe(fd)<0)
		sysfatal("pipe: %r");
	switch(fork())
	{
	case -1:
		sysfatal("fork: %r");
	case 0:
		archivoutf=sufijizer(archivo, ".utf");
		cr=create(archivoutf, OWRITE, 0664);
		dup(fd[0], 0);
		dup(cr, 1);
		close(cr);
		cierrapipes(fd);
		exec("/bin/tr", argtr);
		sysfatal("exec: %r");
	}
	cierrapipes(fd);
	waitpid();
	waitpid();
}

void
creahtml(char* archivo)
{
	int fd[2];
	int cr;
	char* argtcs[]={"/bin/tcs", "-f", "8859-1", "-t", "html", archivo, nil};
	char* archivohtml;
	char buf[1024];

	
	if (pipe(fd)<0)
		sysfatal("pipe: %r");
	switch(fork())
	{
	case -1:
		sysfatal("fork: %r");
	case 0:
		dup(fd[0], 1);
		dup(fd[1], 0);
		cierrapipes(fd);
		exec("/bin/tcs", argtcs);
		sysfatal("exec: %r");
		
	default:
		waitpid();
		archivohtml=sufijizer(archivo, ".html");
		if (read(fd[1], buf, 1024)<1)
			sysfatal("read: %r");
		cr=create(archivohtml, OWRITE, 0664);
		write(cr, buf, 1024);
		cierrapipes(fd);
		close(cr);
		cierrapipes(fd);
	}
}

void
main(int argc, char* argv[])
{
	int 1;
	
	if (argc==1)
		usage(argv[0]);
	for(i=1;i<argc;i++)
	{
		creahtml(argv[i]);
		creautf(argv[i]);
	}
	exits(nil);
}