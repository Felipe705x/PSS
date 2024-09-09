#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "viajante.h"

int leer(int fd, void *vbuf, int n) {
  char *buf= vbuf;
  do {
    int rc= read(fd, buf, n);
    if (rc<=0)
      return 1; /* fracaso: error o fin del archivo/pipe/socket */
    n-= rc; /* descontamos los bytes leídos */
    buf+= rc; /* avanzamos el buffer para no reescribir lo leido previamente */
  } while (n>0); /* mientras no leamos todo lo que esperamos */
  return 0; /* exito */
}

double viajante_par(int z[], int n, double **m, int nperm, int p) {
  int process;
  int fds[p][2];
  int paths = nperm/p;
  int size_z = (n+1)*sizeof(int);
  for (int i=0;i<p;i++) {
    pipe(fds[i]);
    process = fork();
    srandom(getUSecsOfDay()*getpid());
    if (!process) { // proceso hijo
      close(fds[i][0]);
      double res = viajante(z,n,m,paths);
      write(fds[i][1], &res, sizeof(double)); // Escribimos distancia minima
      write(fds[i][1], &z[0], size_z); // Escribimos el arreglo
      exit(0);
    }
    else { // proceso padre
      close(fds[i][1]);
    }
  }
  double min = DBL_MAX;
  int j = 0; // almacenamos cual proceso hijo contiene el minimo (para evitar lecturas innecesarias de arreglos; solo leeremos el del optimo)
  for (int i=0;i<p;i++) {
    double min_hijo;
    leer(fds[i][0], &min_hijo, sizeof(double));
    if(min_hijo < min) {
      min = min_hijo;
      j = i;
    }
  }

  leer(fds[j][0], &z[0], size_z); // actualizamos z con el hijo del arreglo optimo
  for (int i=0;i<p;i++) // cerramos...
    close(fds[i][0]);

  return min;
}
