#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "src/DMC.h"
#include "src/Estado.h"

using namespace std;

int main()
{
    DMC *dmc = new DMC();
    int cont = 0;
    int num;

    for(;;)
    {
        num = rand() %10;

        //Hacemos que sea mas probable el bit 11
        if( num >= 4)
        {
            dmc->actualizarModelo(BITS_11);
        } else
        {
            dmc->actualizarModelo(num);
        }

        if(++cont == 10000000)
            break;
    }

    dmc->~DMC();
    delete dmc;
}
