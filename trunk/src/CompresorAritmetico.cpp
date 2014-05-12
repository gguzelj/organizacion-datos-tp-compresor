#include "CompresorAritmetico.h"
#include <stdio.h>

CompresorAritmetico::CompresorAritmetico(int cantidadDeCaracteresDistintos)
{

    techo = TECHO;

    piso = PISO;

    cantidadDeCaracteresVistos = cantidadDeCaracteresDistintos;

}

void CompresorAritmetico::comprimirCaracterConFrecuencias(Caracter* caracterAcomprimir, std::list<Caracter*>& frecuenciasDeCaracteres)
{

    unsigned long int pisoDelIntervalo = piso;

    unsigned long int techoDelIntervalo;

    unsigned long int unRango;

    unsigned long int nuevoPiso = PISO;

    unsigned long int nuevoTecho = TECHO;

    std::list<Caracter*>::iterator iterador = frecuenciasDeCaracteres.begin();

    while(iterador != frecuenciasDeCaracteres.end())
    {

        unRango = (((techo - piso) / cantidadDeCaracteresVistos) * ((*iterador)->getFrecuencia()));

        techoDelIntervalo = pisoDelIntervalo + unRango;

        if((*iterador)->esIgualAlCaracter(caracterAcomprimir))
        {

            nuevoPiso = pisoDelIntervalo;

            nuevoTecho = techoDelIntervalo;

        }

        eliminarIntervalos();

        intervalos.push_back(new Intervalo(&pisoDelIntervalo, &techoDelIntervalo, (*iterador)));

        pisoDelIntervalo = techoDelIntervalo;

        iterador++;

    }

    piso = nuevoPiso;

    techo = nuevoTecho;

    cantidadDeCaracteresVistos++;

    delete caracterAcomprimir;

}

unsigned long int CompresorAritmetico::getValorDeCompresionFinal()
{

    return piso;

}

void CompresorAritmetico::eliminarIntervalos()
{

    if(!(intervalos.empty()))
    {

        std::list<Intervalo*>::iterator iterador = intervalos.begin();

        while(iterador != intervalos.end())
        {

            delete (*iterador);

            iterador++;

        }

        intervalos.clear();

    }

}

CompresorAritmetico::~CompresorAritmetico()
{

    eliminarIntervalos();

}
