#include "CompresorAritmetico.h"

/**
//---------------------------------------------------------------------------&
// P U B L I C
//---------------------------------------------------------------------------&
*/

//&---------------------------------------------------------------------------&
//& CompresorAritmetico: Constructor
//&---------------------------------------------------------------------------&
CompresorAritmetico::CompresorAritmetico(FileManagerInput *inPut, FileManagerOutput *outPut)
{
    piso = 0;
    techo = ~0;
    valor = 0;
    underflow = 0;
    contadorBits = 0;
    this->output = outPut;
    this->input = inPut;
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

//&---------------------------------------------------------------------------&
//& descomprimir:  Descomprimimos el archivo
//&             http://www.arturocampos.com/ac_arithmetic.html
//&---------------------------------------------------------------------------&
Direccion CompresorAritmetico::descomprimir(int* frecuencias)
{
    Direccion       bits;
    unsigned long   temp;
    unsigned long   rango = techo - piso + 1;
    Frecuencias     frecAcumuladas = {0,0,0,0};

    //Calculamos las frecuencias acumuladas para este estado
    for( register short dir = 0; dir <4 ; dir++)
        frecAcumuladas[dir] = frecuencias[dir] + frecAcumuladas[((dir - 1)<0)?0:(dir-1)];

    //Deshacemos los calculos del nuevo rango
    temp = ( ( (valor - piso) + 1) * frecAcumuladas[3] - 1 ) / rango;

    //Vemos en que intervalo cae el valor calculado
    for(bits = BITS_00 ; (unsigned long)frecAcumuladas[bits] > temp; bits++);

    //Actualizamos los valores de los intervalos
    techo = piso + rango * frecAcumuladas[bits] / frecAcumuladas[3] - 1;
    piso = piso + rango * ((bits-1 < 0)?0:(frecAcumuladas[bits-1])) / frecAcumuladas[3];

    //Una vez calculado el nuevo rango, debemos guardar los bits que converjan
    shiftYValidarUnderflow();

    return bits;
}

//&---------------------------------------------------------------------------&
//& prepararDescompresor:   Preparamos el descompresor aritmetico
//&---------------------------------------------------------------------------&
void CompresorAritmetico::prepararDescompresor()
{
    unsigned char byte;

    //Leemos los primeros 64bits del archivo
    for(register unsigned short i = 0; i<4; i++)
    {
        byte = input->leerBits(8);
        valor <<=8; valor |= byte;
    }

    return;
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
            continue;
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
//& shiftYValidarUnderflow: Luego de descomprimir el rango hacemos el shift
//&                         correspondiente de las variables y validamos el
//&                         underflow
//&---------------------------------------------------------------------------&
void CompresorAritmetico::shiftYValidarUnderflow()
{
    unsigned char byte;
    const unsigned short MSB = 0x8000;
    const unsigned short segundoMSB = 0x4000;
    const unsigned short ultimosBits = 0x3FFF;

    /**Este metodo es muy parecido a guardarBitsYValidarUnderflow()
    *   El tema es que aca no hay que guardar ningun bit en el archivo de salida
    *   sino que hay que hacer los shifts correspondientes y leer el/los bits necesarios
    *   del archivo comprimido. Falta terminar
    *   Analizar posibilidad de usar un puntero a funcion, y juntar los dos metodos en uno
    *   solo
    */

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
            continue;
        }

        //Los MSB coinciden, entonces los guardamos en el archivo/
        if ((techo & MSB) == (piso & MSB))
        {
            //guardarBit(piso & MSB); => No hay que guardar ningun bit..

            //Sacamos los bits en underflow/
            while (underflow > 0)
            {
                //guardarBit((techo & MSB) == 0); => No hay que guardar ningun bit..
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

        //Leemos 1 bit del archivo comprimido
        byte = input->leerBits(1);
        valor <<=1; valor |= (byte & MSB)?1:0;
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
