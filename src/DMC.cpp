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

        delete *iter;

        iter++;
    }

    estados.clear();
}

//&---------------------------------------------------------------------------&
//& getFrecuencias: Devolvemos las frecuencias de los siguientes estados
//&---------------------------------------------------------------------------&
int* DMC::getFrecuencias()
{
    return estadoActual->frecuencias;
}

//&---------------------------------------------------------------------------&
//& actualizarModelo: En este metodo nos encargamos de actualizar todo el
//& modelo (clonaciones, frecuencias, controles, etc..)
//&---------------------------------------------------------------------------&
void DMC::actualizarModelo(Direccion direccion)
{
    //Validamos si es necesario clonar
    if(hayQueClonar(direccion))
    {
        clonarEstado(direccion);
        return;
    }

    //Aumentamos la frecuencia en la direccion dada
    estadoActual->frecuencias[direccion] += 1;

    //Controlamos la cantidad de estados creados
    if(cantidadEstadosCreados > LIMITE_ESTADOS)
        {
            this->~DMC();
            crearArbolInicial();
            return;
        }

    //movemos el estado actual
    estadoActual = estadoActual->estadosSiguientes[direccion];
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
    estados.push_back(raiz);

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

            est->estadosSiguientes[i] = aux;
            estados.push_back(aux);
        }

        //En caso de llegar al limite fijado, dejamos de agregar nuevos estados,
        //y referenciamos cada hoja a la raiz
        if(cantidadEstadosCreados < limite)
            for(int i = 0; i<4 ; i++)
                colaDeEstados.push(est->estadosSiguientes[i]);
        else
            for(int i = 0; i<4 ; i++)
                for(int j = 0; j<4; j++)
                    est->estadosSiguientes[i]->estadosSiguientes[j] = raiz;
    }

    //Dejamos la raiz del arbol como estado inicial
    estadoActual = raiz;
}


//&---------------------------------------------------------------------------&
//& hayQueClonar: En este metodo validamos si es necesario hacer la clonacion:
//&
//& El Estado candidato se clona si y s�lo si el n�mero de transiciones
//& observadas desde el estado actual al estado candidato es mayor que MIN_CNTI,
//& y el n�mero de transiciones observadas desde todos los estados (con
//& excepci�n del estado actual) en el Estado candidato es mayor que MIN_CNT2.
//&---------------------------------------------------------------------------&
bool DMC::hayQueClonar(Direccion direccion)
{
    int cantVisitas; //Cantidad de visitas que tuvo el nodo a clonar
    int cantVisitasOtrosEstados; //Cantidad de visitas desde otros nodos

    //Buscamos la cantidad de veces que fue visitado el nodo a clonar
    cantVisitas = estadoActual->frecuencias[direccion];

    //Calculamos las visitas que tuvo el nodo a clonar, sin tener en cuenta el actual
    cantVisitasOtrosEstados = estadoActual->estadosSiguientes[direccion]->getCantidadVisitas();
    cantVisitasOtrosEstados -= cantVisitas;

    return (cantVisitas > MIN_CNT1 && cantVisitasOtrosEstados > MIN_CNT2);
}


//&---------------------------------------------------------------------------&
//& clonarEstado: En este metodo realizamos la clonacion del estado
//&---------------------------------------------------------------------------&
void DMC::clonarEstado(Direccion direccion)
{
    double ratio;

    Estado *sigEst = estadoActual->estadosSiguientes[direccion];

    //Creamos un nuevo estado
    Estado *nvoEst = new Estado(++cantidadEstadosCreados);
    estados.push_back(nvoEst);

    //Direccionamos el estado actual al nuevo estado
    estadoActual->estadosSiguientes[direccion] = nvoEst;

    //Realizamos una ponderacion para distribuir las frecuencias
    ratio = estadoActual->frecuencias[direccion] /
            sigEst->getCantidadVisitas();

    //Actualizamos las frecuencias de acuerdo a la ponderacion calculada
    for( short dir = 0; dir<4; dir++)
    {

        //El nuevo nodo hereda la misma salida que el original
        nvoEst->estadosSiguientes[dir] = sigEst->estadosSiguientes[dir];

        //Distribuimos las frecuencias
        nvoEst->frecuencias[dir] = ratio*sigEst->frecuencias[dir];
        sigEst->frecuencias[dir] = sigEst->frecuencias[dir] - nvoEst->frecuencias[dir];
    }

    //Movemos al estado actual
    estadoActual = nvoEst;
}