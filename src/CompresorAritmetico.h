#ifndef COMPRESORARITMETICO_H
#define COMPRESORARITMETICO_H
#define TECHO 1000000000
#define PISO 0
#include <list>
#include "Intervalo.h"

class CompresorAritmetico
{
    public:

    /**Metodos Publicos*/
        CompresorAritmetico(int cantidadDeCaracteresDistintos);
        void comprimirCaracterConFrecuencias(Caracter* caracterAcomprimir, std::list<Caracter*>& frecuenciasDeCaracteres);
        unsigned long int getValorDeCompresionFinal();
        virtual ~CompresorAritmetico();

    protected:
    private:

    /**Atributos*/
        /*std::list <Caracter> caracteresAcomprimir;*/
        std::list <Intervalo*> intervalos;
        unsigned long int techo;
        unsigned long int piso;
        int cantidadDeCaracteresVistos;

    /**Metodos Privados*/
        void eliminarIntervalos();

};

#endif // COMPRESORARITMETICO_H
