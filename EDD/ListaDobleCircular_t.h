#include <iostream>
#include "Nodo_t.h"
#pragma once
using namespace std;

template <class T>
class ListaDobleCircular{
	private:
		Nodo<T> *cabeza;
		int size;
	public:
		ListaDobleCircular(){
			this->cabeza = NULL;
			this->size=0;
		}
		void Insertar(T value);
		void SetCabeza(Nodo<T>* nuevo);
		void Imprimir();
		int GetSize();
		Nodo<T>* GetCabeza();
		Nodo<T>* GetLast();
		bool Empty();
		int Buscar(T objeto);
		void InsertAfter(Nodo<T> lugar, Nodo<T> nuevo);
		void DeleteLast();
		void DeleteFirst();
		void InsertarInicio(T value);
};
template <class T>
void ListaDobleCircular<T>:: Insertar(T value){
            Nodo<T> *nuevo = new Nodo<T>(value);
			if(cabeza==NULL){
				this->cabeza = nuevo;
				this->cabeza->setNext(cabeza);
				this->cabeza->setPrevious(cabeza);
				size++;
			}else{
				Nodo<T> *aux= this->cabeza;
				while(aux->getNext() != cabeza){
					aux = aux->getNext();
				}
				aux->setNext(nuevo);
				nuevo->setPrevious(aux);
				nuevo->setNext(cabeza);
                cabeza->setPrevious(nuevo);
				size++;
			}

		}
template <class T>
void ListaDobleCircular<T>:: InsertarInicio(T value){
            Nodo<T> *nuevo = new Nodo<T>(value);
			if(!Empty()){
				nuevo->setNext(cabeza);
                cabeza->getPrevious()->setNext(nuevo);
                nuevo->setPrevious(cabeza->getPrevious());
				cabeza->setPrevious(nuevo);
				cabeza=nuevo;
			}
		}

template <class T>
void ListaDobleCircular<T>:: DeleteFirst(){
			if(!Empty()){
                cabeza->getPrevious()->setNext(cabeza->getNext());
                cabeza->getNext()->setPrevious(cabeza->getPrevious());
				cabeza=cabeza->getNext();
			}
		}

template <class T>
int ListaDobleCircular<T>:: GetSize(){
	return size;
}
template <class T>
Nodo<T>* ListaDobleCircular<T>:: GetCabeza(){
	return cabeza;
}
template <class T>
bool ListaDobleCircular<T>:: Empty(){
	if(size==0){
		return true;
	}
	return false;
}

template <class T>
void ListaDobleCircular<T>:: Imprimir(){
				Nodo<T> *aux;
				aux = this->cabeza;
				if(Empty()){
					 cout << "La lista está vacía" ;
				}else{
					while(aux->getNext()!=cabeza){
						cout << aux->getValue();
						aux = aux->getNext();
					}
                    cout << aux->getValue();
					cout << "\n";
				}
		}

template <class T>
void ListaDobleCircular<T>:: InsertAfter(Nodo<T> lugar, Nodo<T> nuevo){
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
int ListaDobleCircular<T>:: Buscar(T objeto){
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
void ListaDobleCircular<T>::DeleteLast(){
    if(!Empty()){
        if(cabeza->getNext()==cabeza){
            cabeza=NULL;
        }else{
            /*
        Nodo<T> *aux = this->cabeza;
        while(aux->getNext()!=cabeza){
            aux = aux->getNext();
        }
        Nodo<T> *temp = aux->getPrevious();
        temp->setNext(NULL);
        delete aux;
        */
       cabeza->getPrevious()->getPrevious()->setNext(cabeza);
       cabeza->setPrevious(cabeza->getPrevious()->getPrevious());
        }
        size--;
    }
}

template <class T>
void ListaDobleCircular<T>::SetCabeza(Nodo<T> *nuevo){
    Nodo<T> *aux = cabeza->getNext();
    nuevo->setNext(aux);
    cabeza =nuevo;
}

template <class T>
Nodo<T>* ListaDobleCircular<T>::GetLast(){
    if(!Empty()){
        return cabeza->getPrevious();
    }else{
        return NULL;
    }
}




