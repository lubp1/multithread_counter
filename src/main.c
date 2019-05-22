/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

void * ehPrimo(void *i);

typedef struct {
  unsigned int N; //numero a ser passado pra funcao
  int ID;
} thread_args;

pthread_mutex_t trava;
int n_workers = 0;
pthread_t threads[3];
int worker_status[3];
int numPrimos = 0;

int main() {
  unsigned int i, j;
  thread_args* send_args;


  unsigned int num[100]; // vetor em que serão guardadas as entradas
  int numc = 0;

  while (scanf("%ud ", &i) != EOF) { //Lendo a entrada
      num[numc++] = i;
  }

  int numt = numc;
  numc = 0;



  while (numc<numt) {
    send_args = (thread_args*) malloc(sizeof(thread_args));
    send_args->N = num[numc++];
    j = 0;
    while (worker_status[j]) j = (j+1)%3; //procurando o proximo livre
    send_args->ID = j;
    pthread_mutex_lock(&trava);
    worker_status[j] = 1;
    n_workers += 1;
    pthread_mutex_unlock(&trava);
    pthread_create(&(threads[j]), NULL, ehPrimo, (void*) send_args);
  }
  for (j=0; j<3; j++) { //Esperando as threads terminarem
    pthread_join(threads[j], NULL);
  }

  printf("%d\n", numPrimos);

  return 0;
}

void * ehPrimo(void *arg) {
  thread_args* info = (thread_args *)arg;
  unsigned int i = info->N;
  unsigned int j, r = 1;
  if (i==0 || i==1)
    r = 0;
  for(j=2;j<i;j++) {
    if(!(i%j))
      r = 0;
  }
  pthread_mutex_lock(&trava);
  n_workers -= 1;
  worker_status[info->ID] = 0;
  numPrimos += r;
  pthread_mutex_unlock(&trava);
  free(info);

  return NULL;
}
