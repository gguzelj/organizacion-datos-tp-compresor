#ifndef ESTADO_H
#define ESTADO_H

#include "utils/constantes.h"

/**
//----------------------------------------------------------------------------------------&
// Clase:       Estado
// Descripcion: Utilizada para representar los estados del modelo DMC
//----------------------------------------------------------------------------------------&
*/
class Estado
{

//&---------------------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------------------&
    public:

        /** Metodos     */
        Estado();
        Estado(Nombre nombre);
        virtual ~Estado();

        int*    getFrecuencias();
        void    setFrecuencia(Direccion direccion, int frecuencia);
        Estado* getEstadoSiguiente(Direccion direccion);
        void    setEstadoSiguiente(Direccion direccion, Estado *estadoSiguiente);

        void    aumentarFrecuencia(Direccion direccion, int cantidad );
        int     getFrecuencia(Direccion direccion);

        int     getCantidadVisitas();

        /** Atributos   */
        Frecuencias frecuencias;            //Atributos publicos solo por performance
        Estado*     estadosSiguientes[4];

//&---------------------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------------------&
    private:

        /** Atributos   */
        Nombre      nombre;

};

#endif // ESTADO_H
