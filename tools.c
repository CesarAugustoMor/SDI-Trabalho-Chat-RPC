#include "tools.h"

#include <sys/time.h>
#include <time.h>

// %%%%%%%%%%%%%%% FERRAMENTAS p/ VETORES %%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + (double)t.tv_usec / 1000000;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int writeline(char *msg, FILE *file) {
  fprintf(file, "%s", msg);
  return 0;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char *readline(FILE *file, long int index) {
  long int cont = index;
  char *getLine = NULL;
  char c;
  int linha = 0;

  getLine = (char *)malloc(256 * sizeof(char));
  fseek(file, cont, SEEK_CUR);
  for (;; cont++) {
    c = fgetc(file);
    if ((linha == 256) || (c == EOF)) {
      // printf("%ld (%i)\n", cont, c);
      return (getLine);
    }
    getLine[linha++] = c;
    // printf("%ld[%d](%c)\n", cont, linha, c);
  }
  return NULL;
}
