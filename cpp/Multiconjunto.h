#pragma once

//maximo de elementos que puede tener una celda
const int MAX_ELEMENTOS = 16; 

struct tElemento {

    //valor numerico
    int valor;

    //dice las veces que afecta ese valor, maximo 3
    int multiplicidad;
};

class MultiConjunto {
private:

    //una lista de elementos con su multiplicidad
    tElemento elementos[MAX_ELEMENTOS];

    //el numero de elementos que hay
    int num_elems;

    int buscar_pos(int valor) const;
public:
 
    //
    MultiConjunto();

  //devuelve cuantos valores distintos hay para 1
    int dame_num_elems() const;

   //dice si ese valor esta en el multiconjunto
    bool pertenece(int valor) const;

   //añade un valor
    void insertar(int valor);

    //quita un valor
    void eliminar(int valor);

                     
};
