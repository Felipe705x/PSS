#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pss.h"
       
// Defina aca la estructura para guardar nombre y tamanno

typedef struct {
  char *name;
  long int size;
} fileData;

// Agregue aca las funciones y variables globales adicionales que necesite

Queue *q;

void listDir_queue(char *nom) { // Version encoladora de listDir
  struct stat st_nom;
  int rc;
  rc= stat(nom, &st_nom);

  if (rc!=0) {
    printf("%s no existe\n", nom);
    exit(0);
  }
  
  if (S_ISREG(st_nom.st_mode)) {
    // Es un archivo regular
    // No se hace nada en list-dir.bin
    fileData *data = malloc(sizeof(fileData));
    data->name = strdup(nom);
    data->size = st_nom.st_size;
    put(q, data);
  }
  else if (S_ISDIR(st_nom.st_mode)) {
    // Es un directorio
    DIR *dir = opendir(nom);
    if (dir == NULL) {
      perror(nom);
      exit(1);
    }
    for (struct dirent *entry = readdir(dir);
         entry != NULL;
         entry = readdir(dir)) {
      if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) {
        continue;
      }
      char *nom_arch= malloc(strlen(nom)+strlen(entry->d_name)+2);
      strcpy(nom_arch, nom);
      strcat(nom_arch, "/");
      strcat(nom_arch, entry->d_name);
      listDir_queue(nom_arch);
      free(nom_arch);
    }
    closedir(dir);
  }
  else {
    // Podria ser un dispositivo, un link simbolico, etc.
    fprintf(stderr, "Archivo %s es de tipo desconocido\n", nom);
    exit(1);
  }
}

int fileCompare(void *ptr, int i, int j) {
  int difference;
  fileData **array = (fileData**) ptr;
  difference = (array[j]->size) - (array[i]->size);
  if (difference == 0) { // Comparamos lexicograficamente
    difference = strcmp(array[i]->name, array[j]->name);
  }
  return difference;
}

int main(int argc, char *argv[]) {
  if (argc!=3) {
    fprintf(stderr, "uso: %s <arch|dir> <cantidad_archs>\n", argv[0]);
    exit(1);
  }
  q = makeQueue();
  listDir_queue(argv[1]);
  int q_size = queueLength(q);
  fileData *array[q_size];
  for(int i=0;i<q_size;i++) 
    array[i] = (fileData*) get(q);
  sortPtrArray(array, 0, q_size-1, fileCompare);
  int n_biggest = atoi(argv[2]);
  for(int i=0;i<q_size;i++) {
    if(i<n_biggest) {
      fileData a_file = *array[i];
      printf("%s %lu\n",a_file.name,a_file.size);
    }
    free(array[i]->name);
    free(array[i]);
  }
  destroyQueue(q);
  return 0;
}
