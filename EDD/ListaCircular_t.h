#include <iostream>
#include "Nodo_t.h"
#pragma once
using namespace std;

template <class T>
class ListaCicular{
	private:
		Nodo<T> *cabeza;
		Nodo<T> *auxiliar;
		int size;
	public:
		ListaCicular(){
			this->cabeza = NULL;
			this->auxiliar = NULL;
			this->size=0;
		}
		void Insertar(T value);
		void Imprimir();
		int GetSize();
		bool Empty();
		int Buscar(T objeto);
		void InsertAfter(Nodo<T> lugar, Nodo<T> nuevo);
		void DeleteLast();
		Nodo<T>* GetElementAt(int id);
		Nodo<T>* GetCabeza();
};
template <class T>
void ListaCicular<T>:: Insertar(T value){
            Nodo<T> *nuevo = new Nodo<T>(value);
			if(cabeza==NULL){
				this->cabeza = nuevo;
				this->cabeza->setNext(cabeza);
				size++;
			}else{
				this->auxiliar = this->cabeza;
				while(auxiliar->getNext() != cabeza){
					auxiliar = auxiliar->getNext();
				}
				auxiliar->setNext(nuevo);
				nuevo->setNext(cabeza);
				size++;
			}

		}
template <class T>
int ListaCicular<T>:: GetSize(){
	return size;
}
template <class T>
Nodo<T>* ListaCicular<T>:: GetCabeza(){
	return cabeza;
}
template <class T>
bool ListaCicular<T>:: Empty(){
	if(size==0){
		return true;
	}
	return false;
}

template <class T>
void ListaCicular<T>:: Imprimir(){
				Nodo<T> *aux;
				aux = this->cabeza;
				if(Empty()){
					 cout << "La lista está vacía" ;
				}else{
					for(int i =0; i<size;i++){
						cout << aux->getValue();
						aux = aux->getNext();
					}
					cout << "\n";
				}
		}
//ESTE ES EL TIPO DE CODIGO QUE SE USARIOA PARA SUSTITURI UN CARACER O PALABRA EN UN INDICE
template <class T>
void ListaCicular<T>:: InsertAfter(Nodo<T> lugar, Nodo<T> nuevo){
Nodo<T> *aux = this->cabeza;
	if(!Empty()){
		for(int i =0; i<size;i++){
			cout << i;
			if(aux->getValue() == lugar.getValue()){
				cout << "coincide";
				Nodo<T> *temp(aux->getNext());
				aux->setNext(&nuevo);
				nuevo.setPrevious(aux);
				nuevo.setNext(temp);
				temp->setPrevious(&nuevo);
				size++;
			}
			aux = aux->getNext();
		}
	}
}

template <class T>
int ListaCicular<T>:: Buscar(T objeto){
	Nodo<T> *aux = this->cabeza;
	for(int i = 0; i<size;i++){
		if(aux->getValue() == objeto){
			return i;
		}
		aux= aux->getNext();
	}
	return 0;
}

template <class T>
Nodo<T>* ListaCicular<T>:: GetElementAt(int id){
	Nodo<T> *aux = this->cabeza;
	if(id<=size){
		for(int i = 0; i<id;i++){
			aux= aux->getNext();
		}
		return aux;
	}
	return 0;
}

template <class T>
void ListaCicular<T>::DeleteLast(){
    Nodo<T> *aux = this->auxiliar;
    this->auxiliar= this->cabeza;
    while(auxiliar->getNext()!=cabeza){
        aux=auxiliar;
        auxiliar = auxiliar->getNext();
    }
    aux->setNext(cabeza);
    size--;
}
