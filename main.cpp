#include "src/Compresor.h"

using namespace std;

int main(int argc, char* argv[])
{
    Compresor *compresor = new Compresor();
    compresor->ejecutar(argc, argv);
    compresor->~Compresor();
    delete compresor;
}
