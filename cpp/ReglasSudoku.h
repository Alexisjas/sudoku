#pragma once

#include <fstream>
#include "tablero.h"
#include "multiconjunto.h"
using namespace std;

const int MAX_BLOQUEADAS = MAX * MAX;

struct tPosicion {
    int fila;
    int columna;
};

class ReglasSudoku {
private:
    Tablero tablero;
    int contador;
    int num_bloqueadas;
    tPosicion* celdas_bloqueadas[MAX_BLOQUEADAS];

    struct tValores_invalidos {
        int nFilas = 0;
        int nColumnas = 0;
        MultiConjunto no_validos[MAX][MAX];
    };

    tValores_invalidos info_valores_no_validos;

    //marca un valor como no valido para toda esa fila columna y cuadradito
    void actualizar_no_validos_poner(int fila, int columna, int valor);
    //desmarca el valor como no valido
    void actualizar_no_validos_quitar(int fila, int columna, int valor);

    // actualiza bloqueadas
    void actualizar_bloqueadas(int fila, int columna);

    //ver si una celda ya está en la lista
    bool esta_en_bloqueadas(int fila, int columna) const;
    //eliminar una celda concreta de la lista
    void eliminar_de_bloqueadas(int fila, int columna);
    //limpiar de la lista solo la zona afectada
    void limpiar_bloqueadas_zona(int fila, int columna);
    //comprobar una sola celda y añadirla si está bloqueada
    void comprobar_celda_bloqueada(int fila, int columna);

    //libera todas las bloqueadas
    void liberar_bloqueadas();
        
public:
    
    ReglasSudoku();
    ReglasSudoku(const ReglasSudoku& sudoku);
    ~ReglasSudoku();

    //constantes porque solo muestran valores y cosas
    int dame_dimension() const;
    Celda dame_celda(int fila, int columna) const;
    bool terminado() const;
    bool bloqueo() const;
    int dame_num_celdas_bloqueadas() const;
    void dame_celda_bloqueada(int p, int& fila, int& columna) const;
    bool es_valor_posible(int fila, int columna, int valor) const;

    // no constantes porque modifican
    bool pon_valor(int fila, int columna, int valor);
    bool quita_valor(int fila, int columna);
    void reset();
    void autocompletar();

    //aqui empieza y se inicia el sudoku
    void carga_sudoku(ifstream& archivo);

    ReglasSudoku& operator=(const ReglasSudoku& reglas);
};
