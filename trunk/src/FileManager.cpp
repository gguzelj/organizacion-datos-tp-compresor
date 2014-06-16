#include "FileManager.h"

/**
//----------------------------------------------------------------------------------------&
// Clase:       FileManager
// Descripcion: Esta clase es la encargada de hacer la lectura/escritura de
// los archivos a comprimir/descomprimir
//----------------------------------------------------------------------------------------&
*/
/**
//----------------------------------------------------------------------------------------&
// P U B L I C
//----------------------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------------------&
//& FileManager: Constructor
//&---------------------------------------------------------------------------------------&
FileManager::FileManager()
{
}

//&---------------------------------------------------------------------------------------&
//& FileManager: Destructor
//&---------------------------------------------------------------------------------------&
FileManager::~FileManager()
{
}

//&---------------------------------------------------------------------------------------&
//& open: Abrimos un archivo en el modo especificado
//&---------------------------------------------------------------------------------------&
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
        bufferSize_ = 0;
        bufferVacio_ = true;
        ultimoBloque_ = false;

    }
    else
    {
        return ERROR_APERTURA_ARCHIVO;
    }
    return 0;
}

//&---------------------------------------------------------------------------------------&
//& close: Cerramos el archivo
//&---------------------------------------------------------------------------------------&
void FileManager::close()
{
    file_.close();
}

/**
//----------------------------------------------------------------------------------------&
// Clase:       FileManagerInput
// Descripcion: Clase encargada de cargar y manejar el archivo a comprimir
//----------------------------------------------------------------------------------------&
*/
/**
//----------------------------------------------------------------------------------------&
// P U B L I C
//----------------------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------------------&
//& FileManagerInput: Constructor
//&---------------------------------------------------------------------------------------&
FileManagerInput::FileManagerInput()
{
}

//&---------------------------------------------------------------------------------------&
//& FileManagerInput: Destructor
//&---------------------------------------------------------------------------------------&
FileManagerInput::~FileManagerInput()
{
    delete buffer_;

    if (file_.is_open())
        file_.close();
}

//&---------------------------------------------------------------------------------------&
//& leerDosBits: Dentro de este metodo vamos a leer solamente 2 bits del
//& archivo original, y los vamos a devolver para que puedan ser comprimidos
//&---------------------------------------------------------------------------------------&
Direccion FileManagerInput::leerDosBits()
{
    Direccion dir = (Direccion)ERROR_EOF;

    //Controlamos si debemos hacer un read al archivo para completar
    //el buffer con datos
    if(bytesEmitidos_ == bufferSize_)
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

    return dir;
}

//&---------------------------------------------------------------------------------------&
//& getCantidadBytesProcesados: Devolvemos la cantidad de bytes procesados
//&---------------------------------------------------------------------------------------&
uint64_t FileManagerInput::getCantidadBytesProcesados()
{
    return totalBytesArchivo_;
}

//&---------------------------------------------------------------------------------------&
//& getTamanioArchivoOriginal:  Devolvemos el tamanio del archivo original
//&---------------------------------------------------------------------------------------&
uint64_t FileManagerInput::getTamanioArchivoOriginal()
{

    if(!file_.is_open())
        return ERROR_APERTURA_ARCHIVO;

    char* tamanio = (char*)malloc(sizeof(uint64_t));
    uint64_t totalBytes = 0;

    //Leemos la cantidad de bytes del archivo original
    file_.seekg(0, file_.beg);
    file_.read(tamanio, sizeof(uint64_t));
    filePos_ = sizeof(uint64_t);
    file_.seekg(filePos_);

    memcpy(&totalBytes, tamanio, 8);
    delete tamanio;

    return totalBytes;
}

//&---------------------------------------------------------------------------------------&
//& leerBits:   Leemos un bit del archivo de entrada
//&---------------------------------------------------------------------------------------&
int FileManagerInput::leerBit()
{
    int bit = ERROR_EOF;

    //Controlamos si debemos hacer un read al archivo para completar
    //el buffer con datos
    if(bytesEmitidos_ == bufferSize_)
    {
        bufferVacio_ = false;
        bytesEmitidos_ = 0;
        if(this->read() == ERROR_EOF)
            return bit;
    }

    //Analizamos el valor de los dos bits mas significativos
    bit = (buffer_[bytesEmitidos_] & 0x80)?1:0;

    //Hacemos un shitf logico del buffer
    buffer_[bytesEmitidos_] =buffer_[bytesEmitidos_]<< 1;

    //Actualizamos contadores
    bitsEmitidos_ += 1;

    if(bitsEmitidos_ == 8)
    {
        bitsEmitidos_ = 0;
        bytesEmitidos_++;
        totalBytesArchivo_++;
    }

    return bit;
}
/**
//----------------------------------------------------------------------------------------&
// P R I V A T E
//----------------------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------------------&
//& read:   Metodo encargado de leer el archivo a comprimir. El archivo puede estar partido
//&         en varias partes, que seran cargadas paulatinamente en el buffer
//&---------------------------------------------------------------------------------------&
int FileManagerInput::read()
{
    if(ultimoBloque_)
        return ERROR_EOF;

    if(cantPartes_ == 1)
    {
        //El archivo entra en memoria
        unsigned int bufferSize = fileSize_ - filePos_;

        buffer_ = new char[bufferSize];
        file_.seekg (filePos_);
        file_.read (buffer_, ( bufferSize ));
        ultimoBloque_ = true;
        bufferSize_ = bufferSize ;
    }
    else
    {
        //El archivo no entra en memoria=> leemos una parte
        buffer_ = new char[TAMANO_MAX_BUFFER];
        file_.seekg (filePos_);
        file_.read (buffer_, TAMANO_MAX_BUFFER);
        filePos_ += TAMANO_MAX_BUFFER;
        bufferSize_ = TAMANO_MAX_BUFFER;
        cantPartes_--;
    }
    return 0;
}

/**
//----------------------------------------------------------------------------------------&
// Clase:       FileManagerOutput
// Descripcion: Clase encargada de guardar y manejar el archivo a comprimido
//----------------------------------------------------------------------------------------&
*/

/**
//----------------------------------------------------------------------------------------&
// P U B L I C
//----------------------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------------------&
//& FileManagerOutput: Constructor
//&---------------------------------------------------------------------------------------&
FileManagerOutput::FileManagerOutput()
{
    contadorBits_ = 0;
}

//&---------------------------------------------------------------------------------------&
//& FileManagerOutput: Destructor
//&---------------------------------------------------------------------------------------&
FileManagerOutput::~FileManagerOutput()
{
    if (file_.is_open())
        file_.close();
}

//&---------------------------------------------------------------------------------------&
//& open: Abrimos el archivo de salida
//&---------------------------------------------------------------------------------------&
int FileManagerOutput::open(const char *filename, std::ios::openmode mode)
{
    file_.open(filename, mode);

    //Calculamos la cantidad de particiones que debemos hacer
    if (file_.is_open())
    {
        bytesEmitidos_ = 0;
        bitsEmitidos_ = 0;
        contadorBits_ = 0;
    }
    else
    {
        return ERROR_APERTURA_ARCHIVO;
    }
    return 0;
}

//&---------------------------------------------------------------------------------------&
//& EscribirByte:   Metodo encargado de escribir un byte en el arch. de salida
//&---------------------------------------------------------------------------------------&
int FileManagerOutput::escribirByte(Byte byte)
{
    if(!file_.is_open())
        return ERROR_ARCHIVO_CERRADO;

    char charByte = byte.to_ulong();
    file_.write( &charByte, sizeof(charByte));

    return 0;
}

//&---------------------------------------------------------------------------------------&
//& EscribirBits:   Metodo encargado de escribir dos bits en el arch. de salida
//&---------------------------------------------------------------------------------------&
int FileManagerOutput::escribirDosBits(Direccion bits)
{
    if(!file_.is_open())
        return ERROR_ARCHIVO_CERRADO;


    switch( bits )
    {
        case BITS_00:
            guardarBit(0);
            guardarBit(0);
            break;

        case BITS_01:
            guardarBit(0);
            guardarBit(1);
            break;

        case BITS_10:
            guardarBit(1);
            guardarBit(0);
            break;

        case BITS_11:
            guardarBit(1);
            guardarBit(1);
            break;
    }
    return 0;
}


//&---------------------------------------------------------------------------------------&
//& escribirTamanioArchivo: Escribimos en el archivo de salida la cantidad de
//&                         bytes que tiene el archivo original.
//&                         Esto se hace como alternativa al EOF, ya que si se
//&                         utiliza este caracter especial, se reduce mucho el
//&                         nivel de compresion
//&---------------------------------------------------------------------------------------&
int FileManagerOutput::escribirTamanioArchivo(uint64_t tamanio)
{
    if(!file_.is_open())
        return ERROR_ARCHIVO_CERRADO;

    //Guardamos el tamanio del archivo
    file_.seekp(0L);
    file_.write(reinterpret_cast<const char*>(&tamanio), sizeof(tamanio));

    return 0;
}

//&---------------------------------------------------------------------------------------&
//& reservarEspacioTamanio: Reservamos los 8 bytes necesarios para guardar la
//&                         cantidad de bytes que contiene el archivo original
//&---------------------------------------------------------------------------------------&
int FileManagerOutput::reservarEspacioTamanio()
{
    if(!file_.is_open())
        return ERROR_ARCHIVO_CERRADO;

    uint64_t unsigned64BitNumber = 0;

    //Escribimos 8 bytes al inicio del archivo
    file_.write((const char*) (&unsigned64BitNumber), sizeof(uint64_t));

    return 0;
}

//&---------------------------------------------------------------------------------------&
//& guardarBit: Metodo encargado de agrupar bits en bytes, y guardarlos en el
//&             archivo de salida
//&---------------------------------------------------------------------------------------&
void FileManagerOutput::guardarBit(const unsigned short bit)
{
    contadorBits_++;

    //Almacenamos el nuevo bit en el byteBuffer
    byteBuffer_[8-contadorBits_] = (bit)?1:0;

    //En caso de completar un byte entero, lo guardamos en el archivo
    if(contadorBits_ == 8)
    {
        contadorBits_ = 0;
        this->escribirByte(byteBuffer_);
    }
}

//&---------------------------------------------------------------------------------------&
//& flushBuffer:    Guardamos los bits que quedaron guardados en el buffer
//&---------------------------------------------------------------------------------------&
void FileManagerOutput::flushBuffer()
{
    while(contadorBits_ != 0)
    {
        contadorBits_++;

        //Seteamos en 0 todos los bits no utilizados del byte
        byteBuffer_[8-contadorBits_] = 0;

        //En caso de completar un byte entero, lo guardamos en el archivo
        if(contadorBits_ == 8)
        {
            contadorBits_ = 0;
            this->escribirByte(byteBuffer_);
        }
    }
}

/**
//&---------------------------------------------------------------------------------------&
// P R I V A T E
//&---------------------------------------------------------------------------------------&
*/
