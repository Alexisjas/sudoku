#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <crtdbg.h>

#include "Checkml.h"
#include "ReglasSudoku.h"
#include "ListaSudokus.h"

using namespace std;

void cargar_lista_sudokus(ListaSudokus& lista);
void cargar_lista_partidas(ListaSudokus& lista);
void guardar_sudoku(ofstream& archivo, const ReglasSudoku& sudoku);
void guardar_lista_partidas(const ListaSudokus& lista);
bool jugar_sudoku(ReglasSudoku& juego);
bool resolver_sudoku(ReglasSudoku& sudoku, int fila, int columna);
void mostrar_bloqueo(ReglasSudoku& juego);
void mostrar_menu();
void mostrar_tablero(ReglasSudoku& juego);

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    ListaSudokus lista_sudokus;
    ListaSudokus lista_partidas;

    bool fin_aplicacion = false; // dice cuando termina la aplicacion

    // carga los sudokus nuevos y las partidas guardadas
    cargar_lista_sudokus(lista_sudokus);
    cargar_lista_partidas(lista_partidas);

    // se repite hasta que el usuario abandone la aplicacion
    while (!fin_aplicacion) {
        char opcion_inicio;

        cout << "Partida nueva (N), continuar partida (C) o abandonar la aplicacion (A)? ";
        cin >> opcion_inicio;

        if (opcion_inicio == 'A' || opcion_inicio == 'a') {
            // se guarda la lista de partidas antes de salir
            guardar_lista_partidas(lista_partidas);
            fin_aplicacion = true;
        }
        else if (opcion_inicio == 'N' || opcion_inicio == 'n') {
            // partida nueva, se elige un sudoku de la lista de sudokus
            if (lista_sudokus.dame_num_elems() == 0) {
                cout << "Error: no hay sudokus disponibles." << endl;
            }
            else {
                lista_sudokus.mostrar_lista();

                int elegido;
                cout << "Elige un sudoku: ";
                cin >> elegido;

                int pos = elegido - 1;

                if (pos < 0 || pos >= lista_sudokus.dame_num_elems()) {
                    cout << "Error: sudoku incorrecto." << endl;
                }
                else {
                    // se copia el sudoku elegido para jugar con el
                    ReglasSudoku juego(lista_sudokus.dame_sudoku(pos));

                    int opcion_sudoku;
                    cout << "1.- ver un sudoku" << endl;
                    cout << "2.- jugar un sudoku" << endl;
                    cout << "Elige opcion: ";
                    cin >> opcion_sudoku;

                    if (opcion_sudoku == 1) {
                        // solo se muestra el sudoku
                        mostrar_tablero(juego);
                    }
                    else if (opcion_sudoku == 2) {
                        // se juega al sudoku
                        bool resuelto = jugar_sudoku(juego);

                        // si era una partida nueva y no se ha resuelto, se guarda
                        if (!resuelto) {
                            lista_partidas.insertar(juego);
                        }
                    }
                    else {
                        cout << "Opcion incorrecta." << endl;
                    }
                }
            }
        }
        else if (opcion_inicio == 'C' || opcion_inicio == 'c') {
            // continuar partida, se elige una partida guardada
            if (lista_partidas.dame_num_elems() == 0) {
                cout << "No hay partidas guardadas. Debes elegir una partida nueva." << endl;

                if (lista_sudokus.dame_num_elems() == 0) {
                    cout << "Error: no hay sudokus disponibles." << endl;
                }
                else {
                    lista_sudokus.mostrar_lista();

                    int elegido;
                    cout << "Elige un sudoku: ";
                    cin >> elegido;

                    int pos = elegido - 1;

                    if (pos < 0 || pos >= lista_sudokus.dame_num_elems()) {
                        cout << "Error: sudoku incorrecto." << endl;
                    }
                    else {
                        // se copia el sudoku elegido para jugar con el
                        ReglasSudoku juego(lista_sudokus.dame_sudoku(pos));

                        int opcion_sudoku;
                        cout << "1.- ver un sudoku" << endl;
                        cout << "2.- jugar un sudoku" << endl;
                        cout << "Elige opcion: ";
                        cin >> opcion_sudoku;

                        if (opcion_sudoku == 1) {
                            // solo se muestra el sudoku
                            mostrar_tablero(juego);
                        }
                        else if (opcion_sudoku == 2) {
                            // se juega al sudoku
                            bool resuelto = jugar_sudoku(juego);

                            // si era una partida nueva y no se ha resuelto, se guarda
                            if (!resuelto) {
                                lista_partidas.insertar(juego);
                            }
                        }
                        else {
                            cout << "Opcion incorrecta." << endl;
                        }
                    }
                }
            }
            else {
                lista_partidas.mostrar_lista();

                int elegido;
                cout << "Elige una partida: ";
                cin >> elegido;

                int pos = elegido - 1;

                if (pos < 0 || pos >= lista_partidas.dame_num_elems()) {
                    cout << "Error: partida incorrecta." << endl;
                }
                else {
                    // se copia la partida elegida para jugar con ella
                    ReglasSudoku juego(lista_partidas.dame_sudoku(pos));

                    int opcion_sudoku;
                    cout << "1.- ver un sudoku" << endl;
                    cout << "2.- jugar un sudoku" << endl;
                    cout << "Elige opcion: ";
                    cin >> opcion_sudoku;

                    if (opcion_sudoku == 1) {
                        // solo se muestra la partida
                        mostrar_tablero(juego);
                    }
                    else if (opcion_sudoku == 2) {
                        // se juega la partida
                        bool resuelto = jugar_sudoku(juego);

                        // se elimina la partida antigua
                        lista_partidas.eliminar(pos);

                        // si no se ha resuelto, se vuelve a insertar actualizada
                        if (!resuelto) {
                            lista_partidas.insertar(juego);
                        }
                    }
                    else {
                        cout << "Opcion incorrecta." << endl;
                    }
                }
            }
        }
        else {
            cout << "Opcion incorrecta." << endl;
        }
    }

    cout << "\033[5;92m" << "Hasta luego." << "\033[0m" << endl;

    return 0;
}

// le muesta al jugador el tablero
void mostrar_tablero(ReglasSudoku& juego) {
    int dim = juego.dame_dimension();
    int n = (int)sqrt((double)dim);

    // estos son los numeritos de las columnas
    cout << "    ";
    for (int c = 1; c <= dim; c++) {
        cout << c << " ";

        // esto es la linea de los cuadraditos
        if (c % n == 0 && c != dim) {
            cout << "| ";
        }
    }
    cout << endl;

    for (int f = 1; f <= dim; f++) {
        // la linea de los cuadraditos en horizontal
        if ((f - 1) % n == 0) {
            cout << "   +-";

            for (int c = 1; c <= dim; c++) {
                cout << "--";

                if (c % n == 0 && c != dim) {
                    cout << "+-";
                }
            }

            cout << endl;
        }

        cout << f << "  | ";

        for (int c = 1; c <= dim; c++) {
            Celda celda = juego.dame_celda(f, c);

            if (celda.es_vacia()) {
                // aqui meto una comprobacion para ponerle color rojo si se bloquea una celda
                bool esta_bloqueada = false;
                int p = 1;

                while (p <= juego.dame_num_celdas_bloqueadas() && !esta_bloqueada) {
                    int fb, cb;
                    juego.dame_celda_bloqueada(p, fb, cb);

                    if (fb == f && cb == c) {
                        esta_bloqueada = true;
                    }

                    p++;
                }

                if (esta_bloqueada) {
                    cout << "\033[91m" << "X" << "\033[0m" << " ";
                }
                else {
                    cout << ". ";
                }
            }
            else if (celda.es_original()) {
                cout << "\033[1;33m" << celda.dame_valor() << "\033[0m" << " ";
            }
            else {
                cout << "\033[96m" << celda.dame_valor() << "\033[0m" << " ";
            }

            if (c % n == 0 && c != dim) {
                cout << "| ";
            }
        }

        cout << endl;
    }

    cout << endl;
}

// te da las opciones
void mostrar_menu() {
    cout << "1.- poner numero" << endl;
    cout << "2.- quitar numero" << endl;
    cout << "3.- reset" << endl;
    cout << "4.- posibles valores de una celda vacia" << endl;
    cout << "5.- autocompletar celdas con valor unico" << endl;
    cout << "6.- resolver el sudoku" << endl;
    cout << "7.- salir" << endl;
    cout << "Elige una opcion: ";
}

// te enseña las bloqueadas
void mostrar_bloqueo(ReglasSudoku& juego) {
    cout << "\033[91m" << "Sudoku bloqueado.....Las casillas bloqueadas son: " << "\033[0m";

    for (int p = 1; p <= juego.dame_num_celdas_bloqueadas(); p++) {
        int f, c;
        juego.dame_celda_bloqueada(p, f, c);

        cout << "\033[91m" << "(" << f << "," << c << ") " << "\033[0m";
    }

    cout << endl;
}

void cargar_lista_sudokus(ListaSudokus& lista) {
    ifstream archivo_lista("lista_sudokus.txt");

    if (!archivo_lista.is_open()) {
        cout << "Error: no se puede abrir lista_sudokus.txt." << endl;
    }
    else {
        int n;
        archivo_lista >> n;

        for (int i = 0; i < n; i++) {
            string nombre;
            archivo_lista >> nombre;

            ifstream archivo_sudoku(nombre);

            if (!archivo_sudoku.is_open()) {
                cout << "Error: no se puede abrir " << nombre << "." << endl;
            }
            else {
                ReglasSudoku sudoku;

                sudoku.carga_sudoku(archivo_sudoku);
                lista.insertar(sudoku);

                archivo_sudoku.close();
            }
        }

        archivo_lista.close();
    }
}

void cargar_lista_partidas(ListaSudokus& lista) {
    ifstream archivo("lista_partidas.txt");

    if (!archivo.is_open()) {
        cout << "Error: no se puede abrir lista_partidas.txt." << endl;
    }
    else {
        int n;
        archivo >> n;

        for (int i = 0; i < n; i++) {
            ReglasSudoku sudoku;

            // primero carga el tablero original de la partida
            sudoku.carga_sudoku(archivo);

            int fila;
            archivo >> fila;

            // despues carga las posiciones que habia puesto el jugador
            while (fila != -1) {
                int columna, valor;
                archivo >> columna >> valor;

                sudoku.pon_valor(fila, columna, valor);

                archivo >> fila;
            }

            lista.insertar(sudoku);
        }

        archivo.close();
    }
}

bool resolver_sudoku(ReglasSudoku& sudoku, int fila, int columna) {
    bool resuelto = false;

    int dim = sudoku.dame_dimension();

    // caso base de la recursion:
    // si fila == dim significa que ya hemos pasado la ultima fila
    // por tanto, hemos conseguido rellenar todo el sudoku
    if (fila == dim) {
        resuelto = true;
    }
    else {
        // calculamos cual sera la siguiente celda
        // normalmente pasamos a la columna siguiente
        int siguiente_fila = fila;
        int siguiente_columna = columna + 1;

        // si nos salimos por la derecha del tablero,
        // volvemos a la columna 0 y bajamos una fila
        if (siguiente_columna == dim) {
            siguiente_columna = 0;
            siguiente_fila++;
        }

        // ojo:
        // fila y columna van de 0 a dim - 1 en la recursion
        // pero dame_celda usa posiciones de 1 a dim
        Celda celda = sudoku.dame_celda(fila + 1, columna + 1);

        // si la celda no esta vacia, no podemos tocarla
        // entonces simplemente intentamos resolver desde la siguiente celda
        if (!celda.es_vacia()) {
            resuelto = resolver_sudoku(sudoku, siguiente_fila, siguiente_columna);
        }
        else {
            // si la celda esta vacia, probamos valores desde 1 hasta dim
            int valor = 1;

            // paramos si ya hemos encontrado solucion
            // o si ya hemos probado todos los valores
            while (valor <= dim && !resuelto) {
                // antes de poner un valor, comprobamos si se puede poner
                // segun fila, columna y caja
                if (sudoku.es_valor_posible(fila + 1, columna + 1, valor)) {
                    // ponemos el valor provisionalmente
                    sudoku.pon_valor(fila + 1, columna + 1, valor);

                    // ahora intentamos resolver el resto del sudoku
                    resuelto = resolver_sudoku(sudoku, siguiente_fila, siguiente_columna);

                    // si despues de poner ese valor no se consigue resolver,
                    // significa que ese valor llevaba a un camino incorrecto
                    // entonces lo quitamos y probamos el siguiente
                    if (!resuelto) {
                        sudoku.quita_valor(fila + 1, columna + 1);
                    }
                }

                // probamos el siguiente valor
                valor++;
            }
        }
    }

    return resuelto;
}

bool jugar_sudoku(ReglasSudoku& juego) {
    bool fin = false;       // dice cuando termina el juego
    bool resuelto = false;  // dice si el sudoku se ha resuelto

    // te enseña el sudoku
    mostrar_tablero(juego);

    // esto es el juego, se repite el bucle hasta que se completa el juego o se salen
    while (!fin) {
        mostrar_menu();

        int opcion;
        cin >> opcion;

        if (opcion == 1) {
            // poner un numero
            int f, c, v;
            int dim = juego.dame_dimension();

            cout << "Fila y columna entre 1..." << dim << ": ";
            cin >> f >> c;

            cout << "Valor: ";
            cin >> v;

            // si el valor no es posible avisamos
            if (!juego.pon_valor(f, c, v)) {
                cout << "\033[31m" << "Error: no se puede colocar ese valor en esa posicion." << "\033[0m" << endl;
            }

            mostrar_tablero(juego);

            // si se bloquea se avisa
            if (juego.bloqueo()) {
                mostrar_bloqueo(juego);
            }
        }
        else if (opcion == 2) {
            // quitar numero
            int f, c;
            int dim = juego.dame_dimension();

            cout << "Fila y columna entre 1..." << dim << ": ";
            cin >> f >> c;

            // si no se puede quitar se avisa
            if (!juego.quita_valor(f, c)) {
                cout << "Error: no se puede quitar el valor de esa posicion." << endl;
            }

            mostrar_tablero(juego);
        }
        else if (opcion == 3) {
            // se resetea
            juego.reset();
            mostrar_tablero(juego);
        }
        else if (opcion == 4) {
            // se ven los valores posibles
            int f, c;
            int dim = juego.dame_dimension();

            cout << "Fila y columna entre 1..." << dim << ": ";
            cin >> f >> c;

            // se ve que sea valido y que este vacia
            if (f < 1 || f > dim || c < 1 || c > dim) {
                cout << "Error: posicion incorrecta." << endl;
            }
            else if (!juego.dame_celda(f, c).es_vacia()) {
                cout << "Error: la celda no esta vacia." << endl;
            }
            else {
                // muestra todos los valores que se pueden poner
                cout << "Los valores posibles para la celda son: { ";

                for (int v = 1; v <= dim; v++) {
                    if (juego.es_valor_posible(f, c, v)) {
                        cout << v << " ";
                    }
                }

                cout << "}" << endl;
            }

            mostrar_tablero(juego);
        }
        else if (opcion == 5) {
            // autocompleta
            juego.autocompletar();
            mostrar_tablero(juego);

            // si hay bloqueo avisa
            if (juego.bloqueo()) {
                mostrar_bloqueo(juego);
            }
        }
        else if (opcion == 6) {
            // resuelve el sudoku con backtracking
            if (resolver_sudoku(juego, 0, 0)) {
                mostrar_tablero(juego);

                cout << "\033[5;92m" << "Sudoku resuelto." << "\033[0m" << endl;

                resuelto = true;
                fin = true;
            }
            else {
                cout << "No se ha podido resolver el sudoku." << endl;
            }
        }
        else if (opcion == 7) {
            // salirse de la partida
            fin = true;
        }
        else {
            // no existe mas que 7
            cout << "Opcion incorrecta." << endl;
        }

        // comprobamos si esta terminado tras cada movimiento
        if (!fin && juego.terminado()) {
            cout << "\033[5;92m" << "¡Enhorabuena! Has resuelto el Sudoku." << "\033[0m" << endl;

            resuelto = true;
            fin = true;
        }
    }

    return resuelto;
}

void guardar_sudoku(ofstream& archivo, const ReglasSudoku& sudoku) {
    int dim = sudoku.dame_dimension();

    // primero guardamos la dimension
    archivo << dim << endl;

    // guardamos el tablero original
    // las celdas originales se guardan con su valor
    // las celdas puestas por el jugador se guardan como 0
    for (int f = 1; f <= dim; f++) {
        for (int c = 1; c <= dim; c++) {
            Celda celda = sudoku.dame_celda(f, c);

            if (celda.es_original()) {
                archivo << celda.dame_valor() << " ";
            }
            else {
                archivo << 0 << " ";
            }
        }

        archivo << endl;
    }

    // ahora guardamos las celdas ocupadas por el jugador
    // formato: fila columna valor
    for (int f = 1; f <= dim; f++) {
        for (int c = 1; c <= dim; c++) {
            Celda celda = sudoku.dame_celda(f, c);

            if (celda.es_ocupada()) {
                archivo << f << " " << c << " " << celda.dame_valor() << endl;
            }
        }
    }

    // marca el final de las posiciones ocupadas
    archivo << -1 << endl;
}

void guardar_lista_partidas(const ListaSudokus& lista) {
    ofstream archivo("lista_partidas.txt");

    if (!archivo.is_open()) {
        cout << "Error: no se puede guardar la lista de partidas." << endl;
    }
    else {
        // primero se guarda el numero de partidas
        archivo << lista.dame_num_elems() << endl;

        // despues se guarda cada partida
        for (int i = 0; i < lista.dame_num_elems(); i++) {
            guardar_sudoku(archivo, lista.dame_sudoku(i));
        }

        archivo.close();
    }
}