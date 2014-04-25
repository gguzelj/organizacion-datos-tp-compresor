#include "DMC.h"
#include <iostream>
#include <queue>

/**
//---------------------------------------------------------------------------&
// P U B L I C
//---------------------------------------------------------------------------&
*/
//&---------------------------------------------------------------------------&
//& DMC: Constructor de la clase DMC
//&---------------------------------------------------------------------------&
DMC::DMC()
{
    //Creamos el estado inicial del modelo
    crearArbolInicial();
}


//&---------------------------------------------------------------------------&
//& ~DMC: Destructor de la clase DMC
//&---------------------------------------------------------------------------&
DMC::~DMC()
{
    std::list<Estado*>::iterator iter;
    iter = estados.begin();

    //Recorro todos los estados y llamo a su destructor
    while(iter != estados.end())
    {
        (*iter)->~Estado();

        delete (*iter);

        iter++;
    }

    estados.clear();
}

//&---------------------------------------------------------------------------&
//& getFrecuencias: Devolvemos las frecuencias de los siguientes estados
//&---------------------------------------------------------------------------&
int* DMC::getFrecuencias()
{
    return estadoActual->getFrecuencias();
}

//&---------------------------------------------------------------------------&
//& actualizarModelo: En este metodo nos encargamos de actualizar todo el
//& modelo (clonaciones, frecuencias, controles, etc..)
//&---------------------------------------------------------------------------&
void DMC::actualizarModelo(Direccion direccion)
{
    //Aumentamos la frecuencia en la direccion dada
    estadoActual->aumentarFrecuencia(direccion,1);

    //Controlamos la cantidad de estados creados
    if(cantidadEstadosCreados > LIMITE_ESTADOS)
        {
            this->~DMC();
            crearArbolInicial();
            return;
        }

    //Validamos si es necesario clonar
    if(hayQueClonar(direccion))
    {
        clonarEstado(direccion);
        return;
    }

    //movemos el estado actual
    estadoActual = estadoActual->getEstadoSiguiente(direccion);
}


/**
//---------------------------------------------------------------------------&
// P R I V A T E
//---------------------------------------------------------------------------&
*/

//&---------------------------------------------------------------------------&
//& crearArbolInicial: Creamos el arbol inicial con el que comenzamos a operar
//& Las correlaciones en los archivos se dan mas frecuentemente entre bytes que
//& entre bits. Un posible modelo seria armar un arbol binario con 64 estados,
//& y asi tendriamos todas las posibles combinaciones entre 00,01,10,11
//&---------------------------------------------------------------------------&
void DMC::crearArbolInicial()
{
    const Nombre limite = 21;

    std::queue<Estado*> colaDeEstados;
    Estado *raiz;
    Estado *est;
    Estado *aux;

    //Inicializamos la cantidad de estados
    cantidadEstadosCreados = 1;

    //Creamos la raiz del arbol
    raiz = new Estado( cantidadEstadosCreados );

    //Agregamos la raiz a la cola y comenzamos a procesar
    colaDeEstados.push(raiz);

    while(!colaDeEstados.empty())
    {
        //Leemos el estado de la cola
        est = (Estado*) colaDeEstados.front();
        colaDeEstados.pop();

        //Agregamos los 4 estados siguientes posibles
        for(int i = 0; i<4 ; i++)
        {
            aux = new Estado(++cantidadEstadosCreados);

            est->setEstadoSiguiente(i,aux);
            estados.push_back(aux);
        }

        //En caso de llegar al limite fijado, dejamos de agregar nuevos estados,
        //y referenciamos cada hoja a la raiz
        if(cantidadEstadosCreados < limite)
            for(int i = 0; i<4 ; i++)
                colaDeEstados.push(est->getEstadoSiguiente(i));
        else
            for(int i = 0; i<4 ; i++)
                for(int j = 0; j<4; j++)
                    est->getEstadoSiguiente(i)->setEstadoSiguiente(j,raiz);
    }

    //Dejamos la raiz del arbol como estado inicial
    estadoActual = raiz;
}


//&---------------------------------------------------------------------------&
//& hayQueClonar: En este metodo validamos si es necesario hacer la clonacion:
//&
//& El Estado candidato se clona si y sólo si el número de transiciones
//& observadas desde el estado actual al estado candidato es mayor que MIN_CNTI,
//& y el número de transiciones observadas desde todos los estados (con
//& excepción del estado actual) en el Estado candidato es mayor que MIN_CNT2.
//&---------------------------------------------------------------------------&
bool DMC::hayQueClonar(Direccion direccion)
{
    int cantVisitas; //Cantidad de visitas que tuvo el nodo a clonar
    int cantVisitasOtrosEstados; //Cantidad de visitas desde otros nodos

    //Buscamos la cantidad de veces que fue visitado el nodo a clonar
    cantVisitas = estadoActual->getFrecuencia(direccion);

    //Calculamos las visitas que tuvo el nodo a clonar, sin tener en cuenta el actual
    cantVisitasOtrosEstados = cantVisitas - estadoActual->getFrecuencia(direccion);

    return (cantVisitas > MIN_CNT1 && cantVisitasOtrosEstados > MIN_CNT2);
}


//&---------------------------------------------------------------------------&
//& clonarEstado: En este metodo realizamos la clonacion del estado
//&---------------------------------------------------------------------------&
void DMC::clonarEstado(Direccion direccion)
{
    double ratio;

    Estado *sigEst = estadoActual->getEstadoSiguiente(direccion);

    //Creamos un nuevo estado
    Estado *nvoEst = new Estado(++cantidadEstadosCreados);
    estados.push_back(nvoEst);

    //Direccionamos el estado actual al nuevo estado
    estadoActual->setEstadoSiguiente(direccion, nvoEst);

    //Realizamos una ponderacion para distribuir las frecuencias
    ratio = estadoActual->getFrecuencia(direccion) /
            sigEst->getCantidadVisitas();

    //Actualizamos las frecuencias de acuerdo a la ponderacion calculada
    for( short dir = 0; dir<4; dir++)
    {

        //El nuevo nodo hereda la misma salida que el original
        nvoEst->setEstadoSiguiente(dir, sigEst->getEstadoSiguiente(dir));

        //Distribuimos las frecuencias
        nvoEst->setFrecuencia(dir,ratio*sigEst->getFrecuencia(dir));
        sigEst->setFrecuencia(dir,sigEst->getFrecuencia(dir) - nvoEst->getFrecuencia(dir));
    }

    //Movemos al estado actual
    estadoActual = nvoEst;
}

