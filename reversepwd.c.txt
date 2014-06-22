#include <u.h>
#include <libc.h>

typedef struct Nodo Nodo;
	struct Nodo{
		Nodo* prev;
		char* str;
	};

Nodo*
pop(Nodo* nodulo)
{
	print("%s", (*nodulo).str);
	nodulo=(*nodulo).prev;
	return nodulo;
}

Nodo*
push(Nodo* node, char* element)
{
	Nodo* newnode;
	newnode=malloc(sizeof(Nodo));
	(*newnode).prev=node;
	(*newnode).str=strdup(element);
	return newnode;
}

void
manejapila(char* ruta)
{
	Nodo* pila;
	char palabra[10];
	int i;
	int pos=0;
	int sz=strlen(ruta);
	char barramala='/';
	char barrabuena='\\';

	print("%s\n", ruta);
	pila=malloc(sizeof(Nodo));
	(*pila).prev=nil;
	(*pila).str=" ";
	for (i=0;i<sz;i++)
	{
		if (ruta[i]==barramala)
		{
			if (pos)
			{
				palabra[pos]=barrabuena;
				pila=push(pila, palabra);
				pos=0;
				memset(palabra, 0, sizeof(palabra));
			}
		}
		else
		{
			palabra[pos]=ruta[i];
			pos++;
		}
	}
	palabra[pos]=barrabuena;
	pila=push(pila, palabra);
	while ((*pila).prev != nil)
		pila=pop(pila);
	free(pila);
}

void
main(int, char*)
{
	char directorio[1024];
	getwd(directorio, 1024);
	manejapila(directorio);
	exits(nil);
}