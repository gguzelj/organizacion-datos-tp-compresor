#ifndef COMPRESORARITMETICO_H
#define COMPRESORARITMETICO_H

#include "utils/constantes.h"
#include "FileManager.h"

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

        CompresorAritmetico(FileManagerOutput *outPut);
        virtual ~CompresorAritmetico();

        void comprimir(Direccion bits, int* frecuencias);

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:

        unsigned short          piso;
        unsigned short          techo;
        unsigned short          medio;
        unsigned char           underflow;
        FileManagerOutput       *output;
        Byte                    byteBuffer;
        unsigned short          contadorBits;


        void                    guardarBitsYValidarUnderflow();
        void                    guardarBit(const unsigned short bit);

};

#endif // COMPRESORARITMETICO_H
