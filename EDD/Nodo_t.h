#include <iostream>
using namespace std;
#pragma once

template<class T>
class Nodo{
	private:
		T value;
		Nodo *next;
		Nodo *previous;
	public:
		Nodo(T value){
			this->value = value;
			this->next = NULL;
			this->previous = NULL;
		}
	void setValue(T value);
	T getValue();
	void setNext(Nodo *next);
	void setPrevious(Nodo *previous);
	Nodo* getNext();
	Nodo* getPrevious();		
};

template <class T>
	void Nodo<T>::setValue(T value){
				this->value = value;
			}
template <class T>
	T Nodo<T>:: getValue(){
				return this->value;
			}
template <class T>
	 void Nodo<T>:: setNext(Nodo *next){
				this->next = next;
			}
template <class T>
	 void Nodo<T>:: setPrevious(Nodo *previous){
				this->previous = previous;
			}
template <class T>
	Nodo<T>* Nodo<T>:: getNext(){
				return this->next;
			}
template <class T>
	Nodo<T>* Nodo<T>:: getPrevious(){
				return this->previous;
			}

