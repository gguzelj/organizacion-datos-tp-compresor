#ifndef COMPRESORARITMETICO_H
#define COMPRESORARITMETICO_H

#include "utils/constantes.h"

/**
//---------------------------------------------------------------------------&
// Clase:           CompresorAritmetico
// Descripcion:     Esta clase es la encargada de hacer la compresion /
//                  descompresion aritmetica de los archivos
//---------------------------------------------------------------------------&
*/
class CompresorAritmetico
{
//&---------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------&
    public:

        CompresorAritmetico();
        virtual ~CompresorAritmetico();

        void comprimir(Direccion bits, int* frecuencias);

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:

        unsigned short  piso;
        unsigned short  techo;
        unsigned char   underflow;

        void            guardarBitsComprimidos();
};

#endif // COMPRESORARITMETICO_H
