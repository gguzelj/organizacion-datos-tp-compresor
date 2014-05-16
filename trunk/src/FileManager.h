#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "utils/constantes.h"

/**
//---------------------------------------------------------------------------&
// Clase:       FileManager
// Descripcion: Esta clase es la encargada de hacer la lectura/escritura de
// los archivos a comprimir/descomprimir
//---------------------------------------------------------------------------&
*/
class FileManager
{
    public:
    FileManager();
    virtual ~FileManager();
};
/**
//---------------------------------------------------------------------------&
// Clase:       FileManagerInput
// Descripcion: Clase encargada de cargar y manejar el archivo a comprimir
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

        int open(const char *filename, std::ios::openmode mode);
        void close();

        Direccion leerDosBits();

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:

        std::ifstream   file_;
        size_t          fileSize_;
        char*           buffer_;
        size_t          bufferSize_;
        unsigned int    cantPartes_;
        std::streampos  filePos_;
        unsigned short  bitsEmitidos_;
        unsigned long   bytesEmitidos_;
        bool            bufferVacio_;
        bool            ultimoBloque_;

        int read();
};



/**
//---------------------------------------------------------------------------&
// Clase:       FileManagerOutput
// Descripcion: Clase encargada de guardar y manejar el archivo a comprimido
//---------------------------------------------------------------------------&
*/
class FileManagerOutput : public FileManager {
//&---------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------&
    public:

        FileManagerOutput();
        virtual ~FileManagerOutput();

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:

};
#endif // FILEMANAGER_H
