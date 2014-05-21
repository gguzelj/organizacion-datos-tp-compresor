CFLAGS=-Isrc -Iscr/utils
OBJETOS=main.o src/Compresor.o src/DMC.o src/Estado.o src/FileManager.o src/CompresorAritmetico.o
FUENTES=main.c src/Compresor.c src/DMC.c src/Estado.c src/FileManager.c src/CompresorAritmetico.c

TPCompilador: $(OBJETOS) 
	g++ $(OBJETOS) -o TPGrupo

depend: 
	makedepend $(CFLGAS) $(FUENTES)
