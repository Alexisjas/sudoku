#include "ListaSudokus.h"
#include <iostream>
using namespace std;

ListaSudokus::ListaSudokus() :capacidad(10), elementos(0), listasudoku(new ReglasSudoku * [capacidad]) {};

ListaSudokus::~ListaSudokus()
{
	for (int i = 0; i < elementos; i++)
	{
		delete listasudoku[i];
	}
	delete[] listasudoku;       // borrar el array de punteros
	elementos = 0;
}	
int ListaSudokus::dame_num_elems() const {
	return elementos;
}
const ReglasSudoku& ListaSudokus::dame_sudoku(int i) const {
	return *listasudoku[i];
}
void ListaSudokus::insertar(const ReglasSudoku& sudoku) {
	if (elementos == capacidad) {
		for (int i = 0; i < elementos; i++)
		{
			ReglasSudoku sudokunuevo;
		}
	}

}
void ListaSudokus::eliminar(int pos) {

}

void ListaSudokus::mostrar_lista() const {

}

bool operator<(const ReglasSudoku& s1, const ReglasSudoku& s2) {
	bool menorque = false;
	int vacias1 = contar_celdas_vacias(s1);
	int vacias2 = contar_celdas_vacias(s2);

	if (vacias1 < vacias2) {
		menorque = true;
	}
	if (vacias1 == vacias2) {
		int d1 = s1.dame_dimension();
		int d2 = s2.dame_dimension();
		if (d1 != d2) {
			menorque = d1 < d2;
		}
		if (d1 == d2)
		{
			int i = 1;
			bool distintos = false;
			while (!distintos && i <= d1)
			{
				if (num_celdas_con_k_posibles(s1, i) != num_celdas_con_k_posibles(s2, i))
				{
					distintos = true;
					if (num_celdas_con_k_posibles(s1, i) > num_celdas_con_k_posibles(s2, i))
					{
						menorque = true;
					}
				}
				else
				{
					i++;
				}
			}
		}
	}
	return menorque;
}

bool operator==(const ReglasSudoku& s1, const ReglasSudoku& s2) {
    return !(s1 < s2) && !(s2 < s1);
}

int contar_celdas_vacias(const ReglasSudoku& s1){
	int contador = 0;
	int dim = s1.dame_dimension();

	for (int i = 1; i <= dim; i++)
	{
		for (int j = 1; j <= dim; j++)
		{
			if (s1.dame_celda(i, j).es_vacia()) {
				contador++;
			}
		}
	}

	return contador;
}

int num_celdas_con_k_posibles(const ReglasSudoku& s1, int k) {
	int contador_celdas = 0;
	int dim = s1.dame_dimension();

	for (int i = 1; i <= dim; i++) {
		for (int j = 1; j <= dim; j++) {

			if (s1.dame_celda(i, j).es_vacia()) {

				int contador_valores_posibles = 0; // se reinicia por cada celda

				for (int l = 1; l <= dim; l++) {
					if (s1.es_valor_posible(i, j, l)) {
						contador_valores_posibles++;
					}
				}

				if (contador_valores_posibles == k) {
					contador_celdas++;
				}
			}
		}
	}

	return contador_celdas;
}