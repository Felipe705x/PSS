#include <stdio.h>

#include "suma.h"

Bcd sumaBcd(Bcd x, Bcd y) {
  const Bcd mascara = 15; /*Esta mascara en binario es 00...0001111, lo que nos permitira extraer de a 4 bits (i.e. cada digito del BCD)*/
  unsigned char shift = 0; /*Esta variable determina a que posicion se le sumara al total "sum":
  Ejemplo: shift=0 suma a los primeros 4 bits (los de más a la derecha), shift=1 suma a los siguientes 4 bits, y shift = 2 a los siguientes 4 bits, y asi sucesivamente*/
  Bcd carry = 0; /*Indica el valor del acarreo, 0 si no hay, 1 en caso contrario*/
  Bcd sum = 0; /*La suma de ambos BCD, la cual retornaremos si no hay desborde*/
  while(x>0||y>0||carry) { // Shifteando a la derecha (>>) lo suficiente llegaremos al binario 000.000 = 0, o bien llegamos a -1 dependiendo del sistema/arquitectura (ambos son <= 0 de todas formas)
    if(shift==64) { /*Bcd es un unsigned long long, es decir que tiene 64 posiciones. Como shifteamos de a cada 4, el ciclo while correra solo 16 veces a lo más,*/
      return 0xffffffffffffffff; /*pero si se ejecuta exactamente 16 veces, habremos sumado en la posicion de 64 bits a la izquierda (shift = 4*16 = 64, en dicho caso), lo cual resulta en un desborde.*/
    }                   
    Bcd digit_sum = (x&mascara) + (y&mascara) + carry; /*x&mascara nos da los primeros 4 bits del x actual (i.e. digito más derecho en BCD), lo mismo aplica para y&mascara*/
    carry = 0; /*ya se utilizo(de haber ocurrido)el acarreo*/
    if(digit_sum >= 10) { /*si la suma da un decimal de dos digitos, nos quedamos con el digito de más a la derecha y mandamos un acarreo a la proxima iteración*/
      digit_sum-=10;
      carry = 1;
    }
    sum |= (digit_sum<<shift); /*Concatenamos al grupo de 4 bits correspondiente*/
    shift+=4;  /*Originalmente la expresión de arriba era "sum = sum|(digit_sum<<shift)", pero descubri que podia compactarse como "|=" y que quizas eso sea más rapido (y claramente mas legible)*/
    x = x>>4; /*Nos deshacemos de los 4 bits ya leidos y movemos a los primeros bits los 4 bits que vienen*/ 
    y = y>>4;
  }
  return sum; /*Si no hubo desborde, solo retornamos la suma*/
}