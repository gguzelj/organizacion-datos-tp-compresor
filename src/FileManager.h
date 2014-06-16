#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "utils/constantes.h"

/**
//----------------------------------------------------------------------------------------&
// Clase:           FileManager
// Descripcion:     Esta clase es la encargada de hacer la lectura/escritura
//                  de los archivos a comprimir/descomprimir
//----------------------------------------------------------------------------------------&
*/
class FileManager
{
    public:
    FileManager();
    virtual ~FileManager();

    int open(const char *filename, std::ios::openmode mode);
    void close();

    protected:
        std::fstream    file_;
        size_t          fileSize_;
        char*           buffer_;
        size_t          bufferSize_;
        unsigned int    cantPartes_;
        std::streampos  filePos_;
        unsigned short  bitsEmitidos_;
        unsigned long   bytesEmitidos_;
        uint64_t        totalBytesArchivo_;
        bool            bufferVacio_;
        bool            ultimoBloque_;
};
/**
//----------------------------------------------------------------------------------------&
// Clase:           FileManagerInput
// Descripcion:     Clase encargada de cargar y almacenar el archivo a
//                  comprimir / descomprimir
//----------------------------------------------------------------------------------------&
*/
class FileManagerInput : public FileManager
{
//&---------------------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------------------&
    public:

        /** Metodos     */
        FileManagerInput();
        virtual ~FileManagerInput();

        /** Atributos   */
        Direccion       leerDosBits();
        int             leerBit();
        uint64_t        getCantidadBytesProcesados();
        uint64_t        getTamanioArchivoOriginal();

//&---------------------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------------------&
    private:
        /** Metodos     */
        int             read();
};

/**
//----------------------------------------------------------------------------------------&
// Clase:           FileManagerOutput
// Descripcion:     Clase encargada de guardar y manejar el archivo a
//                  comprimir / descomprimir
//----------------------------------------------------------------------------------------&
*/
class FileManagerOutput : public FileManager {
//&---------------------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------------------&
    public:

        /** Metodos     */
        FileManagerOutput();
        virtual ~FileManagerOutput();

        int             open(const char *filename, std::ios::openmode mode);
        void            guardarBit(const unsigned short bit);
        int             escribirByte(Byte byte);
        int             escribirDosBits(Direccion bits);
        int             escribirTamanioArchivo(uint64_t tamanio);
        void            flushBuffer();
        int             reservarEspacioTamanio();

//&---------------------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------------------&
    private:

        /** Atributos   */
        Byte            byteBuffer_;     //Buffer de Bytes
        unsigned short  contadorBits_;   //Contador de bits enviados
        std::ofstream   file_;
};
#endif // FILEMANAGER_H
