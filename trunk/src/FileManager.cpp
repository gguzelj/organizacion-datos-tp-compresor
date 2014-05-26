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

//&---------------------------------------------------------------------------&
//& open: Abrimos un archivo en el modo especificado
//&---------------------------------------------------------------------------&
int FileManager::open(const char *filename, std::ios::openmode mode)
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
        totalBytesArchivo_ = 0;
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
void FileManager::close()
{
    file_.close();
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
        totalBytesArchivo_++;
    }

    //En caso de haber leido todo el buffer, seteamos el flag para indicar
    //que necesitamos leer una nueva parte del archivo
    if(bytesEmitidos_ == bufferSize_)
        bufferVacio_ = true;

    return dir;
}

//&---------------------------------------------------------------------------&
//& getCantidadBytesProcesados: Devolvemos la cantidad de bytes procesados
//&---------------------------------------------------------------------------&
uint64_t FileManagerInput::getCantidadBytesProcesados()
{
    return totalBytesArchivo_;
}

//&---------------------------------------------------------------------------&
//& getTamanioArchivoOriginal:  Devolvemos el tamanio del archivo original
//&---------------------------------------------------------------------------&
uint64_t FileManagerInput::getTamanioArchivoOriginal()
{
    char* tamanio = (char*)malloc(sizeof(uint64_t));
    uint64_t totalBytes = 0;

    //Leemos la cantidad de bytes del archivo original
    file_.seekg(0, file_.beg);
    file_.read(tamanio, sizeof(uint64_t));

    strncpy((char*)&totalBytes, tamanio, 8);

    return totalBytes;
}

//&---------------------------------------------------------------------------&
//& leerBits:   Leemos una determinada cantidad de bits del archivo.
//&             Se pueden leer hasta 1byte
//&---------------------------------------------------------------------------&
unsigned char FileManagerInput::leerBits(int cantidadBits)
{
    unsigned char   byte;
    unsigned short  shift = 8 - cantidadBits;

    while(cantidadBits)
    {
        //Controlamos si debemos hacer un read al archivo para completar el buffer con datos
        if(bufferVacio_)
        {
            bufferVacio_ = false;
            bytesEmitidos_ = 0;
            if(this->read() == ERROR_EOF)
                return byte;
        }

        //Almacenamos el nuevo bit del archivo
        byte <<= 1; byte |=  (buffer_[bytesEmitidos_] & 0x80)?1:0;

        //Hacemos un shitf logico del buffer
        buffer_[bytesEmitidos_] <<= 1;

        //Actualizamos contadores del fileManager
        bitsEmitidos_ ++;

        if(bitsEmitidos_ == 8)
        {
            bitsEmitidos_ = 0;
            bytesEmitidos_++;
        }

        //En caso de haber leido todo el buffer, seteamos el flag para indicar
        //que necesitamos leer una nueva parte del archivo
        if(bytesEmitidos_ == bufferSize_)
            bufferVacio_ = true;

        cantidadBits--;
    }

    byte <<= shift;

    return byte;
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
    delete[] buffer_;

    if (file_.is_open())
    {
        file_.close();
    }
}

//&---------------------------------------------------------------------------&
//& open: Abrimos el archivo de salida
//&---------------------------------------------------------------------------&
int FileManagerOutput::open(const char *filename, std::ios::openmode mode)
{
    file_.open(filename, mode);

    //Calculamos la cantidad de particiones que debemos hacer
    if (file_.is_open())
    {
        bytesEmitidos_ = 0;
        bitsEmitidos_ = 0;
    }
    else
    {
        return ERROR_APERTURA_ARCHIVO;
    }
    return 0;
}

//&---------------------------------------------------------------------------&
//& EscribirBits:   Metodo encargado de escribir la cantidad de bits recibidada
//&                 por parametro en el archivo de salida
//&---------------------------------------------------------------------------&
int FileManagerOutput::escribirByte(Byte byte)
{
    if(!file_.is_open())
        return ERROR_ARCHIVO_CERRADO;

    char charByte = byte.to_ulong();
    file_.write( &charByte, sizeof(charByte));

    return 0;
}

//&---------------------------------------------------------------------------&
//& escribirTamanioArchivo: Escribimos en el archivo de salida la cantidad de
//&                         bytes que tiene el archivo original.
//&                         Esto se hace como alternativa al EOF, ya que si se
//&                         utiliza este caracter especial, se reduce mucho el
//&                         nivel de compresion
//&---------------------------------------------------------------------------&
int FileManagerOutput::escribirTamanioArchivo(uint64_t tamanio)
{
    if(!file_.is_open())
        return ERROR_ARCHIVO_CERRADO;

    //Guardamos el tamanio del archivo
    file_.seekp(0L);
    file_.write(reinterpret_cast<const char*>(&tamanio), sizeof(tamanio));

    return 0;
}

//&---------------------------------------------------------------------------&
//& reservarEspacioTamanio: Reservamos los 8 bytes necesarios para guardar la
//&                         cantidad de bytes que contiene el archivo original
//&---------------------------------------------------------------------------&
int FileManagerOutput::reservarEspacioTamanio()
{
    if(!file_.is_open())
        return ERROR_ARCHIVO_CERRADO;

    uint64_t unsigned64BitNumber = 0;

    //Escribimos 8 bytes al inicio del archivo
    file_.write((const char*) (&unsigned64BitNumber), sizeof(uint64_t));

    return 0;
}

/**
//---------------------------------------------------------------------------&
// P R I V A T E
//---------------------------------------------------------------------------&
*/
