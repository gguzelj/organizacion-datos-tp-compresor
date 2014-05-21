#include "src/Compresor.h"

int main(int argc, char* argv[])
{
    Compresor *compresor = new Compresor();
    compresor->ejecutar(argc, argv);
}

/** EJEMPLO PARA USAR EL FileManagerOutput
    std::bitset<8> byte;

    FileManagerOutput *ar = new FileManagerOutput();

    if ( ar->open("COPIA.txt",std::ios_base::in | std::ios_base::out | std::ios_base::trunc) == ERROR_APERTURA_ARCHIVO)
        std::cout << "ERROR APERTURA";

    byte[0] = 0;
    byte[1] = 1;
    byte[2] = 0;
    byte[3] = 0;
    byte[4] = 1;
    byte[5] = 1;
    byte[6] = 0;
    byte[7] = 0;

    ar->escribirByte( byte );
    ar->close();
*/
