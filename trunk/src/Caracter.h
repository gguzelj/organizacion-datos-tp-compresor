#ifndef CARACTER_H
#define CARACTER_H
#define MINIMA_FRECUENCIA 1

class Caracter
{
    public:

    /**Metodos Publicos*/
        Caracter(char unSimbolo);
        char getSimbolo();
        unsigned long int getFrecuencia();
        void setFrecuencia(unsigned long int unaFrecuencia);
        bool esIgualAlCaracter(Caracter* otroCaracter);
        virtual ~Caracter();

    protected:
    private:

    /**Atributos*/
        char simbolo;
        unsigned long int frecuencia;
};

#endif // CARACTER_H
