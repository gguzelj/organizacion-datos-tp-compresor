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
        Estado(Nombre nombre);
        virtual ~Estado();

        int*    getFrecuencias();
        void    setFrecuencia(Direccion direccion, int frecuencia);
        Estado* getEstadoSiguiente(Direccion direccion);
        void    setEstadoSiguiente(Direccion direccion, Estado *estadoSiguiente);

        void    aumentarFrecuencia(Direccion direccion, int cantidad );

        int     getCantidadVisitas();

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:
        Nombre      nombre;
        Frecuencias frecuencias;
        Estado*     estadosSiguientes[4];
};

#endif // ESTADO_H
