#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <climits>

#include "src/DMC.h"
#include "src/Estado.h"

using namespace std;

int main()
{
    DMC *dmc = new DMC();

    unsigned int cont = 0;

    dmc->getFrecuencias();

    for(;;)
    {
        //Simulamos bits
        dmc->actualizarModelo(rand() % 4);

        if(cont++ == 10000000)
            break;
    }
}

