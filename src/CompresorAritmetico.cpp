#include "CompresorAritmetico.h"

/**
//---------------------------------------------------------------------------&
// P U B L I C
//---------------------------------------------------------------------------&
*/

//&---------------------------------------------------------------------------&
//& CompresorAritmetico: Constructor
//&---------------------------------------------------------------------------&
CompresorAritmetico::CompresorAritmetico()
{
    piso = 0;
    techo = ~0;
    underflow = 0;
}

//&---------------------------------------------------------------------------&
//& ~CompresorAritmetico: Destructor
//&---------------------------------------------------------------------------&
CompresorAritmetico::~CompresorAritmetico()
{
    //dtor
}

//&---------------------------------------------------------------------------&
//& comprimir:  Comprimimos los bits recibidos
//&             http://www.arturocampos.com/ac_arithmetic.html
//&---------------------------------------------------------------------------&
void CompresorAritmetico::comprimir(Direccion bits, int* frecuencias)
{
    unsigned short nuevoTecho = techo;
    unsigned short nuevoPiso = piso;
    unsigned long rango = techo - piso + 1;
    unsigned long totalFrecuencias = 0;

    //Sumamos el total de todas las posibilidades
    totalFrecuencias += frecuencias[BITS_00] + frecuencias[BITS_01];
    totalFrecuencias += frecuencias[BITS_10] + frecuencias[BITS_11];

    //Calculamos el nuevo rango
    for( register short i = 0; i <4 ; i++)
    {
        nuevoTecho = nuevoPiso + ( ( rango * frecuencias[i] ) / totalFrecuencias );

        //Dejamos de comprimir en caso de encontrar el caracter a comprimir
        if(bits == i) break;

        nuevoPiso = nuevoTecho;
    }

    techo = nuevoTecho - 1;
    piso = nuevoPiso;

    //Una vez calculado el nuevo rango, debemos guardar los bits que converjan
    guardarBitsComprimidos();

    piso = piso;

}

/**
//---------------------------------------------------------------------------&
// P R I V A T E
//---------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------&
//& guardarBitsComprimidos: Luego de comprimir el rango buscamos los bits que
//&                         puedan ser almacenados en el archivo de salida
//&---------------------------------------------------------------------------&
void CompresorAritmetico::guardarBitsComprimidos()
{
    unsigned short MSB = 0x8000;


    while( (techo & MSB) == (piso & MSB))
    {
        techo <<=1; techo |= 1;
        piso <<=1;
    }
}
