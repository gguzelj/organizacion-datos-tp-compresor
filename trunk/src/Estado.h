#ifndef ESTADO_H
#define ESTADO_H

#include "utils/constantes.h"

/**
//---------------------------------------------------------------------------&
// Clase:       Estado
// Descripcion: Utilizada para representar los estados del modelo DMC
//---------------------------------------------------------------------------&
*/
class Estado
{

//&---------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------&
    public:
        Estado();
        virtual ~Estado();

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:
        Nombre      nombre;
        Frecuencias frecuencias;
        Estado*     proximosEstados[4];
};

#endif // ESTADO_H
