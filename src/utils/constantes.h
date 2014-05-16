#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <stddef.h>

/**
//---------------------------------------------------------------------------&
// Descripcion:
// Archivo con constantes y tipos de datos utilizados en el compresor
//---------------------------------------------------------------------------&
*/

/*  Tipos de datos  */
typedef unsigned char Byte;
typedef unsigned char Direccion;
typedef unsigned long Nombre;
typedef int Frecuencias[4];

/*  Errores */

/*  Constantes  */
const Direccion BITS_00                 = 0;
const Direccion BITS_01                 = 1;
const Direccion BITS_10                 = 2;
const Direccion BITS_11                 = 3;


const int MIN_CNT1                      = 1000;
const int MIN_CNT2                      = 500;

const unsigned long LIMITE_ESTADOS      = 50000;

/*  Constantes para los archivos    */
const unsigned int TAMANO_MAX_BUFFER    = 10485760; //10Mb

const int ERROR_APERTURA_ARCHIVO        = 1;
const int ERROR_EOF                     = 4;


const unsigned char BIT_MASK            = 0xC0;

#endif // CONSTANTES_H_INCLUDED
