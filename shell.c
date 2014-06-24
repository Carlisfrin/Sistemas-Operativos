#include <u.h>
#include <libc.h>
#include <bio.h>

static void
usage(char* mynameis)
{
	print("usage: %s\n", mynameis);
	exits("usage");
}

char*
formaruta(char* actual)
{

	return smprint("/bin/%s", actual);
}

int
builtin(char* cmd[10], int cuanto)
{
	char* casa;

	if (!strcmp(cmd[0], "cd"))
		if (cuanto==2)
		{
			if (chdir(cmd[1])<0)
				sysfatal("chdir: %r");
		}
		else
		{
			casa=getenv("home");
			if (chdir(casa)<0)
				sysfatal("chdir: %r");
		}
	return (!strcmp(cmd[0], "exit"));
}

int
haystr(char* cadena, char* s)
{
	return (0!=strstr(cadena, s));
}

void
creaproceso(int ntubos, int tubos[40][2], int nproceso, char* orden[10], char* archin, char* archout)
{
	int l, fdin=0; fdout=1;

	if (archin)
		fdin=open(archin, OREAD);
	if (archout)
		fdout=create(archout, OWRITE, 0664);
	if (fdin<0)
		sysfatal("open: %r");
	if (fdout<0)
		sysfatal("create: %r");
	switch(fork())
	{
	case -1:
		sysfatal("fork: %r");
	case 0:
		if (ntubos>0)
		{
			if (nproceso==0)
			{
				dup(tubos[nproceso][1], 1);
				if (archin)
					dup(fdin, 0);
			}
			else if (nproceso==ntubos)
			{
				dup(tubos[nproceso-1][0], 0);
				if (archout)
					dup(fdout, 1);
			}
			else
			{
				dup(tubos[nproceso-1][0], 0);
				dup(tubos[nproceso][1], 1);
			}
			for (l=0; l<ntubos; l++)
				close(tubos[l][0]);
			if (archin)
				close(fdin);
		}
		else
		{
			if (archin)
				dup(fdin, 0);
			if (archout)
				dup(fdout, 1);
		}
		exec(orden[0], orden);
		sysfatal("exec: %r");
	}
	close(tubos[nproceso][1]);
	if (archin)
		close(fdin);
	if (archout)
		close(fdout);
}

void
cierrapipes(int maximo, int tuberias[40][2])
{
	int q;
	
	for (q=0; q<maximo; q++)
		if (pipe(tuberias[q])<0)
			sysfatal("pipe: %r");
}

int
esenv(char* word)
{
	return word[0]=='$';
}

char*
variable(char* var)
{
	char* v[2];
	gettokens(var, v, 2, "$");
	return getenv(v[0]);
}

int
cambiaenv(char* com)
{
	char* nombre[2];
	char* valor[2];
	gettokens(com, nombre, 2, "=");
	tokenize(nombre[1], valor, 2);
	return putenv(nombre[0], valor[0]);
}

int
procesacmd(char* texto[10], int ncmd)
{
	int hayin, hayout, seguimos, hayalgo, longcmd, logulti=0, itera, pipes[40][2], w;
	char* comando[10], *fichin=nil, *fichout=nil, *ultimo[10], *penultimo[10], *antepenultimo[10], *final[10], *fich[10];
	
	creapipes(ncmd-1, pipes);
	seguimos=haystr(texto[ncmd-1], "&");
	hayin=haystr(texto[ncmd-1], "<");
	hayout=haystr(texto[ncmd-1], ">");
	hayalgo=hayin || hayout || seguimos;
	if (seguimos)
	{
		fichin="/dev/null";
		gettokens(texto[ncmd-1], antepenultimo, 2, "&");
	}
	if (hayout)
	{
		if (seguimos)
			gettokens(antepenultimo[0], penultimo, 2, ">");
		else
			gettokens(texto[ncmd-1], penultimo, 2, ">");
		tokenize(penultimo[1], fich, 2);
		fichout=fich[0];
	}
	if (hayin)
	{
		if (hayout)
			gettokens(penultimo[0], ultimo, 2, "<");
		else if (seguimos)
			gettokens(antepenultimo[0], ultimo, 2, "<");
		else
			gettokens(texto[ncmd-1], ultimo, 2, "<");
		tokenize(ultimo[1], fich, 2);
		fichin=fich[0];
		longulti=tokenize(ultimo[0], final, 10);
	}
	if (!hayin && hayout)
		longulti=tokenize(penultimo[0], final, 10);
	if (!hayin && !hayout && seguimos)
		longulti=tokenize(antepenultimo[0], final, 10);
	if (haystr(texto[0], "="))
	{
		if (cambiaenv(texto[0]))
			sysfatal("putenv: %r");
	}
	else
	{
		for (itera=0; itera<ncmd; itera++)
		{
			if (!(itera==ncmd-1 && (hayalgo)))
				longcmd=tokenize(texto[itera], comando, 10);
			else
			{
				longcmd=longulti;
				for (w=0; w<longulti; w++)
					comando[w]=final[w];
			}
			for (w=0; w<longcmd; w++)
				if (esenv(comando[w]))
					comando[w]=variable(comando[w]);
			if ((itera==0) && builtin(comando, longcmd))
				return 1;
			if (strcmp(comando[0], "cd"))
			{
				if (access(comando[0], AEXEC))
					comando[0]=formaruta(comando[0]);
				comando[longcmd]=nil;
				creaproceso(ncmd-1, pipes, itera, comando, fichin, fichout);
			}
		}
		if (!seguimos)
			for (itera=0; itera<ncmd; itera++)
				waitpid();
	}
	return 0;
}

void
readentrada(void)
{
	Biobuf* bin;
	char* line, *comandos[10];
	int szarg;

	bin=Bopen("/fd/0", OREAD);
	if (bin==nil)
		sysfatal("bopen: %r");
	while (line=Brdstr(bin, '\n', 0))
	{
		szarg=gettokens(line, comandos, 10, "|");
		if (procesacmd(comandos, szarg))
			break;
		free(line);
	}
	Bterm(bin);
}

void
main(int argc, char* argv[])
{
	if (argc==1)
		readentrada();
	else
		usage(argv[0]);
	exits(nil);
}