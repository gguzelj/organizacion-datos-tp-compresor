#ifndef COMPRESORARITMETICO_H
#define COMPRESORARITMETICO_H

#include "utils/constantes.h"
#include "FileManager.h"

/**
//----------------------------------------------------------------------------------------&
// Clase:           CompresorAritmetico
// Descripcion:     Esta clase es la encargada de hacer la compresion /
//                  descompresion aritmetica de los archivos
//----------------------------------------------------------------------------------------&
*/
class CompresorAritmetico
{
//&---------------------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------------------&
    public:

        /** Metodos     */
        CompresorAritmetico(FileManagerInput *inPut, FileManagerOutput *outPut);
        virtual ~CompresorAritmetico();

        void        prepararCompresion();
        void        comprimir(Direccion bits, int* frecuencias);
        void        terminarCompresion();

        void        prepararDescompresion();
        Direccion   descomprimir(int* frecuencias);
        void        terminarDescompresion();

//&---------------------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------------------&
    private:

        /** Atributos   */
        unsigned short      piso;           //Piso del intervalo
        unsigned short      techo;          //Techo del intervalo
        unsigned short      valor;          //Valor del archivo para el intervalo
        unsigned char       underflow;      //Contador de underflow
        FileManagerInput    *input;         //Ref. al archivo de entrada
        FileManagerOutput   *output;        //Ref. al archivo de salida

        /** Metodos     */
        void                guardarBit(const unsigned short bit);
};

#endif // COMPRESORARITMETICO_H
