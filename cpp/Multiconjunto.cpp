#include "Multiconjunto.h"


MultiConjunto::MultiConjunto() {
    num_elems = 0;
}

// te dice cuantos distintos hay en el multiconjunto
int MultiConjunto::dame_num_elems() const {
    return num_elems;
}

//recorre una lista buscando ese valor, si esta da true, si no da false
bool MultiConjunto::pertenece(int valor) const {
    return buscar_pos(valor) != -1;
}

//busca si el valor existe
void MultiConjunto::insertar(int valor) {
    int pos = buscar_pos(valor);
    // si ya existe, no se crea uno simplemente se aumenta la multiplicidad (cuantas veces aparece)
    if (pos != -1) {
        elementos[pos].multiplicidad++;
    }
    else {
        // si no existe, se añade al final del array se guarda el valor y se pone multiplicidad 1 (aparece una vez)
        elementos[num_elems].valor = valor;
        elementos[num_elems].multiplicidad = 1;
        num_elems++;
    }
}

//elimina el valor del multiconjunto, baja la multiplicidad y desbloquea si llega a 0 y desplazamos todos para rellenar
void MultiConjunto::eliminar(int valor) {
    int pos = buscar_pos(valor);
    // si existe el valor
    if (pos != -1) {
        // se reduce su multiplicidad (una aparicion menos)
        elementos[pos].multiplicidad--;

        // si la multiplicidad llega a 0, significa que ya no queda ese valor, entonces hay que eliminarlo del array
        if (elementos[pos].multiplicidad == 0) {

            // se desplazan todos los elementos hacia la izquierda para no dejar huecos en el array

            for (int j = pos; j < num_elems - 1; j++) {
                elementos[j] = elementos[j + 1];
            }
            num_elems--;
        }
    }
}

int MultiConjunto::buscar_pos(int valor) const {
    int i = 0;
    int resultado = -1;
    // se recorre el array mientras no se haya encontrado el valor y no se haya llegado al final
    while (i < num_elems && elementos[i].valor != valor) {
        i++;
    }
    // si se ha parado antes de llegar al final, es que lo ha encontrado

    if (i < num_elems) {
        resultado = i; // devuelve la posicion donde esta
    }
    return resultado;
}