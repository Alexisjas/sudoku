#pragma once
#include "ReglasSudoku.h"

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

private:
	ReglasSudoku** listasudoku;
	int elementos;

};

