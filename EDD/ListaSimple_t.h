#include <iostream>
using namespace std;
#include "Nodo_t.h"
#pragma once
template <class T>
class ListaSimple{
	Nodo<T> *cabeza;
	int size;
	public:
		ListaSimple(){
			this->cabeza = NULL;
			this->size=0;
		}
		void Insertar(T value);
		void Imprimir();
		int GetSize();
		Nodo<T>* GetCabeza();
		bool Empty();
		void DeleteLast();
		Nodo<T>* SacarElemento(int posiscion);
		Nodo<T>* ElementAt(int posiscion);
		void Vaciar();
		bool InsertAt(T value, int place);
		bool InsertBefore(T value, int place);
};
template <class T>
void ListaSimple<T>:: Insertar(T value){
			Nodo<T> *nuevo = new Nodo<T>(value);
			if(cabeza==NULL){
				this->cabeza = nuevo;
				size++;
			}else{
				Nodo<T> *aux = this->cabeza;
				while(aux->getNext()!=NULL){
					aux = aux->getNext();
				}
				aux->setNext(nuevo);
				nuevo->setNext(NULL);
				size++;
			}

		}
template <class T>
int ListaSimple<T>:: GetSize(){
	return size;
}
template <class T>
bool ListaSimple<T>:: Empty(){
	if(size==0){
		return true;
	}
	return false;
}
template <class T>
void ListaSimple<T>:: Vaciar(){
	this->cabeza=NULL;
}

template <class T>
Nodo<T>* ListaSimple<T>:: GetCabeza(){
	return cabeza;
}
template <class T>
void ListaSimple<T>:: Imprimir(){
				Nodo<T> *aux = this->cabeza;
				if(Empty()){
					 cout << "La lista está vacía \n" ;
				}else{
					for(int i = 0; i<size;i++){
						cout << aux->getValue();
						aux = aux->getNext();
					}
					cout << "\n";
				}
		}
template <class T>
void ListaSimple<T>::DeleteLast(){
    if(!Empty()){
        if(cabeza->getNext()==NULL){
            cabeza=NULL;
        }else{
        Nodo<T> *aux = this->cabeza;
        Nodo<T> *last = aux;
        while(aux->getNext()!=NULL){
            last = aux;
            aux = aux->getNext();
        }
        last->setNext(NULL);
        delete aux;
        }
        size--;
    }
}

template <class T>
Nodo<T>* ListaSimple<T>::SacarElemento(int posicion){
    if(!Empty()){
		if(posicion<=size){
			Nodo<T> *aux = this->cabeza;
			Nodo<T> *temp = aux;
				for(int i =0;i<posicion;i++){
					temp=aux;
					aux=aux->getNext();
				}
				size--;	
				if(aux->getNext()!=NULL){
					temp->setNext(aux->getNext());
				}else{
					temp->setNext(NULL);
				}
				return aux;	
			}
		}
	return NULL;
}
template <class T>
Nodo<T>* ListaSimple<T>::ElementAt(int posicion){
    if(!Empty()){
		if(posicion<=size){
			Nodo<T> *aux = this->cabeza;
				for(int i =0;i<posicion;i++){
					aux=aux->getNext();
				}
				return aux;	
			}
			return NULL;
		}
	return NULL;
}

template <class T>
bool ListaSimple<T>::InsertAt(T value, int place){
    Nodo<T> *nuevo = new Nodo<T>(value); 
    if(place<=size){
        Nodo<T> *auxiliar=this->cabeza;
        for(int i =0;i<place;i++){
            auxiliar=auxiliar->getNext();
        }
        nuevo->setNext(auxiliar->getNext());
        auxiliar->setNext(nuevo);
		size++;   
		return true;
    }else{
		return false;
	} 
}

template <class T>
bool ListaSimple<T>::InsertBefore(T value, int place){
    Nodo<T> *nuevo = new Nodo<T>(value); 
    if(place<=size){
        Nodo<T> *auxiliar=this->cabeza;
        Nodo<T> *temp=auxiliar;
        for(int i =0;i<place;i++){
			temp=auxiliar;
			if(auxiliar->getNext()!=NULL){
            	auxiliar=auxiliar->getNext();
			}
        }
		if(auxiliar==this->cabeza){
			nuevo->setNext(this->cabeza);
			this->cabeza=nuevo;
		}else if(auxiliar->getNext()==NULL){
			nuevo->setNext(auxiliar);
			temp->setNext(nuevo);
		}else{
			temp->setNext(nuevo);
			nuevo->setNext(auxiliar);
		}
		size++;   
		return true;
    }else{
		return false;
	} 
}