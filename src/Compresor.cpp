#include "Compresor.h"
#include <iostream>       // std::cout
#include <string>         // std::string
#include <bitset>         // std::bitset

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
    this->dmc     = new DMC();
    this->input   = new FileManagerInput();
    this->output  = new FileManagerOutput();
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

    //Abrimos el archivo
    if(input->open(filename, ios::in|ios::binary|ios::ate) == ERROR_APERTURA_ARCHIVO)
    {
        cout << "ERROR: No se pudo abrir el archivo" << endl;
        return;
    }

    //Recorremos el archivo de a dos bits
    bits = input->leerDosBits();
    while(bits != ERROR_EOF)
    {
        //Comprimimos
        //compresorAritmetico->comprimir(bits,dmc->getFrecuencias());

        //Actualizamos el modelo DMC
        dmc->actualizarModelo(bits);

        //Leemos los proximos dos bits
        bits = input->leerDosBits();
    }

/**
    //EL SIGUIENTE CODIGO ES PARA EL FILEMANAGEROUTPUT. ASI SE GUARDAN BITS:

    std::bitset<8> byte;
    Direccion bits;
    short cont = 7;
    char e;

    std::ofstream ArchivoFinal ("COPIA",std::ios::binary);

    input->open(filename, ios::in|ios::binary|ios::ate);

    bits = input->leerDosBits();
    while(bits != ERROR_EOF)
    {
        if(cont == -1)
        {
            e = byte.to_ulong();
            ArchivoFinal.write( &e, sizeof(e) ) ;
            cont = 7;
        }

        switch( bits )
        {
            case BITS_00:
                byte[cont] = 0;
                byte[cont-1] = 0;
                break;

            case BITS_01:
                byte[cont] = 0;
                byte[cont-1] = 1;
                break;

            case BITS_10:
                byte[cont] = 1;
                byte[cont-1] = 0;
                break;

            case BITS_11:
                byte[cont] = 1;
                byte[cont-1] = 1;
                break;
        }

        cont -= 2;

        //Leemos los proximos dos bits
        bits = input->leerDosBits();
    }
    ArchivoFinal.close();
*/
}

/**
//---------------------------------------------------------------------------&
// P R I V A T E
//---------------------------------------------------------------------------&
*/
