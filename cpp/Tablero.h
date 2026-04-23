#pragma once
#include <iostream>
#include "Celda.h"
using namespace std;

//este es el tamaño maximo del tablero
const int MAX = 16;

class Tablero
{
private:
	//guarda la dimension real que tendra la matriz y aunque la matriz
	// siempre sea de max x max, sera de dimension x dimension los valores que coja
	int dimension = 0;
	Celda matriz[MAX][MAX];
public:
	//pone dimension a 0
	Tablero();

	//inicia dimension para saber el tamaño de l sudoku
	Tablero(int dim);

	//devuelve la dimension
	int dame_dimension() const;

	//devuelve la cerda que hay en una posicion
	Celda dame_celda(int f, int c) const;

	//modifica y coloca una celda en una posicion
	void set_celda(int f, int c, Celda celda);

};