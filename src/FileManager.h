#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "utils/constantes.h"

/**
//---------------------------------------------------------------------------&
// Clase:           FileManager
// Descripcion:     Esta clase es la encargada de hacer la lectura/escritura
//                  de los archivos a comprimir/descomprimir
//---------------------------------------------------------------------------&
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
//---------------------------------------------------------------------------&
// Clase:           FileManagerInput
// Descripcion:     Clase encargada de cargar y almacenar el archivo a
//                  comprimir / descomprimir
//---------------------------------------------------------------------------&
*/
class FileManagerInput : public FileManager
{
//&---------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------&
    public:

        FileManagerInput();
        virtual ~FileManagerInput();

        Direccion       leerDosBits();
        std::list<Byte> leerBits(int cantidadBits);
        uint64_t        getCantidadBytesProcesados();
        uint64_t        getTamanioArchivoOriginal();

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:

        int read();
};

/**
//---------------------------------------------------------------------------&
// Clase:           FileManagerOutput
// Descripcion:     Clase encargada de guardar y manejar el archivo a
//                  comprimir / descomprimir
//---------------------------------------------------------------------------&
*/
class FileManagerOutput : public FileManager {
//&---------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------&
    public:

        FileManagerOutput();
        virtual ~FileManagerOutput();

        int open(const char *filename, std::ios::openmode mode);
        int escribirByte(Byte byte);
        int escribirTamanioArchivo(uint64_t tamanio);
        int reservarEspacioTamanio();

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:
        std::ofstream   file_;
};
#endif // FILEMANAGER_H
