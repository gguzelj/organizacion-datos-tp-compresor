#include "Estado.h"

/**
//----------------------------------------------------------------------------------------&
// P U B L I C
//----------------------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------------------&
//& Estado: Constructor de la clase Estado
//&---------------------------------------------------------------------------------------&
Estado::Estado()
{
}

//&---------------------------------------------------------------------------------------&
//& Estado: Constructor de la clase Estado
//&---------------------------------------------------------------------------------------&
Estado::Estado(Nombre nom)
{
    nombre = nom;
    frecuencias[BITS_00] = 1;
    frecuencias[BITS_01] = 1;
    frecuencias[BITS_10] = 1;
    frecuencias[BITS_11] = 1;
}

//&---------------------------------------------------------------------------------------&
//& ~Estado: Destructor de la clase Estado
//&---------------------------------------------------------------------------------------&
Estado::~Estado()
{
    //dtor
}

//&---------------------------------------------------------------------------------------&
//& getFrecuencias: Devolvemos las frecuencias de los siguientes estados
//&---------------------------------------------------------------------------------------&
int* Estado::getFrecuencias()
{
    return frecuencias;
}
//&---------------------------------------------------------------------------------------&
//& getFrecuencia: Devolvemos la frecuencia correspondiente a una direccion
//&---------------------------------------------------------------------------------------&
int Estado::getFrecuencia(Direccion direccion)
{
    return frecuencias[direccion];
}

//&---------------------------------------------------------------------------------------&
//& setFrecuencia: Seteamos la frecuencia para el siguiente estado
//&---------------------------------------------------------------------------------------&
void Estado::setFrecuencia(Direccion direccion, int frecuencia)
{
    frecuencias[direccion] = frecuencia;
}

//&---------------------------------------------------------------------------------------&
//& getProximoEstado: Devolvemos el siguiente estado asociado a la direccion
//& recibida por parametro (00, 01, 10, 11)
//&---------------------------------------------------------------------------------------&
Estado* Estado::getEstadoSiguiente(Direccion direccion)
{
    return estadosSiguientes[direccion];
}

//&---------------------------------------------------------------------------------------&
//& setProximoEstado: Seteamos el siguiente estado asociado a la direccion recibida por
//& parametro (00, 01, 10, 11)
//&---------------------------------------------------------------------------------------&
void Estado::setEstadoSiguiente(Direccion direccion, Estado *estadoSiguiente)
{
    estadosSiguientes[direccion] = estadoSiguiente;
}

//&---------------------------------------------------------------------------------------&
//& aumentarFrecuencia: Aumentamos la frecuencia en la direccion dada
//&---------------------------------------------------------------------------------------&
void Estado::aumentarFrecuencia(Direccion direccion, int cantidad )
{
    frecuencias[direccion] += cantidad;
}

//&---------------------------------------------------------------------------------------&
//& getCantidadVisitas: Devolvemos la cantidad de visitas que tuvo el estado
//&---------------------------------------------------------------------------------------&
int Estado::getCantidadVisitas()
{
    return  frecuencias[BITS_00] +
            frecuencias[BITS_01] +
            frecuencias[BITS_10] +
            frecuencias[BITS_11];
}

/**
//----------------------------------------------------------------------------------------&
// P R I V A T E
//----------------------------------------------------------------------------------------&
*/
