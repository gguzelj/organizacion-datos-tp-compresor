#include "Intervalo.h"

Intervalo::Intervalo(unsigned long int* unPiso, unsigned long int* unTecho, Caracter* unCaracter)
{
    piso = *unPiso;

    techo = *unTecho;

    caracter = unCaracter;
}

unsigned long int Intervalo::getPiso()
{

    return piso;

}

unsigned long int Intervalo::getTecho()
{

    return techo;

}

Caracter* Intervalo::getCaracter()
{

    return caracter;

}

Intervalo::~Intervalo()
{

    delete caracter;

}
