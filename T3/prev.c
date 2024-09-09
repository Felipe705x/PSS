#include <stddef.h>

#include "prev.h"


void asignarPrev(Nodo *t, Nodo **pprev) {
    if (t != NULL) {
        Nodo *left = t->izq;
        Nodo *right = t->der;     // Empezaremos recorrido en inorden:
        asignarPrev(left, pprev);   // Primero subarbol izquierdo
        if (*pprev != NULL) {
            (*pprev)->prox = t; 
        }
        t->prox = NULL;
        t->prev = *pprev;  // La raíz del subarbol
        *pprev = t;
        asignarPrev(right, pprev);   // Luego el subarbol derecho 
    }
}
