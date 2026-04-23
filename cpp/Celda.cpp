#include "Celda.h"
#include <iostream>
using namespace std;


//inicia la celda a 0 y a vacia
Celda::Celda(){
	valor = 0;
	estado = VACIA;
}

//asigna a la celda los valores que recibe
Celda::Celda(int v, tEstado est){
	valor = v;
	estado = est;
}

//te informa del estado de la celda y su valor
bool Celda::es_vacia() const{
	return estado == VACIA;
}
bool Celda::es_original() const{
	return estado == ORIGINAL;
}
bool Celda::es_ocupada() const{
	return estado == OCUPADA;
}
int Celda::dame_valor() const{
	return valor;
}
//estos ya cambian el valor o estado
void Celda::set_valor(int v){
	valor = v;
}
void Celda::set_ocupada(){
	estado = OCUPADA;
}
void Celda::set_original(){
	estado = ORIGINAL;
}
void Celda::set_vacia(){
	valor = 0;
	estado = VACIA;
}
