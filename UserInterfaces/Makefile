all: VM FrontC compiler

VM: UI-MaquinaVirtual.c
	gcc $( pkg-config --cflags gtk4 ) -o VM UI-MaquinaVirtual.c $( pkg-config --libs gtk4 )  

	
FrontC: UI-Compilador.c
	gcc $( pkg-config --cflags gtk4 ) -o FrontC UI-Compilador.c $( pkg-config --libs gtk4 )  
	
compiler: Compilador.c
	gcc Compilador.c -o compiler
