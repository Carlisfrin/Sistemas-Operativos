#include <u.h>
#include <libc.h>
#include <bio.h>

static void
usage(char* mynameis)
{
	print("usage: %s\n", mynameis);
	exits("usage");
}

void
readentrada(void)
{
	Biobuf bin;
	char* line, *comandos[10];
	int szarg, i, max=0;

	Binit(&bin, 0, OREAD);
	if (&bin==nil)
		sysfatal("bopen: %r");
	while (line=Brdstr(&bin, '\n', 0))
	{
		szarg=tokenize(line, comandos, 10);
		for (i=0; i<szarg; i++)
			print("%d: [%s]\n", i, comandos[i]);
		if (max==5)
			break;
		max++;
		free(line);
	}
	Bterm(&bin);
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