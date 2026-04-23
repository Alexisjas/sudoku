#include <iostream>
#include <fstream>
#include "ReglasSudoku.h"

using namespace std;

// le muesta al jugador el tablero
void mostrar_tablero(ReglasSudoku& juego) {
    int dim = juego.dame_dimension();

    // estos son los numeritos de las columnas
    cout << "    ";
    for (int c = 1; c <= dim; c++) {
        cout << c << " ";
        // esto es la linea de los cuadraditos
        int n = (int)sqrt((double)dim);
        if (c % n == 0 && c != dim) {
            cout << "| ";
        }
    }
    cout << endl;

    for (int f = 1; f <= dim; f++) {
        // la linea de los cuadraditos en horizontal
        int n = (int)sqrt((double)dim);
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
    cout << "6.- salir" << endl;
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

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    ReglasSudoku juego;   
    string nombre_archivo;
    bool fin = false;     // dice cuándo termina el juego

    
    cout << "Introduce el nombre del archivo: ";
    cin >> nombre_archivo;

    // abre el archivo txt
    ifstream archivo(nombre_archivo);

    if (!archivo.is_open()) {
        //si no se abre acaba el programa
        cout << "Error: no se puede abrir el archivo." << endl;
    }
    else {
        // carga el sudoku y cierra el txt
        juego.carga_sudoku(archivo);
        archivo.close();

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

                // si el valor no es posible avismanos
                if (!juego.pon_valor(f, c, v)) {
                    cout << "\033[31m" << "Error: no se puede colocar ese valor en esa posicion." << "\033[0m" << endl;
                }
                mostrar_tablero(juego);

                // se se bloquea se avisa
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
                // se veen los valores psibles
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
                // salirse
                fin = true;

            }
            else {
                //no existe mas que 6
                cout << "Opcion incorrecta." << endl;
            }

            // comprobamos si esta terminado tras cada movimiento
            if (!fin && juego.terminado()) {
                cout << "\033[5;92m" << "¡Enhorabuena! Has resuelto el Sudoku." << "\033[0m" << endl;
                fin = true;
            }
        }

        cout << "\033[5;92m" << "Hasta luego." << "\033[0m" << endl;
    }

    return 0;
}




