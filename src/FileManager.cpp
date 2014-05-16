#include "FileManager.h"
/**
//---------------------------------------------------------------------------&
// Clase:       FileManager
// Descripcion: Esta clase es la encargada de hacer la lectura/escritura de
// los archivos a comprimir/descomprimir
//---------------------------------------------------------------------------&
*/
/**
//---------------------------------------------------------------------------&
// P U B L I C
//---------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------&
//& FileManager: Constructor
//&---------------------------------------------------------------------------&
FileManager::FileManager()
{
}

//&---------------------------------------------------------------------------&
//& FileManager: Destructor
//&---------------------------------------------------------------------------&
FileManager::~FileManager()
{
}

/**
//---------------------------------------------------------------------------&
// Clase:       FileManagerInput
// Descripcion: Clase encargada de cargar y manejar el archivo a comprimir
//---------------------------------------------------------------------------&
*/
/**
//---------------------------------------------------------------------------&
// P U B L I C
//---------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------&
//& FileManagerInput: Constructor
//&---------------------------------------------------------------------------&
FileManagerInput::FileManagerInput()
{
}

//&---------------------------------------------------------------------------&
//& FileManagerInput: Destructor
//&---------------------------------------------------------------------------&
FileManagerInput::~FileManagerInput()
{
    delete[] buffer_;

    if (file_.is_open())
    {
        file_.close();
    }
}

//&---------------------------------------------------------------------------&
//& open: Abrimos un archivo en el modo especificado
//&---------------------------------------------------------------------------&
int FileManagerInput::open(const char *filename, std::ios::openmode mode)
{
    file_.open(filename, mode);

    //Calculamos la cantidad de particiones que debemos hacer
    if (file_.is_open())
    {
        file_.seekg(0, file_.end);
        fileSize_ = file_.tellg();
        cantPartes_ = 1 + (fileSize_ / TAMANO_MAX_BUFFER);
        filePos_ = 0;
        bytesEmitidos_ = 0;
        bitsEmitidos_ = 0;
        bufferVacio_ = true;
        ultimoBloque_ = false;

    }
    else
    {
        return ERROR_APERTURA_ARCHIVO;
    }
    return 0;
}

//&---------------------------------------------------------------------------&
//& close: Cerramos el archivo
//&---------------------------------------------------------------------------&
void FileManagerInput::close()
{
    file_.close();
}

//&---------------------------------------------------------------------------&
//& leerDosBits: Dentro de este metodo vamos a leer solamente 2 bits del
//& archivo original, y los vamos a devolver para que puedan ser comprimidos
//&---------------------------------------------------------------------------&
Direccion FileManagerInput::leerDosBits()
{
    Direccion dir = (Direccion)ERROR_EOF;

    //Controlamos si debemos hacer un read al archivo para completar
    //el buffer con datos
    if(bufferVacio_)
    {
        bufferVacio_ = false;
        bytesEmitidos_ = 0;
        if(this->read() == ERROR_EOF)
            return dir;
    }

    //Analizamos el valor de los dos bits mas significativos
    switch( buffer_[bytesEmitidos_] & BIT_MASK )
    {
        case 0x00: dir = BITS_00; break;
        case 0x40: dir = BITS_01; break;
        case 0x80: dir = BITS_10; break;
        case 0xC0: dir = BITS_11; break;
    }

    //Hacemos un shitf logico del buffer
    buffer_[bytesEmitidos_] =buffer_[bytesEmitidos_]<< 2;

    //Actualizamos contadores
    bitsEmitidos_ += 2;

    if(bitsEmitidos_ == 8)
    {
        bitsEmitidos_ = 0;
        bytesEmitidos_++;
    }

    //En caso de haber leido todo el buffer, seteamos el flag para indicar
    //que necesitamos leer una nueva parte del archivo
    if(bytesEmitidos_ == bufferSize_)
        bufferVacio_ = true;

    return dir;
}

/**
//---------------------------------------------------------------------------&
// P R I V A T E
//---------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------&
//& read: Metodo encargado de leer el archivo a comprimir. El archivo puede
//& estar partido en varias partes, que seran cargadas paulatinamente en el
//& buffer
//&---------------------------------------------------------------------------&
int FileManagerInput::read()
{

    if(ultimoBloque_)
        return ERROR_EOF;

    if(cantPartes_ == 1)
    {
        //El archivo entra en memoria
        unsigned int bufferSize = fileSize_ - filePos_;

        delete buffer_;
        buffer_ = new char[bufferSize];
        file_.seekg (filePos_);
        file_.read (buffer_, ( bufferSize ));
        ultimoBloque_ = true;
        bufferSize_ = bufferSize ;
        return 0;
    }
    else
    {
        //El archivo no entra en memoria=> leemos una parte
        delete buffer_;
        buffer_ = new char[TAMANO_MAX_BUFFER];
        file_.seekg (filePos_);
        file_.read (buffer_, TAMANO_MAX_BUFFER);
        filePos_ += TAMANO_MAX_BUFFER;
        bufferSize_ = TAMANO_MAX_BUFFER;
        cantPartes_--;
        return 0;
    }
}

/**
//---------------------------------------------------------------------------&
// Clase:       FileManagerOutput
// Descripcion: Clase encargada de guardar y manejar el archivo a comprimido
//---------------------------------------------------------------------------&
*/

/**
//---------------------------------------------------------------------------&
// P U B L I C
//---------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------&
//& FileManagerOutput: Constructor
//&---------------------------------------------------------------------------&
FileManagerOutput::FileManagerOutput()
{
}

//&---------------------------------------------------------------------------&
//& FileManagerOutput: Destructor
//&---------------------------------------------------------------------------&
FileManagerOutput::~FileManagerOutput()
{
}
/**
//---------------------------------------------------------------------------&
// P R I V A T E
//---------------------------------------------------------------------------&
*/
