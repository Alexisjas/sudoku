#include "ReglasSudoku.h"
#include <iostream>
#include <cmath>

using namespace std;


// inicializa el juego en blanco
ReglasSudoku::ReglasSudoku() {
    contador = 0;
    num_bloqueadas = 0;
    info_valores_no_validos.nFilas = 0;
    info_valores_no_validos.nColumnas = 0;
    for (int i = 0; i < MAX_BLOQUEADAS; i++) {
        celdas_bloqueadas[i] = nullptr;
    }
}

ReglasSudoku::ReglasSudoku(const ReglasSudoku& sudoku) {
    num_bloqueadas = 0;

    for (int i = 0; i < MAX_BLOQUEADAS; i++) {
        celdas_bloqueadas[i] = nullptr;
    }

    *this = sudoku;
}

ReglasSudoku::~ReglasSudoku() {
    liberar_bloqueadas();
}


// le pregunta al tablero cuál es su dimensión y la devuelve
int ReglasSudoku::dame_dimension() const {
    return tablero.dame_dimension();
}

// le pregunta al tablero la celda que hay en esa fila y columna y la devuelve
Celda ReglasSudoku::dame_celda(int fila, int columna) const {
    return tablero.dame_celda(fila, columna);
}

//acaba cuando el numero de celdas con numero = total de celdas
bool ReglasSudoku::terminado() const {
    return contador == dame_dimension() * dame_dimension();
}

//se bloquea si el numero de celdas bloqueadas no es = 0
bool ReglasSudoku::bloqueo() const {
    return num_bloqueadas > 0;
}

// dice cuantas hay bloquedas
int ReglasSudoku::dame_num_celdas_bloqueadas() const {
    return num_bloqueadas;
}

// te da la fila y columna de la bloqueada que esta en la posición p en la lista, p va de 1 a dame_num_celdas_bloq
void ReglasSudoku::dame_celda_bloqueada(int p, int& fila, int& columna) const {
    fila = celdas_bloqueadas[p - 1]->fila;
    columna = celdas_bloqueadas[p - 1]->columna;
}


// si el valor no está en la lista de no válidos, significa que no hay ningun motivo para no ponerlo ahí.
bool ReglasSudoku::es_valor_posible(int fila, int columna, int valor) const {
    return !info_valores_no_validos.no_validos[fila - 1][columna - 1].pertenece(valor);
}



// recorre todas las celdas de la fila columna y cuadrado de donde has colocado un valor para poner ese valor en no valido
void ReglasSudoku::actualizar_no_validos_poner(int fila, int columna, int valor) {
    int dim = dame_dimension ();
    // n es el tamaño de la submatriz ej: 9x9  sqrt(9) = 3
    int n = (int)sqrt((double)dim);

    // aqui marco el valor como no válido en todas las celdas de la misma fila
    for (int c = 1; c <= dim; c++) {
        if (c != columna) {  // en la propio celda no lo hago
            info_valores_no_validos.no_validos[fila - 1][c - 1].insertar(valor);
        }
    }

    // aqui de la columna
    for (int f = 1; f <= dim; f++) {
        if (f != fila) {  // en ella no lo hago
            info_valores_no_validos.no_validos[f - 1][columna - 1].insertar(valor);
        }
    }

    // calculamos donde empieza el cuadrado, funciona para cualquier caso
    int fila_inicio = ((fila - 1) / n) * n + 1;
    int columna_inicio = ((columna - 1) / n) * n + 1;

    //se pone a no valido en toda el cuadrado
    for (int f = fila_inicio; f < fila_inicio + n; f++) {
        for (int c = columna_inicio; c < columna_inicio + n; c++) {
            if (f != fila || c != columna) {  // en ella no se marca
                info_valores_no_validos.no_validos[f - 1][c - 1].insertar(valor);
            }
        }
    }
}

//hace lo contrario, si quito un numero, desbloquea todo
void ReglasSudoku::actualizar_no_validos_quitar(int fila, int columna, int valor) {
    int dim = dame_dimension();
    int n = (int)sqrt((double)dim);

    // valido toda la fila
    for (int c = 1; c <= dim; c++) {
        if (c != columna) {
            info_valores_no_validos.no_validos[fila - 1][c - 1].eliminar(valor);
        }
    }

    // valido toda la columna
    for (int f = 1; f <= dim; f++) {
        if (f != fila) {
            info_valores_no_validos.no_validos[f - 1][columna - 1].eliminar(valor);
        }
    }

    // busco el cuadrado
    int fila_inicio = ((fila - 1) / n) * n + 1;
    int columna_inicio = ((columna - 1) / n) * n + 1;

    //valido toda la cuadradito
    for (int f = fila_inicio; f < fila_inicio + n; f++) {
        for (int c = columna_inicio; c < columna_inicio + n; c++) {
            if (f != fila || c != columna) {
                info_valores_no_validos.no_validos[f - 1][c - 1].eliminar(valor);
            }
        }
    }
    //IMPORTANTE!!!!! si en esas filas o columnas o cuadradito se bloquea por otro motivo seguira bloqueado, para eso esta el contador
}



// actualiza las celdas bloqueadas solo en la zona afectada
void ReglasSudoku::actualizar_bloqueadas(int fila, int columna) {
    int dim = dame_dimension(); // tamaño del tablero
    int n = (int)sqrt((double)dim); // tamaño del subcuadro

    // borra las bloqueadas antiguas de esa zona
    limpiar_bloqueadas_zona(fila, columna);

    // revisa todas las celdas de la fila
    for (int c = 1; c <= dim; c++) {
        comprobar_celda_bloqueada(fila, c);
    }

    // revisa todas las celdas de la columna
    for (int f = 1; f <= dim; f++) {
        comprobar_celda_bloqueada(f, columna);
    }

    // calcula el inicio del subcuadro
    int fila_inicio = ((fila - 1) / n) * n + 1;
    int columna_inicio = ((columna - 1) / n) * n + 1;

    // revisa todas las celdas del subcuadro
    for (int f = fila_inicio; f < fila_inicio + n; f++) {
        for (int c = columna_inicio; c < columna_inicio + n; c++) {
            comprobar_celda_bloqueada(f, c);
        }
    }
}

// comprueba si una celda ya esta en la lista de bloqueadas
bool ReglasSudoku::esta_en_bloqueadas(int fila, int columna) const {
    bool encontrada = false;
    int i = 0;

    // busca la celda en la lista
    while (i < num_bloqueadas && !encontrada) {
        if (celdas_bloqueadas[i]->fila == fila &&
            celdas_bloqueadas[i]->columna == columna) {
            encontrada = true;
        }
        else {
            i++;
        }
    }

    return encontrada; // devuelve si la encontro o no
}

// elimina una celda concreta de la lista de bloqueadas
void ReglasSudoku::eliminar_de_bloqueadas(int fila, int columna) {
    int i = 0;
    bool encontrada = false;

    // busca la celda en la lista
    while (i < num_bloqueadas && !encontrada) {
        if (celdas_bloqueadas[i]->fila == fila &&
            celdas_bloqueadas[i]->columna == columna) {
            encontrada = true;
        }
        else {
            i++;
        }
    }

    // si la encuentra, la elimina desplazando el array
    if (encontrada) {
        delete celdas_bloqueadas[i];

        for (int j = i; j < num_bloqueadas - 1; j++) {
            celdas_bloqueadas[j] = celdas_bloqueadas[j + 1];
        }

        celdas_bloqueadas[num_bloqueadas - 1] = nullptr;
        num_bloqueadas--;
    }
}

// borra de la lista las bloqueadas de la fila, columna y subcuadro
void ReglasSudoku::limpiar_bloqueadas_zona(int fila, int columna) {
    int dim = dame_dimension(); // tamaño del tablero
    int n = (int)sqrt((double)dim); // tamaño del subcuadro

    // elimina bloqueadas de la fila
    for (int c = 1; c <= dim; c++) {
        eliminar_de_bloqueadas(fila, c);
    }

    // elimina bloqueadas de la columna
    for (int f = 1; f <= dim; f++) {
        eliminar_de_bloqueadas(f, columna);
    }

    // calcula el inicio del subcuadro
    int fila_inicio = ((fila - 1) / n) * n + 1;
    int columna_inicio = ((columna - 1) / n) * n + 1;

    // elimina bloqueadas del subcuadro
    for (int f = fila_inicio; f < fila_inicio + n; f++) {
        for (int c = columna_inicio; c < columna_inicio + n; c++) {
            eliminar_de_bloqueadas(f, c);
        }
    }
}

// comprueba si una celda esta bloqueada y la añade si lo esta
void ReglasSudoku::comprobar_celda_bloqueada(int fila, int columna) {
    int dim = dame_dimension(); // tamaño del tablero

    // solo tiene sentido si la celda esta vacia
    if (tablero.dame_celda(fila, columna).es_vacia()) {
        bool hay_valor_posible = false;
        int v = 1;

        // prueba todos los valores posibles
        while (v <= dim && !hay_valor_posible) {
            if (es_valor_posible(fila, columna, v)) {
                hay_valor_posible = true;
            }
            v++;
        }

        // si no hay ningun valor posible, esta bloqueada
        if (!hay_valor_posible) {
            // evita añadirla dos veces
            if (!esta_en_bloqueadas(fila, columna)) {
                celdas_bloqueadas[num_bloqueadas] = new tPosicion;
                celdas_bloqueadas[num_bloqueadas]->fila = fila;
                celdas_bloqueadas[num_bloqueadas]->columna = columna;
                num_bloqueadas++; // añade la nueva bloqueada
            }
        }
    }
}



//coloca valores, mira si existe esa columna  y fila, que el valor es del tamaño de la dimension, que esta vacia y que no rompe regla de sudoku
bool ReglasSudoku::pon_valor(int fila, int columna, int valor) {
    int dim = dame_dimension();

    // se comprieba todo de una
    bool correcto = (fila >= 1 && fila <= dim && columna >= 1 && columna <= dim && valor >= 1 && valor <= dim && tablero.dame_celda(fila, columna).es_vacia() && es_valor_posible(fila, columna, valor));

    if (correcto) {
        //si todo esta bien, la celda pasa a estar ocupada y con el valor que pone el usuario
        Celda celda(valor, OCUPADA);
        tablero.set_celda(fila, columna, celda);
        contador++;  // sumamos el contador porque ya hay una celda mas con numero

        // esto llama a la funciones de antes para terminar
        actualizar_no_validos_poner(fila, columna, valor);
        actualizar_bloqueadas(fila, columna);
    }

    return correcto;
    //este return sera util luego en el main
}

//quita un numero comprobando que no es vacia ni original y que la posicion es buena
bool ReglasSudoku::quita_valor(int fila, int columna) {
    int dim = dame_dimension();

    bool correcto = (fila >= 1 && fila <= dim &&
        columna >= 1 && columna <= dim &&
        !tablero.dame_celda(fila, columna).es_vacia() &&
        !tablero.dame_celda(fila, columna).es_original());

    if (correcto) {
        // nos quedamos el valor un segundo para comrpobar los no validos
        int valor = tablero.dame_celda(fila, columna).dame_valor();

        // ponemos la celda a vacia
        Celda celda_vacia;
        tablero.set_celda(fila, columna, celda_vacia);
        contador--;  // Hal haber un valor menos = una celda menos, quitamos una

        // igual que antes usamos funciones anteriores
        actualizar_no_validos_quitar(fila, columna, valor);
        actualizar_bloqueadas(fila, columna);
    }

    return correcto;
    //este return sera util luego
}


    //devuelve el sudoku a original, reinicia todos los multiconjuntos a vacio, las celdas ocupadas se borran y las originales se mantienen y se vuelven a poner en novlaidos
    void ReglasSudoku::reset() {
        int dim = dame_dimension();
        contador = 0;
        liberar_bloqueadas();

        // vaciamos todos los multiconjuntos
        for (int f = 1; f <= dim; f++) {
            for (int c = 1; c <= dim; c++) {
                info_valores_no_validos.no_validos[f - 1][c - 1] = MultiConjunto();
            }
        }

        // dejamos solo las originales
        for (int f = 1; f <= dim; f++) {
            for (int c = 1; c <= dim; c++) {
                Celda celda = tablero.dame_celda(f, c);

                if (celda.es_ocupada()) {
                    // las cupadas se quitan
                    Celda celda_vacia;
                    tablero.set_celda(f, c, celda_vacia);

                }
                else if (celda.es_original()) {
                    actualizar_no_validos_poner(f, c, celda.dame_valor());
                    contador++;
                }
            }
        }
    }

//ponemos directamente las celdas que solo tienen 1 numero posible, se repite hasta que: se completa, ya no hay mas con 1 valor o se bloquea
void ReglasSudoku::autocompletar() {
    int dim = dame_dimension();
    bool hubo_cambio = true;

    while (hubo_cambio && !terminado() && !bloqueo()) {
        hubo_cambio = false;

        for (int f = 1; f <= dim; f++) {
            for (int c = 1; c <= dim; c++) {
                if (tablero.dame_celda(f, c).es_vacia()) {

                    // ver cuantos valores posibles tiene una celda
                    int valor_posible = 0;
                    int num_posibles = 0;
                    for (int v = 1; v <= dim; v++) {
                        if (es_valor_posible(f, c, v)) {
                            num_posibles++;
                            valor_posible = v;  // se guarda el ultimo valor posible encontrado
                        }
                    }

                    // si solo hay un valor posible se coloca
                    if (num_posibles == 1) {
                        pon_valor(f, c, valor_posible);
                        hubo_cambio = true;  // al cambiar una celda, volvemos a seguir con el bucle
                    }
                }
            }
        }
    }
}

//carga el sudoku desde el txt
void ReglasSudoku::carga_sudoku(ifstream& archivo) {
    int dim;
    archivo >> dim;  // primero el numero de la dimension

    liberar_bloqueadas();

    // inicia el tablero con esa dimension
    tablero = Tablero(dim);
    info_valores_no_validos.nFilas = dim;
    info_valores_no_validos.nColumnas = dim;
    contador = 0;

    // vaciar no_validos
    for (int f = 1; f <= dim; f++) {
        for (int c = 1; c <= dim; c++) {
            info_valores_no_validos.no_validos[f - 1][c - 1] = MultiConjunto();
        }
    }

    // lee cada valor y lo coloca
    for (int f = 1; f <= dim; f++) {
        for (int c = 1; c <= dim; c++) {
            int v;
            archivo >> v;  //mira el siguiente

            if (v == 0) {
                // si es 0 es VACIA
                Celda celda_vacia;
                tablero.set_celda(f, c, celda_vacia);
            }
            else {
                // otro es original
                Celda celda(v, ORIGINAL);
                tablero.set_celda(f, c, celda);
                // marca como no valido ese valor en fila columna y cuadrado
                actualizar_no_validos_poner(f, c, v);
                contador++;  // ya es una celda mas que tiene valor
            }
        }
    }
}

void ReglasSudoku::liberar_bloqueadas() {
    for (int i = 0; i < num_bloqueadas; i++)
    {
        delete celdas_bloqueadas[i];
        celdas_bloqueadas[i] = nullptr;
    }
    num_bloqueadas = 0;
}

ReglasSudoku& ReglasSudoku::operator=(const ReglasSudoku& reglas) {
    if (this != &reglas) {

        liberar_bloqueadas();

        // copiar atributos
        tablero = reglas.tablero;
        contador = reglas.contador;
        num_bloqueadas = reglas.num_bloqueadas;
        info_valores_no_validos = reglas.info_valores_no_validos;

        // inicializar array
        for (int i = 0; i < MAX_BLOQUEADAS; i++) {
            celdas_bloqueadas[i] = nullptr;
        }

        // copiar bloqueadas
        for (int i = 0; i < num_bloqueadas; i++) {
            celdas_bloqueadas[i] = new tPosicion(*reglas.celdas_bloqueadas[i]);
        }
    }

    return *this;
}