#include <iostream>
#include <fstream>
#include "Nodo_t.h"
#include "../jugadores.h"
#pragma once
using namespace std;

//Node Pointers Interpretacion
//Next -> Right
//Previous -> Left
class ABB{
	private:
		Nodo<jugadores> *raiz;
        Nodo<jugadores> *aux;
		int size = 0;
	public:
		ABB(){
			this->raiz = NULL;
			this->aux = NULL;
		}
		void Insertar(string value);
		Nodo<jugadores>* GetRoot();
		void Imprimir();
		int GetSize();
		bool Empty();
        string PrintTree(Nodo<jugadores> *Central);
		//int Buscar(T objeto);
};
void ABB:: Insertar(string value){
            Nodo<jugadores> *nuevo = new Nodo<jugadores>(value);
            jugadores jugador(value);
            jugador.setID(size);
            nuevo->setValue(jugador);
            cout<<"size is "<<size<<"\n";
            cout<<"el ID del nuevo "<<nuevo->getValue().getName()<<" nodo es "<<nuevo->getValue().getID()<<"\n";
			if(raiz==NULL){
				this->raiz = nuevo;
				this->raiz->setNext(NULL);
				this->raiz->setPrevious(NULL);
                size++;
			}else{
                this->aux=this->raiz;
                while(true){
                    if(aux->getValue().getName() < nuevo->getValue().getName()){
                        if(aux->getNext()==NULL){
                            aux->setNext(nuevo);
                            break;
                        }else{
                            aux=aux->getNext();
                        }
                    }
                    if(aux->getValue().getName() > nuevo->getValue().getName()){
                        if(aux->getPrevious()==NULL){
                            aux->setPrevious(nuevo);
                            break;
                        }else{
                            aux=aux->getPrevious();
                        }
                    }else if(aux->getValue().getName() == nuevo->getValue().getName()){
                        //valor repetido
                        size--;
                        break;
                    }
                }
				size++;
			}

		}

int ABB:: GetSize(){
	return this->size;
}
Nodo<jugadores>* ABB:: GetRoot(){
	return this->raiz;
}

bool ABB:: Empty(){
	if(size==0){
		return true;
	}
	return false;
}

void ABB:: Imprimir(){
            string content = "digraph G {\n rankdir=TB nodesep=0.3;\n ranksep=0.2;\n    margin=0.1;\n   node [shape=circle];\n  edge [arrowsize=0.8];";
            string name = "";
            name="ABB";
            content+=PrintTree(this->raiz);
            ofstream graphFile;
            graphFile.open(""+name+".txt");
            graphFile << content+"}";
            graphFile.close();
            std::string filePath="dot -Tpng "+name+".txt -o "+name+".png";
            system(filePath.c_str());
        
		}

string ABB::PrintTree(Nodo<jugadores> *Central){
    string content = "";
    content += to_string(Central->getValue().getID()) + "[label=\"" + Central->getValue().getName() + "\"];\n";    
    if (Central->getPrevious() != NULL)//Left
    {        
        content += to_string(Central->getValue().getID()) + " -> " + to_string(Central->getPrevious()->getValue().getID()) +";\n";
        content += PrintTree(Central->getPrevious());
    }
    if (Central->getNext() != NULL)//Right
    {
        content += to_string(Central->getValue().getID()) + " -> " + to_string(Central->getNext()->getValue().getID()) +";\n";
        content += PrintTree(Central->getNext());
    }
    return content;
}
