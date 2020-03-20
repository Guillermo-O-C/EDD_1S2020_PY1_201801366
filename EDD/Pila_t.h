#include <iostream>
#include "Nodo_t.h"
#pragma once
using namespace std;

template <class T>
class Pila{
	private:
		Nodo<T> *cabeza;
		Nodo<T> *auxiliar;
		int size;
	public:
		Pila(){
			this->cabeza = NULL;
			this->auxiliar = NULL;
			this->size=0;
		}
		void Push(T value);
		Nodo<T>* Pop();
		void SuperPop();
		void Imprimir();        //Pendiente
		int GetSize();
		bool Empty();
		int Buscar(T objeto);   //Pendiente
		Nodo<T>* ReturnTop();
};
template <class T>
void Pila<T>:: Push(T value){
            Nodo<T> *nuevo = new Nodo<T>(value);
			if(cabeza==NULL){
				this->cabeza = nuevo;
				this->cabeza->setNext(NULL);
				size++;
			}else{
				this->auxiliar = this->cabeza;
				this->cabeza = nuevo;
				cabeza->setNext(auxiliar);
				size++;
			}

		}
template <class T>
int Pila<T>:: GetSize(){
	return size;
}
template <class T>
Nodo<T>* Pila<T>:: ReturnTop(){
	return cabeza;
}
template <class T>
bool Pila<T>:: Empty(){
	if(size==0){
		return true;
	}
	return false;
}

template <class T>
void Pila<T>:: Imprimir(){
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

template <class T>
int Pila<T>:: Buscar(T objeto){
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
Nodo<T>* Pila<T>::Pop(){
    this->auxiliar = this->cabeza;
    this->cabeza= this->cabeza->getNext();
    size--;
    return auxiliar;
}

template <class T>
void Pila<T>::SuperPop(){
    while(!Empty()){
        Pop();
    }
    size=0;
}
