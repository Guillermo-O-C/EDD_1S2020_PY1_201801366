#include <iostream>
#include "Nodo_t.h"
#pragma once
using namespace std;

template <class T>
class Cola{
	private:
		Nodo<T> *cabeza;
		Nodo<T> *auxiliar;
		int size;
	public:
		Cola(){
			this->cabeza = NULL;
			this->auxiliar = NULL;
			this->size=0;
		}
		void Encolar(T value);
		Nodo<T>* Desencolar();
		void Imprimir();        //Pendiente
		int GetSize();
		bool Empty();
		int Buscar(T objeto);   //Pendiente
		Nodo<T>* ReturnTop();
        void Vaciar();
        void InsertAt(T value, int place);
};
template <class T>
void Cola<T>:: Encolar(T value){
            Nodo<T> *nuevo = new Nodo<T>(value);
			if(Empty()){
				this->cabeza = nuevo;
				this->cabeza->setNext(NULL);
				size++;
			}else{
                this->auxiliar=this->cabeza;
				while(auxiliar->getNext()!=NULL){
                    auxiliar = auxiliar->getNext();
                }
                this->auxiliar->setNext(nuevo);
                nuevo->setNext(NULL);
				size++;
			}
		}

template <class T>
Nodo<T>* Cola<T>::Desencolar(){
    this->auxiliar = this->cabeza;
    this->cabeza= this->cabeza->getNext();
    size--;
    return auxiliar;
}
template <class T>
void Cola<T>::InsertAt(T value, int place){
    Nodo<T> *nuevo = new Nodo<T>(value); 
    if(place<=size){
        this->auxiliar=this->cabeza;
        for(int i =0;i<place;i++){
            auxiliar=auxiliar->getNext();
        }
        nuevo->setNext(auxiliar->getNext());
        auxiliar->setNext(nuevo);   
    }else{
        this->auxiliar=this->cabeza;
        while(auxiliar->getNext()!=NULL){
            auxiliar=auxiliar->getNext();
        }
        auxiliar->setNext(nuevo);
        nuevo->setNext(NULL);
    }    
}
template <class T>
int Cola<T>:: GetSize(){
	return size;
}
template <class T>
Nodo<T>* Cola<T>:: ReturnTop(){
	return cabeza;
}
template <class T>
bool Cola<T>:: Empty(){
	if(size==0){
		return true;
	}
	return false;
}

template <class T>
void Cola<T>:: Imprimir(){
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
int Cola<T>:: Buscar(T objeto){
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
void Cola<T>::Vaciar(){
    while(!Empty()){
        Desencolar();
    }
    size=0;
}
