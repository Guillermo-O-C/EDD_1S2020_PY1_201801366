#include <iostream>
#include "Ficha.h"
#include "EDD/ListaDoble_t.h"
#pragma once
using namespace std;

class jugadores
{
private:
    string name;
    int ID;
	ListaDoble<Ficha> *listaFichas;
	int PunteoTemporal;

public:
    jugadores()
	{
		this->name="";
		this->listaFichas = new ListaDoble<Ficha>();
		this->PunteoTemporal=0;
	}
    jugadores(string name)
	{
		this->name=name;
		this->listaFichas = new ListaDoble<Ficha>();
		this->PunteoTemporal=0;
	}
    string getName()
	{
		return this->name;
	}
    void setName(string name)
	{
		this->name = name;
	}
    int getID()
	{
		return this->ID;
	}
    void setID(int ID1)
	{
		this->ID = ID1;
	}
	void setFichas(ListaDoble<Ficha> *lista){
		this->listaFichas=lista;
	}
	ListaDoble<Ficha>* getFichas(){
		return this->listaFichas;
	}
	int getPunteoTemporal()
	{
		return this->PunteoTemporal;
	}
    void setPunteoTemporal(int PunteoTemporal1)
	{
		this->PunteoTemporal = PunteoTemporal1;
	}
};
