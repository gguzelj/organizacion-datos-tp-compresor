#ifndef DMC_H
#define DMC_H

#include <list>
#include "utils/constantes.h"
#include "Estado.h"

/**
//---------------------------------------------------------------------------&
// Clase:       DCM
// Descripcion: Dynamic Markov Compression. Clase encargada de representar
//              la maquina de estados finitos del metodo de Markov
//---------------------------------------------------------------------------&
*/
class DMC
{

//&---------------------------------------------------------------------------&
//& P U B L I C
//&---------------------------------------------------------------------------&
    public:

        DMC();
        virtual ~DMC();

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:

        int                 M1;
        int                 M2;
        Estado*             estadoActual;
        std::list<Estado*>  estados;


};
#endif // DMC_H
