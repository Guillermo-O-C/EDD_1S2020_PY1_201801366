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
	ListaSimple<int> *Scores;

public:
    jugadores()
	{
		this->name="";
		this->listaFichas = new ListaDoble<Ficha>();
		this->PunteoTemporal=0;
		this->Scores=new ListaSimple<int>();
	}
    jugadores(string name)
	{
		this->name=name;
		this->listaFichas = new ListaDoble<Ficha>();
		this->PunteoTemporal=0;
		this->Scores=new ListaSimple<int>();
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
	void setFichas(ListaSimple<int> *Scores){
		this->Scores=Scores;
	}
	ListaSimple<int>* getScores(){
		return this->Scores;
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
