#pragma once
#include "ReglasSudoku.h"

bool operator<(const ReglasSudoku& s1, const ReglasSudoku& s2);
bool operator==(const ReglasSudoku& s1, const ReglasSudoku& s2);


int contar_celdas_vacias(const ReglasSudoku& s1);
int num_celdas_con_k_posibles(const ReglasSudoku& s1, int k);

class ListaSudokus
{
public:
	ListaSudokus();
	~ListaSudokus();
	int dame_num_elems() const;
	const ReglasSudoku& dame_sudoku(int i) const;
	void insertar(const ReglasSudoku& sudoku);
	void eliminar(int pos); // elimina el elemento de la posición pos
	void mostrar_lista() const;

	ReglasSudoku& operator[](int indice);

private:
	ReglasSudoku** listasudoku;
	int elementos;
	int capacidad;

};

