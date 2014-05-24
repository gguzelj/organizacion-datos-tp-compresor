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

        CompresorAritmetico(FileManagerInput *inPut, FileManagerOutput *outPut);
        virtual ~CompresorAritmetico();

        void        comprimir(Direccion bits, int* frecuencias);
        Direccion   descomprimir(int* frecuencias);
        void        prepararDescompresor();

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:

        unsigned short      piso;           //Piso del intervalo
        unsigned short      techo;          //Techo del intervalo
        unsigned short      valor;          //Valor del archivo para el intervalo
        unsigned char       underflow;      //Contador de underflow
        Byte                byteBuffer;     //Buffer de Bytes
        unsigned short      contadorBits;   //Contador de bits enviados
        FileManagerInput    *input;         //Ref. al archivo de entrada
        FileManagerOutput   *output;        //Ref. al archivo de salida

        void                guardarBitsYValidarUnderflow();
        void                guardarBit(const unsigned short bit);
};

#endif // COMPRESORARITMETICO_H
