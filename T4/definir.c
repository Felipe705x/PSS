#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pss.h"

// Llena buffer target con el contenido que debe escribirse en el archivo (<llave>:<definicion>)
void concat(const char *key, const char *definition, char *target){
  while(*key) {
    *target = *key;
    key++;
    target++;
  }
  *target = ':';
  target++;
  while(*definition) {
    *target = *definition;
    definition++;
    target++;
  }
}

// Retorna 1 si result y ref son la misma llave, 0 en caso contrario
int same_key(const char *result, const char *ref, const int ref_len) {
  for(int i=0; i<ref_len; i++) {
    if(ref[i]!=result[i])
      return 0;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  // ERROR: Mal uso de argumentos/programa
  if(argc!=4) {
    fprintf(stderr, "Uso: ./definir <diccionario> <llave> <definicion>\n");
    exit(1);
  }
  // Abrimos archivo para lectura y escritura
  FILE *inout = fopen(argv[1],"r+");
  // ERROR: No puede abrirse el archivo
  if(inout==NULL) {
      perror(argv[1]);
      exit(1);
  }
  // Definimos algunas constantes
  fseek(inout, 0, SEEK_END); 
  const int tamArch= ftell(inout);  // Tamaño del archivo
  const int numLineas = tamArch/81; // Cantidad de lineas del archivo
  const char *llave = argv[2];     
  const int largo_llave = strlen(argv[2]);

  int llave_pos = hash_string(argv[2])%numLineas;  // posicion tentativa de inserción
  char linea[81]; // buffer con el largo de una linea
  for(int i=0; i<numLineas; i++) { // Hasta encontrar una linea vacia o determinar que el diccionario esta lleno (se sabe si se recorren numLineas sin encontrar lugar para insertar)
    fseek(inout, llave_pos*81, SEEK_SET); // Posicionamos cursor sobre la linea de inserción (tentativa) actual
    fread(linea, 81, 1, inout); // Leemos la linea
    if(linea[0]==' ') {  // Si esta vacía:
      fseek(inout, llave_pos*81, SEEK_SET); // Posicionamos cursor sobre la linea actual
      concat(llave, argv[3], linea); // Llenamos buffer "linea" con los datos que debemos escribir (<llave>:<definicion>)
      fwrite(linea, 81, 1, inout); // Insertamos la llave con su definicion
      fclose(inout); // Cerramos archivo
      return 0; // Programa termina normalmente
    }
    if(same_key(linea, llave, largo_llave)) {  // ERROR: Se intenta insertar una llave pre-existente
      fprintf(stderr, "La llave alimento ya se encuentra en el diccionario\n");
      exit(1);
    }
    llave_pos = (llave_pos+1)%numLineas; // Avanzamos de manera circular a la siguente línea
  }
  // ERROR: Si se termina el ciclo for sin inserción es porque el diccionario estaba lleno
  fprintf(stderr, "dicc-full.txt: el diccionario esta lleno\n");
  exit(1);
}