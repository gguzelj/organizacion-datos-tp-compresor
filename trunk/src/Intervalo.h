#ifndef INTERVALO_H
#define INTERVALO_H
#include "Caracter.h"

class Intervalo
{
    public:

    /**Metodos Publicos*/
        Intervalo(unsigned long int* unPiso, unsigned long int* unTecho, Caracter* unCaracter);
        unsigned long int getPiso();
        unsigned long int getTecho();
        Caracter* getCaracter();
        virtual ~Intervalo();

    protected:
    private:

    /**Atributos*/
        unsigned long int piso;
        unsigned long int techo;
        Caracter* caracter;
};

#endif // INTERVALO_H
