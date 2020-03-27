#include <iostream>
#include "Nodo_t.h"
#pragma once
using namespace std;

template <class T>
class MatrizDispersa{
    private:
        Nodo<T> *raiz;
        Nodo<T> *auxiliar;
        T NoValue;

    public:
        MatrizDispersa(T NoValue)
        {
            this->raiz = new Nodo<T>(NoValue, -1, -1);
            this->auxiliar = NULL;
            this->NoValue = NoValue;
        }
        Nodo<T> *getRoot();
        Nodo<T> *BuscarEnY(int y);
        Nodo<T> *BuscarEnX(int x);
        bool Insertar(T Value, int x, int y);
        Nodo<T> *InsertarOrdenadoColumna(Nodo<T> *nuevo, Nodo<T> *cabezaColumna);
        Nodo<T> *InsertarOrdenadoFila(Nodo<T> *nuevo, Nodo<T> *cabezaFila);
        Nodo<T> *CrearColumna(int x);
        Nodo<T> *CrearFila(int y);
        void ImprimirMatriz();
        bool FilaOcupada(Nodo<T> *nuevo, Nodo<T> *cabezaFila);
        bool ColumnaOcupada(Nodo<T> *nuevo, Nodo<T> *cabezaColumna);
        Nodo<T>* FilaAt(Nodo<T> *nuevo, Nodo<T> *cabezaFila);
        Nodo<T>* ColumnaAt(Nodo<T> *nuevo, Nodo<T> *cabezaColumna);
        bool NodeAt(int x, int y);
        bool SameFilaNode(Nodo<T> *nuevo, Nodo<T> *cabezaFila);
        bool SameColumnNode(Nodo<T> *nuevo, Nodo<T> *cabezaColumna);
        bool SameNode(T value, int x, int y);
        bool CasillaOcupada(T Value, int x, int y);
    };
template <class T>
Nodo<T> *MatrizDispersa<T>::getRoot(){
    return this->raiz;
    }
template <class T>
Nodo<T> *MatrizDispersa<T>::BuscarEnY(int y){
    this->auxiliar = this->raiz;
    while (auxiliar != NULL)
    {
        if (auxiliar->getY() == y)
        {
            cout<<"Sí existe la Fila"<<to_string(y)<<"\n";
            return auxiliar;
        }
        auxiliar = auxiliar->getDown();
    }
    cout<<"No existe la Fila"<<to_string(y)<<"\n";
    return NULL;
    }
template <class T>
Nodo<T> *MatrizDispersa<T>::BuscarEnX(int x){
    this->auxiliar = this->raiz;
    while (auxiliar != NULL)
    {
        if (auxiliar->getX() == x)
        {
            cout<<"Sí existe la columna"<<to_string(x)<<"\n";
            return auxiliar;
        }
        auxiliar = auxiliar->getNext();
    }
    cout<<"No existe la columna"<<to_string(x)<<"\n";
    return NULL;
    }
template <class T>
Nodo<T> *MatrizDispersa<T>::InsertarOrdenadoColumna(Nodo<T> *nuevo, Nodo<T> *cabezaColumna){
    Nodo<T> *aux = cabezaColumna;
    bool bandera = false;
    while (true)
    {
        if (aux->getX() == nuevo->getX())
        {
            //Se sobreescribe si es la misma posición
            aux->setY(nuevo->getY());
            aux->setValue(nuevo->getValue());
            return aux;
        }
        else if (aux->getX() > nuevo->getX())
        {
            //Se intserta antes que aux
            bandera = true;
            break;
        }
        if (aux->getNext() != NULL)
        {
            aux = aux->getNext();
        }
        else
        {
            //Insertar nuevo despues de aux
            break;
        }
    }
    if (bandera)
    {
        //inserta antes de aux
        nuevo->setNext(aux);
        aux->getPrevious()->setNext(nuevo);
        nuevo->setPrevious(aux->getPrevious());
        aux->setPrevious(nuevo);
        cout<<"nueva X"<<to_string(nuevo->getX())<<"\n";
    }
    else
    {
        cout<<"nueva X"<<to_string(nuevo->getX())<<"\n";
        aux->setNext(nuevo);
        nuevo->setPrevious(aux);
    }
    return nuevo;
    }
template <class T>
Nodo<T> *MatrizDispersa<T>::InsertarOrdenadoFila(Nodo<T> *nuevo, Nodo<T> *cabezaFila){
    Nodo<T> *aux = cabezaFila;
    bool bandera = false;
    while (true)
    {
        if (aux->getY() == nuevo->getY())
        {
            aux->setX(nuevo->getX());
            aux->setValue(nuevo->getValue());
            return aux;
        }
        else if (aux->getY() > nuevo->getY())
        {
            bandera = true;
            break;
        }
        if (aux->getDown() != NULL)
        {
            aux = aux->getDown();
        }
        else
        {
            break;
        }
    }
    if (bandera)
    {
        nuevo->setDown(aux);
        aux->getUp()->setDown(nuevo);
        nuevo->setUp(aux->getUp());
        aux->setUp(nuevo);
        cout<<"nueva Y"<<to_string(nuevo->getY())<<"\n";
    }
    else
    {
        cout<<"nueva Y"<<to_string(nuevo->getY())<<"\n";
        aux->setDown(nuevo);
        nuevo->setUp(aux);
    }
    return nuevo;
    }
template <class T>
Nodo<T>* MatrizDispersa<T>::CrearColumna(int x){
    Nodo<T> *nuevo = new Nodo<T>(this->NoValue, x, -1);
    Nodo<T> *Columna = InsertarOrdenadoColumna(nuevo, this->raiz);
    cout<<Columna->getX()<<","<<Columna->getY()<<"\n";
    return Columna;
    }
template <class T>
Nodo<T>* MatrizDispersa<T>::CrearFila(int y){
    Nodo<T> *nuevo = new Nodo<T>(this->NoValue, -1, y);
    Nodo<T> *Fila = InsertarOrdenadoFila(nuevo, this->raiz);
    cout<<Fila->getX()<<","<<Fila->getY()<<"\n";
    return Fila;
    }
template <class T>
bool MatrizDispersa<T>::Insertar(T Value, int x, int y){
    Nodo<T> *nuevo = new Nodo<T>(Value, x, y);
    Nodo<T> *NodoColumna, *NodoFila;
    NodoFila = BuscarEnY(y);
    NodoColumna = BuscarEnX(x);
    //Columna y Fila no existen
    if (NodoColumna == NULL && NodoFila == NULL)
    {
        //Creamos Columna
        NodoColumna = CrearColumna(x);
        //Creamos Fila
        NodoFila = CrearFila(y);
        //Insertamos en la Columna
        nuevo = InsertarOrdenadoColumna(nuevo, NodoFila);
        //Insertamos en la Fila
        nuevo = InsertarOrdenadoFila(nuevo, NodoColumna);
        return true;
    }
    //Columna existe y Fila no existe
    else if (NodoColumna != NULL && NodoFila == NULL)
    {
        //Creamos Fila
        NodoFila = CrearFila(y);
        //Insertamos en la Columna
        nuevo = InsertarOrdenadoColumna(nuevo, NodoFila);
        //Insertamos en la Fila
        nuevo = InsertarOrdenadoFila(nuevo, NodoColumna);
        return true;
    }
    //Columna no existe y Fila existe
    else if (NodoColumna == NULL && NodoFila != NULL)
    {
        //Creamos Columna
        NodoColumna = CrearColumna(x);
        //Insertamos en la Columna
        nuevo = InsertarOrdenadoColumna(nuevo, NodoFila);
        //Insertamos en la Fila
        nuevo = InsertarOrdenadoFila(nuevo, NodoColumna);
        return true;
    }
    //Columna y Fila existen
    else if (NodoColumna != NULL && NodoFila != NULL)
    {
        if(ColumnaOcupada(nuevo, NodoFila) && FilaOcupada(nuevo, NodoColumna)){
            return false;
        }
        //Insertamos en la Columna
        nuevo = InsertarOrdenadoColumna(nuevo, NodoFila);
        //Insertamos en la Fila
        nuevo = InsertarOrdenadoFila(nuevo, NodoColumna);
        return true;
    }
    return true;
    }

template <class T>
void MatrizDispersa<T>::ImprimirMatriz(){
    this->auxiliar = this->raiz;
    while (auxiliar != NULL)
    {
        Nodo<T> *temp = auxiliar;
        while (temp != NULL)
        {
            if (temp->getX() == -1 && temp->getY() == -1)
            {
                cout << "\nRaiz ";
            }
            else if (temp->getY() == -1)
            {
                cout << "Y_"+to_string(temp->getX()) << "  ";
            }
            else if (temp->getX() == -1)
            {
                cout << "X_"+to_string(temp->getY());
            }
            else
            {
                cout << temp->getValue() << " ";
            }
            temp = temp->getNext();
        }        
        cout << "\n";
        auxiliar = auxiliar->getDown();
    }
    }

template <class T>
bool MatrizDispersa<T>::ColumnaOcupada(Nodo<T> *nuevo, Nodo<T> *cabezaColumna){
    Nodo<T> *aux = cabezaColumna;
    bool bandera = false;
    while (true)
    {
        if (aux->getX() == nuevo->getX())
        {
            //Se sobreescribe si es la misma posición
            return true;
        }
        else if (aux->getX() > nuevo->getX())
        {
            //Se intserta antes que aux
            bandera = true;
            break;
        }
        if (aux->getNext() != NULL)
        {
            aux = aux->getNext();
        }
        else
        {
            //Insertar nuevo despues de aux
            break;
        }
    }
    return false;
    }
template <class T>
bool MatrizDispersa<T>::FilaOcupada(Nodo<T> *nuevo, Nodo<T> *cabezaFila){
        Nodo<T> *aux = cabezaFila;
        bool bandera = false;
        while (true)
        {
            if (aux->getY() == nuevo->getY())
            {
                return true;
            }
            else if (aux->getY() > nuevo->getY())
            {
                bandera = true;
                break;
            }
            if (aux->getDown() != NULL)
            {
                aux = aux->getDown();
            }
            else
            {
                break;
            }
        }
        return false;
    }
template<class T>
bool MatrizDispersa<T>::SameNode(T value, int x, int y){
    Nodo<T> *nuevo= new Nodo<T>(value, x, y);
    Nodo<T> *NodoColumna, *NodoFila;
    NodoFila = BuscarEnY(y);
    NodoColumna = BuscarEnX(x);
    if (NodoColumna != NULL && NodoFila != NULL)
    {
        if(ColumnaOcupada(nuevo, NodoFila) && FilaOcupada(nuevo, NodoColumna)){
            if(SameColumnNode(nuevo, NodoFila) && SameFilaNode(nuevo, NodoColumna)){
                return true;
            }
        }
    }
    return false;
    }
template <class T>
bool MatrizDispersa<T>::SameColumnNode(Nodo<T> *nuevo, Nodo<T> *cabezaColumna){
    Nodo<T> *aux = cabezaColumna;
    bool bandera = false;
    while (true)
    {
        if (aux->getX() == nuevo->getX())
        {/*
            if(aux->getValue()==nuevo->getValue()){
                    return true;
                }*/
        }
        else if (aux->getX() > nuevo->getX())
        {
            //Se intserta antes que aux
            bandera = true;
            break;
        }
        if (aux->getNext() != NULL)
        {
            aux = aux->getNext();
        }
        else
        {
            //Insertar nuevo despues de aux
            break;
        }
    }
    return false;
    }
template <class T>
bool MatrizDispersa<T>::SameFilaNode(Nodo<T> *nuevo, Nodo<T> *cabezaFila){
        Nodo<T> *aux = cabezaFila;
        bool bandera = false;
        while (true)
        {
            if (aux->getY() == nuevo->getY())
            {/*
                if(aux->getValue()==nuevo->getValue()){
                    return true;
                }*/
            }
            else if (aux->getY() > nuevo->getY())
            {
                bandera = true;
                break;
            }
            if (aux->getDown() != NULL)
            {
                aux = aux->getDown();
            }
            else
            {
                break;
            }
        }
        return false;
    }
template <class T>
Nodo<T>* MatrizDispersa<T>::ColumnaAt(Nodo<T> *nuevo, Nodo<T> *cabezaColumna){
    Nodo<T> *aux = cabezaColumna;
    while (true)
    {
        if (aux->getX() == nuevo->getX())
        {
            //Se sobreescribe si es la misma posición
            nuevo->setY(aux->getY());
            nuevo->setValue(aux->getValue());
            return nuevo;
        }
        else if (aux->getX() > nuevo->getX())
        {
            //Se intserta antes que aux
            break;
        }
        if (aux->getNext() != NULL)
        {
            aux = aux->getNext();
        }
        else
        {
            //Insertar nuevo despues de aux
            break;
        }
    }
    return nuevo;
    }
template <class T>
Nodo<T>* MatrizDispersa<T>::FilaAt(Nodo<T> *nuevo, Nodo<T> *cabezaFila){
        Nodo<T> *aux = cabezaFila;
        while (true)
        {
            if (aux->getY() == nuevo->getY())
            {
                nuevo->setX(aux->getX());
                if(nuevo->getValue()==aux->getValue()){
                    cout<<"Si es la misma ficha"<<endl;
                }
                return nuevo;
            }
            else if (aux->getY() > nuevo->getY())
            {
                break;
            }
            if (aux->getDown() != NULL)
            {
                aux = aux->getDown();
            }
            else
            {
                break;
            }
        }
        return nuevo;
    }
template<class T>
bool MatrizDispersa<T>::CasillaOcupada(T Value, int x, int y){
    Nodo<T> *nuevo = new Nodo<T>(Value, x, y);
    Nodo<T> *NodoColumna, *NodoFila;
    NodoFila = BuscarEnY(y);
    NodoColumna = BuscarEnX(x);
    if (NodoColumna != NULL && NodoFila != NULL)
    {
        if(ColumnaOcupada(nuevo, NodoFila) && FilaOcupada(nuevo, NodoColumna)){
            return true;
        }
    }
    return false;
    }