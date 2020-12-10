#include <assert.h>
#include <pthread.h>
#include <rpc/rpc.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "tools.h"

// Interface gerada pelo RPCGen a partir da IDL (ex.x) especificada
#include "ex.h"

// Estrutura RPC de comunicação
CLIENT *cl = NULL;

// Mensagem enviada
struct insPalavraParam mensagem;

// Semaphoros
sem_t sClient, sMensage;

void delay(int number_of_seconds) {
  // Converting time into milli_seconds
  int milli_seconds = 1000 * number_of_seconds;

  // Storing start time
  clock_t start_time = clock();

  // looping till required time is not achieved
  while (clock() < start_time + milli_seconds)
    ;
}

void *buscaMensagensEPrint() {
  FILE *fileWrite;
  char filename[50];
  long int contador = 0;

  int printD = 0;

  while (1) {
    sem_wait(&sMensage);
    sem_wait(&sClient);
    if (mensagem.userName != NULL && cl != NULL) {
      sem_post(&sMensage);
      sem_post(&sClient);
      break;
    }
    sem_post(&sMensage);
    sem_post(&sClient);
  };

  while (1) {
    sem_wait(&sClient);
    struct retChat *retorno = retfrase_2(&contador, cl);
    sem_post(&sClient);

    // printf("th-%i\t%ld\n", printD++, contador);

    if (retorno == NULL) {
      delay(1);

      // printf("th-%i\t%ld\n", printD++, contador);
    } else {
      printf("%s: ", retorno->userName);
      printf("%s \n\n", retorno->msg);

      sem_wait(&sMensage);
      int comp = strcmp(retorno->userName, mensagem.userName);
      sem_post(&sMensage);

      printf("th-%i\t%ld\n", printD++, contador);

      if (comp != 0) {
        sprintf(filename, "%s-%ld.chat", retorno->userName, retorno->index);
        fileWrite = fopen(filename, "w");

        if (fileWrite != NULL) {
          writeline(retorno->msg, fileWrite);

          fclose(fileWrite);
        } else {
          printf("Error: Na abertura dos arquivos (%s).", filename);
        }

        printf("th-%i\t%ld\n", printD++, contador);
      }

      contador++;
    }
  }
}

int main(int argc, char *argv[]) {
  FILE *fileRead;
  char *msg = NULL;
  char filename[50];

  sem_init(&sClient, 0, 0);
  sem_init(&sMensage, 0, 0);

  struct insPalavraParam mensagem;

  // Verificação dos parâmetros oriundos do console
  if (argc != 4) {
    printf("ERRO: ./client <hostname> <filename> <userName>\n");
    exit(1);
  }

  sprintf(filename, "%s.client", argv[2]);
  fileRead = fopen(filename, "r");

  if (fileRead == NULL) {
    printf("Error: Na abertura dos arquivos (%s).", filename);
    exit(1);
  }

  // retorno das funções
  int *ret = NULL;

  cl = clnt_create(argv[1], PALAVRA_PROG, PALAVRA_VERSION2, "tcp");

  if (cl == NULL) {
    clnt_pcreateerror(argv[1]);
    exit(1);
  }
  sem_post(&sClient);
  printf("CONEXÃO REALIZADA COM SUCESSO\n");

  pthread_t print;
  int rc = pthread_create(&print, NULL, buscaMensagensEPrint, NULL);
  assert(rc == 0);

  long int index = 256;
  int cont = 1;

  msg = readline(fileRead, 0);
  mensagem.msg = msg;
  mensagem.userName = argv[3];
  sem_post(&sMensage);

  while (*msg != 0) {
    sem_wait(&sMensage);
    mensagem.msg = msg;

    sem_wait(&sClient);
    ret = inspalavra_2(&mensagem, cl);
    sem_post(&sMensage);
    sem_post(&sClient);

    if (ret == NULL) {
      printf("Erro no server.\n");
      sem_wait(&sClient);
      clnt_perror(cl, mensagem.userName);
      sem_post(&sClient);
      exit(1);
    }

    msg = readline(fileRead, index);
    index += 256;
    cont++;
  }

  rc = pthread_join(print, NULL);
  assert(rc == 0);

  fclose(fileRead);

  return 0;
}