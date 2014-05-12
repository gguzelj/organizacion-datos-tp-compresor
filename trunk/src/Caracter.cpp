#include "Caracter.h"

Caracter::Caracter(char unSimbolo)
{
    simbolo = unSimbolo;

    frecuencia = MINIMA_FRECUENCIA;
}

char Caracter::getSimbolo()
{

    return simbolo;

}

unsigned long int Caracter::getFrecuencia()
{

    return frecuencia;

}

bool Caracter::esIgualAlCaracter(Caracter* otroCaracter)
{

    if(simbolo == otroCaracter->getSimbolo())return true;

    return false;

}

void Caracter::setFrecuencia(unsigned long int unaFrecuencia)
{

    frecuencia = unaFrecuencia;

}

Caracter::~Caracter()
{
    //dtor
}
