#pragma once

#include <semaphore.h>
#include <queue>

using namespace std;

struct exam {
  int id; // identificador
  int i; // cola de entrada
  char k; // tipo de muestra
  int q; // cantidad de muestra
  
  char r; // informe final de la muestra
  int p; // tiempo de procesamiento en segundos

  int b; // nivel de reactivo de sangre
  int d; // nivel de reactivo de detritos
  int s; // nivel de reactivo de piel

  int i_rec;// numero de colas de entrada
  int ie; // numero posiciones en cada cola de entrada
  int oe; // numero posiciones en la cola de salida
};

struct queues {
  sem_t mutex;
  int size;
  queue <exam> cola;  
};
