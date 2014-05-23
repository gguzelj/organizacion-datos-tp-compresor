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
        int*    getFrecuencias();
        void    actualizarModelo(Direccion direccion);

//&---------------------------------------------------------------------------&
//& P R I V A T E
//&---------------------------------------------------------------------------&
    private:

        /** Atributos   */
        Estado*             estadoActual;
        std::list<Estado*>  estados;
        Nombre              cantidadEstadosCreados;

        /** Metodos     */
        void crearArbolInicial();
        bool hayQueClonar(Direccion direccion);
        void clonarEstado(Direccion direccion);
        void verificarFrecuenciasDelEstado();

};
#endif // DMC_H
