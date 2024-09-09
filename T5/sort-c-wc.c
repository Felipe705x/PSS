#include <string.h>


// Funcion auxiliar pedida en el enunciado
int word_count(const char *s) {
  int count = 0; // Count of words
  int on_space = 1;  // Not reading a word
  while(*s) {
    if(*s==' ') {
      on_space = 1;
    } else {
      count+=on_space;   // count+=1 if we were on a "space-zone", else count+=0 if we were already reading a word
      on_space = 0;      // Now we are reading a word
    }
    s++;
  }
  return count;
}

void sort(char **a, int n) {
  char **ult= &a[n-1];
  char **p= a;
  while (p<ult) {

    int t1 = word_count(p[0]) - word_count(p[1]);

    if (t1 <= 0)
      p++;
    else {
      char *tmp= p[0];
      p[0]= p[1];
      p[1]= tmp;
      p= a;
    }
  }
}


