#pragma once

//using namespace std;

struct Header{
  int i; // numero de colas de entrada
  int ie; // numero posiciones en cada cola de entrada
  int oe; // numero posiciones en la cola de salida
  int q; // size of colas internas
  int b; // nivel de reactivo de sangre
  int d; // nivel de reactivo de detritos
  int s; // nivel de reactivo de piel
  int sCopy;
  int bCopy;
  int dCopy;
};

//Entrada de una bandeja de entrada
struct Exam {
  int id; // identificador
  int i; // cola de entrada
  char k; // tipo de muestra
  int q; // cantidad de muestra
  char r; // informe final de la muestra
  int p; // tiempo de procesamiento en segundos
};
