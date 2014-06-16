#include "Compresor.h"
using namespace std;

/**
//----------------------------------------------------------------------------------------&
// P U B L I C
//----------------------------------------------------------------------------------------&
*/

//&---------------------------------------------------------------------------------------&
//& Compresor: Constructor de la clase Compresor
//&---------------------------------------------------------------------------------------&
Compresor::Compresor()
{
    this->dmc_                 = new DMC();
    this->input_               = new FileManagerInput();
    this->output_              = new FileManagerOutput();
    this->compresorAritmetico_ = new CompresorAritmetico( this->input_ , this->output_ );
}

//&---------------------------------------------------------------------------------------&
//& ~Compresor: Destructor de la clase Compresor
//&---------------------------------------------------------------------------------------&
Compresor::~Compresor()
{
    delete dmc_;
    delete input_;
    delete output_;
    delete compresorAritmetico_;
}

//&---------------------------------------------------------------------------------------&
//& ejecutar: Leemos la opcion a ejecutar
//&---------------------------------------------------------------------------------------&
void Compresor::ejecutar(int argc, char** argv)
{
    std::string modoComprimir ("-c");
    std::string modoDescomprimir ("-d");

    if (argc != 3)
    {
        cout << "La cantidad de argumentos no es la correcta"<<endl;
        exit(-1);
    }

    std::string modo( argv[1] );
    char* filename = argv[2];

    //Validamos si tenemos que comprimir
    if( modoComprimir.compare(modo) == 0)
        this->comprimir(filename);

    //Validamos si tenemos que descomprimir
    if( modoDescomprimir.compare(modo) == 0)
        this->descomprimir(filename);

}

//&---------------------------------------------------------------------------------------&
//& Comprimir: Procedemos a comprimir el archivo
//&---------------------------------------------------------------------------------------&
void Compresor::comprimir(char *filename)
{
    Direccion bits;

    //Abrimos los archivos
    if(abrirArchivosComprimir(filename))
        return;

    //Preparamos el compresor
    compresorAritmetico_->prepararCompresion();

    //Recorremos el archivo de a dos bits
    bits = input_->leerDosBits();
    while(bits != ERROR_EOF)
    {
        //Comprimimos
        compresorAritmetico_->comprimir(bits,dmc_->getFrecuencias());

        //Actualizamos el modelo DMC
        dmc_->actualizarModelo(bits);

        //Leemos los proximos dos bits
        bits = input_->leerDosBits();
    }

    //Terminamos la compresion
    compresorAritmetico_->terminarCompresion();

    //Guardamos la cantidad total de bytes procesados
    output_->escribirTamanioArchivo( input_->getCantidadBytesProcesados() );
}

//&---------------------------------------------------------------------------------------&
//& Descomprimir: Procedemos a descomprimir el archivo
//&---------------------------------------------------------------------------------------&
void Compresor::descomprimir(char *filename)
{
    Direccion       bits;
    uint64_t        cantidadBytesDescomprimidos = 0;
    unsigned short  cantidadBitsDescomprimidos = 0;

    //Abrimos los archivos
    if(abrirArchivosDescomprimir(filename))
        return;

    //Preparar compresor para descomprimir
    compresorAritmetico_->prepararDescompresion();

    //Comenzamos a descomprimir
    while(cantidadBytesDescomprimidos < totalBytesArchivo_)
    {
        //Descomprimimos los bits del archivo
        bits = compresorAritmetico_->descomprimir(dmc_->getFrecuencias());

        //Actualizamos el modelo DMC
        dmc_->actualizarModelo(bits);

        //Guardamos los dos bits
        output_->escribirDosBits(bits);

        //Actualizamos la cantidad de bytes descomprimidos
        if(cantidadBitsDescomprimidos == 6)
        {
            cantidadBitsDescomprimidos = 0;
            cantidadBytesDescomprimidos++;
        }
        else cantidadBitsDescomprimidos +=2;
    }
}
/**
//----------------------------------------------------------------------------------------&
// P R I V A T E
//----------------------------------------------------------------------------------------&
*/

//&---------------------------------------------------------------------------------------&
//& abrirArchivosComprimir: Abrimos los archivos para trabajar en la
//&                         compresion
//&---------------------------------------------------------------------------------------&
int Compresor::abrirArchivosComprimir(char *filename)
{
    char* filenameOut = (char*) malloc(strlen(filename)+strlen(NUMERO_GRUPO)+1);

    //Armamos el nombre del archivo de salida
    strcpy(filenameOut, filename);
    strcat(filenameOut, NUMERO_GRUPO);

    //Abrimos el archivo a comprimir
    if( input_->open(filename, ios::in|ios::binary|ios::ate) == ERROR_APERTURA_ARCHIVO )
    {
        cout << "ERROR: No se pudo abrir el archivo " << filename <<endl;
        free(filenameOut);
        exit(-1);
    }

    //Abrimos el archivo de salida
    if(output_->open(filenameOut,std::ios::binary))
    {
        cout << "ERROR: No se pudo crear el archivo " << filenameOut <<endl;
        free(filenameOut);
        exit(-1);
    }

    //En caso de poder abrir el archivo de salida, reservamos los primeros 8 bytes
    //para guardar la cantidad de bytes que procesa el compresor
    output_->reservarEspacioTamanio();

    free(filenameOut);
    return 0;
}

//&---------------------------------------------------------------------------------------&
//& abrirArchivosDescomprimir:  Abrimos los archivos para trabajar en la
//&                             Descompresion
//&---------------------------------------------------------------------------------------&
int Compresor::abrirArchivosDescomprimir(char *filename)
{
    const char nombreValido = strstr(filename, NUMERO_GRUPO) != NULL;

    if(!nombreValido)
    {
        cout << "ERROR: Formato de archivo imposible de descomprimir"<<endl;
        return ERROR_APERTURA_ARCHIVO;
    }

    //Creamos el nombre para el archivo de salida
    char* filenameOut = (char*) malloc(strlen(filename) - 3);
    strncpy(filenameOut, filename, strlen(filename)-3);
    filenameOut[strlen(filename) - 3] = '\0';

    //Abrimos el archivo a comprimir
    if( input_->open(filename, ios::in|ios::binary|ios::ate) == ERROR_APERTURA_ARCHIVO )
    {
        cout << "ERROR: No se pudo abrir el archivo " << filename <<endl;
        free(filenameOut);
        exit(-1);
    }

    //Abrimos el archivo de salida
    if(output_->open(filenameOut,std::ios::binary))
    {
        cout << "ERROR: No se pudo crear el archivo " << filenameOut <<endl;
        free(filenameOut);
        exit(-1);
    }

    //Leemos la cantidad de bytes del archivo original
    totalBytesArchivo_ = input_->getTamanioArchivoOriginal();

    free(filenameOut);
    return 0;
}
