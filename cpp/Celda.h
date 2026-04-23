#pragma once


enum tEstado { ORIGINAL, OCUPADA, VACIA };

class Celda {
private:

    // el numero que hay en la casilla
    int valor;

    // su estado
    tEstado estado;

public:

    // crea la celda estando vacía le pone valor 0 y estado vacio
    Celda();

    // crea una celda con los valores que le digan
    Celda(int v, tEstado estado);

    // lo pongo en const porque no modifican valores

    // devuelve true si vacia, si original y si ocupada respectivamente
    bool es_vacia() const;
    bool es_original() const;
    bool es_ocupada() const;

    // devuelve el numero que tiene la celda
    int dame_valor() const;

    // aqui si modifican cosas

    // pone el valor que le digas a la celda
    void set_valor(int v);

    // cambia el estado a ocupada, original y vacia respectivamente
    void set_ocupada();
    void set_original();
    void set_vacia();


};
