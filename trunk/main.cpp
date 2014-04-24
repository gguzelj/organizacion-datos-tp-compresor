#include <iostream>
#include <stdio.h>

#include "src/DMC.h"
#include "src/Estado.h"

using namespace std;

int main()
{
    DMC *dmc = new DMC();

    int* frecuencias;

    Estado *est = new Estado();
    Estado *sig00 = new Estado();
    Estado *sig01 = new Estado();
    Estado *sig10 = new Estado();
    Estado *sig11 = new Estado();

    cout << sig00 << endl;
    cout << sig01 << endl;
    cout << sig10 << endl;
    cout << sig11 << endl;


    est->setEstadoSiguiente(BITS_00, sig00);
    est->setFrecuencia(BITS_00, 14);

    est->setEstadoSiguiente(BITS_01, sig01);
    est->setFrecuencia(BITS_01, 16);

    est->setEstadoSiguiente(BITS_10, sig10);
    est->setFrecuencia(BITS_10, 18);

    est->setEstadoSiguiente(BITS_11, sig11);
    est->setFrecuencia(BITS_11, 20);


    cout << est->getEstadoSiguiente(BITS_00) << endl;
    cout << est->getEstadoSiguiente(BITS_01) << endl;
    cout << est->getEstadoSiguiente(BITS_10) << endl;
    cout << est->getEstadoSiguiente(BITS_11) << endl;

    frecuencias = est->getFrecuencias();

    cout << frecuencias[BITS_00] << endl;
    cout << frecuencias[BITS_01] << endl;
    cout << frecuencias[BITS_10] << endl;
    cout << frecuencias[BITS_11] << endl;

}
