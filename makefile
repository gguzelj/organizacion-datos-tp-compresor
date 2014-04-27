CFLAGS=-Isrc -Iscr/utils
OBJETOS=main.o src/DMC.o src/Estado.o
FUENTES=main.c src/DMC.c src/Estado.c

TPCompilador: $(OBJETOS) 
	g++ $(OBJETOS) -o TPCompilador 

depend: 
	makedepend $(CFLGAS) $(FUENTES)
