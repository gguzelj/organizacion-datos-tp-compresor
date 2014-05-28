#include "CompresorAritmetico.h"

/**
//----------------------------------------------------------------------------------------&
// P U B L I C
//----------------------------------------------------------------------------------------&
*/

//&---------------------------------------------------------------------------------------&
//& CompresorAritmetico: Constructor
//&---------------------------------------------------------------------------------------&
CompresorAritmetico::CompresorAritmetico(FileManagerInput *inPut, FileManagerOutput *outPut)
{
    this->output = outPut;
    this->input = inPut;
}

//&---------------------------------------------------------------------------------------&
//& ~CompresorAritmetico: Destructor
//&---------------------------------------------------------------------------------------&
CompresorAritmetico::~CompresorAritmetico()
{
}

//&---------------------------------------------------------------------------------------&
//& prepararCompresion:  Preparamos el compresor
//&---------------------------------------------------------------------------------------&
void CompresorAritmetico::prepararCompresion()
{
    piso = PISO;
    techo = TECHO;
    underflow = 0;
}

//&---------------------------------------------------------------------------------------&
//& comprimir:  Comprimimos los bits recibidos
//&             http://www.arturocampos.com/ac_arithmetic.html
//&---------------------------------------------------------------------------------------&
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

    for(;;)
    {
        if(techo < MEDIO)
            guardarBit(0);
        else if(piso >= MEDIO)
        {
            guardarBit(1);
            piso -= MEDIO;
            techo -= MEDIO;
        }
        else if( piso >= PRIMER_CUARTO && techo < TERCER_CUARTO)
        {
            underflow++;
            piso -= PRIMER_CUARTO;
            techo -= PRIMER_CUARTO;
        }
        else break;

        piso = 2*piso;
        techo = 2*techo+1;
    }
}

//&---------------------------------------------------------------------------------------&
//& terminarCompresion: Terminamos la compersion. Seleccionamos el intervalo
//&                     correspondiente al caracter y sacamos underflow
//&---------------------------------------------------------------------------------------&
void CompresorAritmetico::terminarCompresion()
{
    underflow++;
    if(piso < PRIMER_CUARTO) guardarBit(0);
    else guardarBit(1);

    output->flushBuffer();
}

//&---------------------------------------------------------------------------------------&
//& prepararDescompresion:  Preparamos el descompresor
//&---------------------------------------------------------------------------------------&
void CompresorAritmetico::prepararDescompresion()
{
    piso = PISO;
    techo = TECHO;
    valor = 0;

    //Leemos los primeros 16bits del archivo
    for(register unsigned short i = 0; i<16; i++)
    {
        valor = 2*valor+input->leerBit();
    }

    return;
}
//&---------------------------------------------------------------------------------------&
//& descomprimir:  Descomprimimos el archivo
//&             http://www.arturocampos.com/ac_arithmetic.html
//&---------------------------------------------------------------------------------------&
Direccion CompresorAritmetico::descomprimir(int* frecuencias)
{
    Direccion       bits;
    long            temp;
    unsigned long   rango = techo - piso + 1;
    Frecuencias     frecAcumuladas = {0,0,0,0};

    //Calculamos las frecuencias acumuladas para este estado
    for( register short dir = 0; dir <4 ; dir++)
        frecAcumuladas[dir] = frecuencias[dir] + frecAcumuladas[((dir - 1)<0)?0:(dir-1)];

    //Deshacemos los calculos del nuevo rango
    temp = (valor - piso) + 1;
    temp = temp * frecAcumuladas[3] - 1;
    temp /= rango;

    //Vemos en que intervalo cae el valor calculado
    for(register short i = 0 ; i < 4; i++)
        if(frecAcumuladas[i] > temp)
        {
            bits = i;
            break;
        }

    //Actualizamos los valores de los intervalos
    techo = piso + rango * frecAcumuladas[bits] / frecAcumuladas[3] - 1;
    piso = piso + rango * ((bits-1 < 0)?0:(frecAcumuladas[bits-1])) / frecAcumuladas[3];

    //Una vez calculado el nuevo rango, debemos guardar los bits que converjan
    for(;;)
    {
        if(techo < MEDIO){}
        else if(piso >= MEDIO)
        {
            piso -= MEDIO;
            techo -= MEDIO;
            valor -= MEDIO;
        }
        else if( piso >= PRIMER_CUARTO && techo < TERCER_CUARTO)
        {
            piso -= PRIMER_CUARTO;
            techo -= PRIMER_CUARTO;
            valor -= PRIMER_CUARTO;
        }
        else break;

        piso = 2*piso;
        techo = 2*techo+1;
        valor = 2*valor+input->leerBit();
    }
    return bits;
}

/**
//----------------------------------------------------------------------------------------&
// P R I V A T E
//----------------------------------------------------------------------------------------&
*/

//&---------------------------------------------------------------------------------------&
//& guardarBit: Terminamos la compersion. Seleccionamos el intervalo
//&             correspondiente al caracter y sacamos underflow
//&---------------------------------------------------------------------------------------&
void CompresorAritmetico::guardarBit(const unsigned short bit)
{
    output->guardarBit(bit);
    while(underflow>0)
    {
        output->guardarBit(!bit);
        underflow--;
    }
}
