#include "CompresorAritmetico.h"

/**
//---------------------------------------------------------------------------&
// P U B L I C
//---------------------------------------------------------------------------&
*/

//&---------------------------------------------------------------------------&
//& CompresorAritmetico: Constructor
//&---------------------------------------------------------------------------&
CompresorAritmetico::CompresorAritmetico(FileManagerOutput *outPut)
{
    piso = 0;
    techo = ~0;
    underflow = 0;
    contadorBits = 0;
    this->output = outPut;
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

    /** EN UN MOMENTO TECHO Y PISO CONVERGEN DEMASIADO Y SE DEJA DE COMPRIMIR
    ANALIZAR!
    if( techo == 0x7fff && piso == 0x8000)
    {
        piso = 0;
        techo = ~0;
    }*/


    //Una vez calculado el nuevo rango, debemos guardar los bits que converjan
    guardarBitsYValidarUnderflow();
}

/**
//---------------------------------------------------------------------------&
// P R I V A T E
//---------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------&
//& guardarBitsYValidarUnderflow: Luego de comprimir el rango buscamos los bits
//&                               que puedan ser almacenados en el archivo de
//&                               salida
//&---------------------------------------------------------------------------&
void CompresorAritmetico::guardarBitsYValidarUnderflow()
{
    const unsigned short MSB = 0x8000;
    const unsigned short segundoMSB = 0x4000;
    const unsigned short ultimosBits = 0x3FFF;

    //Guardamos los bits que convergen
    while(true)
    {
        //MSB techo == MSB piso ?
        if((techo & MSB) == (piso & MSB))
        {
            //Output MSB piso
            guardarBit(piso & MSB);

            //Sacamos los bits en underflow
            while(underflow)
            {
                //output ~MSB piso
                guardarBit(~(piso & MSB));
                underflow--;
            }

            //SHIFT
            techo <<= 1; techo |= 1;
            piso <<= 1;
        }
        else
        {
            //Validamos por Underflow
            if( (piso & segundoMSB) & ~(techo & segundoMSB) )
            {
                //Eliminimo el segundoMSB, y corro todos los bits
                piso = (piso & MSB) | ((piso & ultimosBits) << 1 );
                techo = (techo & MSB) | ((techo & ultimosBits) << 1 );
                techo |= 1;

                //Incremento el contador de underflow
                underflow++;
            }
            else
            {
                return;
            }
        }
    }
}

//&---------------------------------------------------------------------------&
//& guardarBit: Metodo encargado de agrupar bits en bytes, y guardarlos en el
//&             archivo de salida
//&---------------------------------------------------------------------------&
void CompresorAritmetico::guardarBit(const unsigned short bit)
{
    contadorBits++;

    //Almacenamos el nuevo bit en el byteBuffer
    byteBuffer[8-contadorBits] = (bit)?1:0;

    //En caso de completar un byte entero, lo guardamos en el archivo
    if(contadorBits == 8)
    {
        contadorBits = 0;
        output->escribirByte(byteBuffer);
    }
}
