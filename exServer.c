#include <rpc/rpc.h>

#include "tools.h"

// Interface gerada pelo RPCGen a partir da IDL (ex.h) especificada
#include "ex.h"

long int *conter;
char frase[2000];
retChat *retorn = NULL;
char **usersNames;
int nUsers = 0;
int nUsersVetor = 10;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char **retfrase_1_svc(void *a, struct svc_req *req) {
  static char msg[2000];
  static char *p;

  printf("retfrase (%s)\n", frase);
  p = frase;

  return (&p);
}

retChat *retfrase_2_svc(long int *a, struct svc_req *req) {
  FILE *fileRead;
  char delimiter = '-';
  char filename[100];
  char *msgTmp;
  int contUser = 0;

  if (conter == NULL) {
    long int p = 0;
    conter = &p;
  }

  if (*a >= *conter) {
    return NULL;
  }

  do {
    sprintf(filename, "%s-%ld.serv", usersNames[contUser], *a);
    fileRead = fopen(filename, "r");

    if (contUser == nUsers) {
      break;
    } else {
      contUser++;
    }

  } while (fileRead == NULL);

  if (fileRead == NULL) {
    return (NULL);
  }

  strcpy(retorn->userName, usersNames[contUser]);
  retorn->msg = readline(fileRead, 0);
  retorn->index = *a;

  fclose(fileRead);

  return (retorn);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *inspalavra_1_svc(char **a, struct svc_req *req) {
  static int ret = 1;

  printf("%s (%s)\n", *a, frase);
  strcat(frase, *a);

  return (&ret);
}

int *inspalavra_2_svc(struct insPalavraParam *a, struct svc_req *req) {
  if (usersNames == NULL) {
    usersNames = (char **)malloc(sizeof(char *) * nUsersVetor);
  }

  if (conter == NULL) {
    long int p = 0;
    conter = &p;
  }

  if (nUsers == nUsersVetor) {
    nUsersVetor += 2;
    usersNames = (char **)realloc(usersNames, sizeof(char *) * nUsersVetor);
  }

  strcpy(usersNames[nUsers], a->userName);
  nUsers++;

  FILE *filewrite;
  char filenameWrite[50];

  static int ret = 1;

  sprintf(filenameWrite, "%s-%ld.serv", a->userName, *conter);
  filewrite = fopen(filenameWrite, "w");

  if (filewrite == NULL) {
    printf("Error: Na abertura dos arquivos (%s).", filenameWrite);
    static int ret2 = 0;

    return (&ret2);
  }
  writeline(a->msg, filewrite);

  fclose(filewrite);

  printf("%s\n", a->msg);

  return (&ret);
}