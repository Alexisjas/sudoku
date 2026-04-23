#include "Tablero.h"

// constructora inicial a 0
Tablero::Tablero() {
    dimension = 0;
}

// constructora con lo que pongas, celdas no se inicializan porque se inician solas a vacia
Tablero::Tablero(int dim) {
    dimension = dim;
}

// da el tamaño o dimension del tablero
int Tablero::dame_dimension() const {
    return dimension;
}


//te da una celda, se pone el -1 porque si es 9x9 en el array esa posicion es 8x8
Celda Tablero::dame_celda(int fila, int columna) const {
    return matriz[fila - 1][columna - 1];
}

//pone un valor en un hueco
void Tablero::set_celda(int fila, int columna, Celda celda) {
    matriz[fila - 1][columna - 1] = celda;
}