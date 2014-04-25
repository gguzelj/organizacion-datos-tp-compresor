#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

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
const char      BIT_MASK = 0x00000002;
const Direccion BITS_00 = 0;
const Direccion BITS_01 = 1;
const Direccion BITS_10 = 2;
const Direccion BITS_11 = 3;


const int MIN_CNT1 = 80;
const int MIN_CNT2 = 60;

const unsigned long LIMITE_ESTADOS = 500000;

#endif // CONSTANTES_H_INCLUDED

