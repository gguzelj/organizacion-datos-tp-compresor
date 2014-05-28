#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <iostream>
#include <fstream>
#include <bitset>
#include <list>
#include <string>
#include <queue>


/**
//----------------------------------------------------------------------------------------&
// Descripcion:
// Archivo con constantes y tipos de datos utilizados en el compresor
//----------------------------------------------------------------------------------------&
*/

/*  Tipos de datos  */
typedef std::bitset<8> Byte;
typedef unsigned char Direccion;
typedef unsigned long Nombre;
typedef int Frecuencias[4];

/*  Constantes para el compresor */
const unsigned short TECHO              =~0;
const unsigned short PISO               =0;
const unsigned short PRIMER_CUARTO      = (TECHO/4+1);
const unsigned short MEDIO              = 2 * PRIMER_CUARTO;
const unsigned short TERCER_CUARTO      = 3 * PRIMER_CUARTO;

/*  Constantes para el modelo DMC */
const Direccion BITS_00                 = 0;
const Direccion BITS_01                 = 1;
const Direccion BITS_10                 = 2;
const Direccion BITS_11                 = 3;

const int MIN_CNT1                      = 1000;
const int MIN_CNT2                      = 745;

const unsigned long LIMITE_ESTADOS      = 13107200;//40 Bytes por estado =>13107200=500Mb
const unsigned short LIMITE_FRECUENCIAS = 16383;   //Max frecuencias acumuladas(2^14 - 1)

/*  Constantes para el FileManager    */
const unsigned int TAMANO_MAX_BUFFER    = 10485760;//10Mb

const int ERROR_APERTURA_ARCHIVO        = 1;
const int ERROR_ARCHIVO_CERRADO         = 2;
const int ERROR_EOF                     = 4;

const unsigned char BIT_MASK            = 0xC0;

#endif // CONSTANTES_H_INCLUDED
