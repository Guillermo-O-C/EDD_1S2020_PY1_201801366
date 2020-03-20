#include <iostream>
#include "Nodo_t.h"
#pragma once
using namespace std;

template <class T>
class ListaDoble{
	private:
		Nodo<T> *cabeza;
		int size;
	public:
		ListaDoble(){
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
void ListaDoble<T>:: Insertar(T value){
            Nodo<T> *nuevo = new Nodo<T>(value);
			if(cabeza==NULL){
				this->cabeza = nuevo;
				this->cabeza->setNext(NULL);
				this->cabeza->setPrevious(NULL);
				size++;
			}else{
				Nodo<T> *aux= this->cabeza;
				while(aux->getNext() != NULL){
					aux = aux->getNext();
				}
				aux->setNext(nuevo);
				nuevo->setPrevious(aux);
				nuevo->setNext(NULL);
				size++;
			}

		}
template <class T>
void ListaDoble<T>:: InsertarInicio(T value){
            Nodo<T> *nuevo = new Nodo<T>(value);
			if(!Empty()){
				nuevo->setNext(cabeza);
				cabeza->setPrevious(nuevo);
				cabeza=nuevo;
				cabeza->setPrevious(NULL);
			}
		}

template <class T>
void ListaDoble<T>:: DeleteFirst(){
			if(!Empty()){
				cabeza=cabeza->getNext();
				cabeza->setPrevious(NULL);
			}
		}

template <class T>
int ListaDoble<T>:: GetSize(){
	return size;
}
template <class T>
Nodo<T>* ListaDoble<T>:: GetCabeza(){
	return cabeza;
}
template <class T>
bool ListaDoble<T>:: Empty(){
	if(size==0){
		return true;
	}
	return false;
}

template <class T>
void ListaDoble<T>:: Imprimir(){
				Nodo<T> *aux;
				aux = this->cabeza;
				if(Empty()){
					 cout << "La lista está vacía" ;
				}else{
					while(aux->getNext()!=NULL){
						cout << aux->getValue();
						aux = aux->getNext();
					}
                    cout << aux->getValue();
					cout << "\n";
				}
		}

template <class T>
void ListaDoble<T>:: InsertAfter(Nodo<T> lugar, Nodo<T> nuevo){
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
int ListaDoble<T>:: Buscar(T objeto){
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
void ListaDoble<T>::DeleteLast(){
    if(!Empty()){
        if(cabeza->getNext()==NULL){
            cabeza=NULL;
        }else{
        Nodo<T> *aux = this->cabeza;
        while(aux->getNext()!=NULL){
            aux = aux->getNext();
        }
        Nodo<T> *temp = aux->getPrevious();
        temp->setNext(NULL);
        delete aux;
        }
        size--;
    }
}

template <class T>
void ListaDoble<T>::SetCabeza(Nodo<T> *nuevo){
    Nodo<T> *aux = cabeza->getNext();
    nuevo->setNext(aux);
    cabeza =nuevo;
}

template <class T>
Nodo<T>* ListaDoble<T>::GetLast(){
    if(!Empty()){
        if(cabeza->getNext()==NULL){
            cabeza=NULL;
        }else{
            Nodo<T> *aux = this->cabeza;
            while(aux->getNext()!=NULL){
                aux = aux->getNext();
            }
            return aux;
        }
    }
}




