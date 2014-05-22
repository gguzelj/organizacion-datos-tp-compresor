#include "Compresor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

/**
//---------------------------------------------------------------------------&
// P U B L I C
//---------------------------------------------------------------------------&
*/

//&---------------------------------------------------------------------------&
//& Compresor: Constructor de la clase Compresor
//&---------------------------------------------------------------------------&
Compresor::Compresor()
{
    this->dmc                 = new DMC();
    this->input               = new FileManagerInput();
    this->output              = new FileManagerOutput();
    this->compresorAritmetico = new CompresorAritmetico( this->output );
}

//&---------------------------------------------------------------------------&
//& ~Compresor: Destructor de la clase Compresor
//&---------------------------------------------------------------------------&
Compresor::~Compresor()
{
    dmc->~DMC();
    delete dmc;

    input->~FileManagerInput();
    delete input;

    output->~FileManagerOutput();
    delete output;

    compresorAritmetico->~CompresorAritmetico();
    delete compresorAritmetico;
}

//&---------------------------------------------------------------------------&
//& ejecutar: Leemos la opcion a ejecutar
//&---------------------------------------------------------------------------&
void Compresor::ejecutar(int argc, char** argv)
{
    std::string modoComprimir ("-c");
    std::string modoDescomprimir ("-d");

    if (argc != 3)
    {
        cout << "La cantidad de argumentos no es la correcta"<<endl;
        return;
    }

    std::string modo( argv[1] );
    char* filename = argv[2];

    //Validamos si tenemos que comprimir
    if( modoComprimir.compare(modo) == 0)
        this->comprimir(filename);

    //Validamos si tenemos que descomprimir
    if( modoDescomprimir.compare(modo) == 0)
        this->comprimir(filename); //this->descomprimir(filename);

}

//&---------------------------------------------------------------------------&
//& Comprimir: Procedemos a comprimir el archivo
//&---------------------------------------------------------------------------&
void Compresor::comprimir(char *filename)
{
    Direccion bits;

    //Abrimos los archivos
    if(abrirArchivos(filename))
        return;

    //Recorremos el archivo de a dos bits
    bits = input->leerDosBits();
    while(bits != ERROR_EOF)
    {
        //Comprimimos
        compresorAritmetico->comprimir(bits,dmc->getFrecuencias());

        //Actualizamos el modelo DMC
        dmc->actualizarModelo(bits);

        //Leemos los proximos dos bits
        bits = input->leerDosBits();
    }
}

/**
//---------------------------------------------------------------------------&
// P R I V A T E
//---------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------&
//& abrirArchivos:  Abrimos los archivos para trabajar
//&---------------------------------------------------------------------------&
int Compresor::abrirArchivos(char *filename)
{
    const char* NUMERO_GRUPO = ".13";
    char* filenameOut = (char*) malloc(strlen(filename)+strlen(NUMERO_GRUPO)+1);
    int resultado = 0;

    //Armamos el nombre del archivo de salida
    strcpy(filenameOut, filename);
    strcat(filenameOut, NUMERO_GRUPO);

    //Abrimos el archivo
    resultado = input->open(filename, ios::in|ios::binary|ios::ate);
    resultado = output->open(filenameOut,std::ios::binary);

    if(resultado == ERROR_APERTURA_ARCHIVO)
        cout << "Error al tratar de abrir los archivos"<<endl;

    //Liberamos variables
    free(filenameOut);
    return resultado;
}
