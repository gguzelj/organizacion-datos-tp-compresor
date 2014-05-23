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
    unsigned long rango = techo - piso + 1;
    Frecuencias   frecAcumuladas = {0,0,0,0};

    //Calculamos las frecuencias acumuladas para este estado
    for( register short dir = 0; dir <4 ; dir++)
        frecAcumuladas[dir] = frecuencias[dir] + frecAcumuladas[((dir - 1)<0)?0:(dir-1)] ;

    //Calculamos el nuevo rango (frecAcumuladas[3] = Total Acumulado)
    techo = piso + rango * frecAcumuladas[bits] / frecAcumuladas[3] - 1;
    piso = piso + rango * ((bits-1 < 0)?0:(frecAcumuladas[bits-1])) / frecAcumuladas[3];

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

    for (;;)
    {
        //Validamos underflow
        if ( ( (techo & MSB) != (piso & MSB) ) &&
             ( (piso & segundoMSB) && !(techo & segundoMSB) ) )
        {
            //Eliminimo el segundoMSB, y corro todos los bits
            piso = (piso & MSB) | ((piso & ultimosBits) << 1 );
            techo = (techo & MSB) | ((techo & ultimosBits) << 1 );
            techo |= 1;

            //Incremento el contador de underflow
            underflow++;
        }

        //Los MSB coinciden, entonces los guardamos en el archivo/
        if ((techo & MSB) == (piso & MSB))
        {
            guardarBit(piso & MSB);

            //Sacamos los bits en underflow/
            while (underflow > 0)
            {
                guardarBit((techo & MSB) == 0);
                underflow--;
            }
        }
        else
        {
            //Salimo jugando...
            return;
        }

        //Shift de variables/
        piso <<= 1;
        techo <<= 1;
        techo |= 1;
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
