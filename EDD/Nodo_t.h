#include <iostream>
using namespace std;
#pragma once

template<class T>
class Nodo{
	private:
		T value;
		Nodo *next;
		Nodo *previous;
		Nodo *up;
		Nodo *down;
		int x;
		int y;
	public:
		Nodo(T value){
			this->value = value;
			this->next = NULL;
			this->previous = NULL;
		}
		Nodo(T value, int x , int y){
			this->value = value;
			this->next = NULL;
			this->previous = NULL;
			this->up = NULL;
			this->down = NULL;
			this->x = x;
			this->y = y;
		}
	void setValue(T value);
	T getValue();
	void setNext(Nodo *next);
	void setPrevious(Nodo *previous);
	Nodo* getNext();
	Nodo* getPrevious();	
	void setUp(Nodo *up);
	void setDown(Nodo *down);
	Nodo* getUp();
	Nodo* getDown();
	void setX(int x);
	void setY(int Y);
	int getX();
	int getY();		
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
template <class T>
	 void Nodo<T>:: setUp(Nodo *up){
				this->up = up;
			}
template <class T>
	 void Nodo<T>:: setDown(Nodo *down){
				this->down = down;
			}
template <class T>
	Nodo<T>* Nodo<T>:: getUp(){
				return this->up;
			}
template <class T>
	Nodo<T>* Nodo<T>:: getDown(){
				return this->down;
			}
template <class T>
	 void Nodo<T>:: setX(int x){
				this->x = x;
			}
template <class T>
	 void Nodo<T>:: setY(int y){
				this->y = y;
			}
template <class T>
	int Nodo<T>:: getX(){
				return this->x;
			}
template <class T>
	int Nodo<T>:: getY(){
				return this->y;
			}
