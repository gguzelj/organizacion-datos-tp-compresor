#ifndef COMPRESOR_H
#define COMPRESOR_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "DMC.h"
#include "Estado.h"
#include "FileManager.h"

/**
//---------------------------------------------------------------------------&
// Clase:       Compresor
// Descripcion: Clase encargada de coordinar todas las entidades del compresor
//---------------------------------------------------------------------------&
*/
class Compresor
{
//&---------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------&
    public:
        Compresor();
        virtual ~Compresor();

        void ejecutar(int argc, char** argv);
        void comprimir(char *filename);

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:

        DMC                 *dmc;   //Contendra el modelo DMC
        FileManagerInput    *input; //Referencia al archivo de entrada
        FileManagerOutput   *output;//Referencia al archivo de salida

};
#endif // COMPRESOR_H