#include <iostream>
#include <fstream>
#include "Nodo_t.h"
#include "ListaSimple_t.h"
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
		bool Insertar(string value);
		Nodo<jugadores>* GetRoot();
		void Imprimir();
		int GetSize();
		bool Empty();
        string PrintTree(Nodo<jugadores> *Central);
        string Preorder(Nodo<jugadores> *Central);
        void InOrder(Nodo<jugadores> *Central, ListaSimple<int> *lista);
        string PosOrder(Nodo<jugadores> *Central);
        string GetLabels(Nodo<jugadores> *Central);
};
bool ABB:: Insertar(string value){
            Nodo<jugadores> *nuevo = new Nodo<jugadores>(value);
            jugadores jugador(value);
            jugador.setID(size);
            nuevo->setValue(jugador);
            //cout<<"size is "<<size<<"\n";
            //cout<<"el ID del nuevo "<<nuevo->getValue().getName()<<" nodo es "<<nuevo->getValue().getID()<<"\n";
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
				            size++;
                            return true;
                        }else{
                            aux=aux->getNext();
                        }
                    }
                    if(aux->getValue().getName() > nuevo->getValue().getName()){
                        if(aux->getPrevious()==NULL){
                            aux->setPrevious(nuevo);
				            size++;
                            return true;
                        }else{
                            aux=aux->getPrevious();
                        }
                    }else if(aux->getValue().getName() == nuevo->getValue().getName()){
                        //valor repetido
                        return false;
                    }
                }
			}

		}

int ABB:: GetSize(){
	return this->size;
}
Nodo<jugadores>* ABB:: GetRoot(){
	return this->raiz;
}
int impresos;
bool ABB:: Empty(){
	if(size==0){
		return true;
	}
	return false;
    }

void ABB:: Imprimir(){
            string content = "digraph G {\n rankdir=TB nodesep=0.3;\n ranksep=0.2;\n    margin=0.1;\n   node [shape=circle];\n  edge [arrowsize=0.8];";
            string name ="ABB";
            content+=PrintTree(this->raiz);
            ofstream graphFile;
            graphFile.open(""+name+".txt");
            graphFile << content+"}";
            graphFile.close();
            std::string filePath="dot -Tpng "+name+".txt -o "+name+".png";
            system(filePath.c_str());
        //Preorder
            string ABB_Preorder="ABB_Pre";
            impresos=1;
            content=Preorder(this->raiz);
            content = "digraph G {\n rankdir=LR nodesep=0.3;\n ranksep=0.2;\n margin=0.1;\n   node [shape=box];\n  edge [arrowsize=0.8]\n" + content;
            graphFile.open(""+ABB_Preorder+".txt");
            graphFile << content+GetLabels(this->raiz)+"}";
            graphFile.close();
            filePath="dot -Tpng "+ABB_Preorder+".txt -o "+ABB_Preorder+".png";
            system(filePath.c_str());
        //InOrder
            string ABB_InOrder="ABB_In";
            impresos=1;
            ListaSimple<int> *ids = new ListaSimple<int>();
            InOrder(this->raiz, ids);
            content = "digraph G {\n rankdir=LR nodesep=0.3;\n ranksep=0.2;\n margin=0.1;\n   node [shape=box];\n  edge [arrowsize=0.8]\n";
            for(int i=0;i<ids->GetSize();i++){
                if(i==0){
                    content+=to_string(ids->ElementAt(i)->getValue());
                }else{
                    content+="->"+to_string(ids->ElementAt(i)->getValue());
                }
                }
            graphFile.open(""+ABB_InOrder+".txt");
            graphFile <<  content+";\n" +GetLabels(this->raiz)+"}";
            graphFile.close();
            filePath="dot -Tpng "+ABB_InOrder+".txt -o "+ABB_InOrder+".png";
            system(filePath.c_str());
        //Posorder
            string ABB_Posorder="ABB_Pos";
            impresos=1;
            content=PosOrder(this->raiz);
            content = "digraph G {\n rankdir=LR nodesep=0.3;\n ranksep=0.2;\n margin=0.1;\n   node [shape=box];\n  edge [arrowsize=0.8]\n" + content;
            graphFile.open(""+ABB_Posorder+".txt");
            graphFile << content+GetLabels(this->raiz)+"}";
            graphFile.close();
            filePath="dot -Tpng "+ABB_Posorder+".txt -o "+ABB_Posorder+".png";
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
string ABB::Preorder(Nodo<jugadores> *Central){
        string content = "";
        cout<<to_string(size)<<","<<to_string(impresos);
        if(impresos<size){
                content += to_string(Central->getValue().getID())+ " -> ";
        }else{
                content += to_string(Central->getValue().getID())+ ";\n";
        }
        if (Central->getPrevious() != NULL)//Left
        {
            impresos++;
            content += Preorder(Central->getPrevious()); 
        }    
            
        if (Central->getNext() != NULL)//Right
        {
            impresos++;
            content += Preorder(Central->getNext());
        }
        return content;
    }
void ABB::InOrder(Nodo<jugadores> *Central, ListaSimple<int> *lista){
        if (Central->getPrevious() != NULL)//Left
        {            
            InOrder(Central->getPrevious(), lista); 
        }
        lista->Insertar(Central->getValue().getID());   
        if (Central->getNext() != NULL)//Right
        {
            InOrder(Central->getNext(), lista);
        }
    }
string ABB::PosOrder(Nodo<jugadores> *Central){
        string content = "";
        if (Central->getPrevious() != NULL)//Left
        {
            impresos++;
            content += PosOrder(Central->getPrevious()); 
        }    
            
        if (Central->getNext() != NULL)//Right
        {
            impresos++;
            content += PosOrder(Central->getNext());
        }
        if(impresos<size){
                content += to_string(Central->getValue().getID())+ " -> ";
        }else{
                content += to_string(Central->getValue().getID())+ ";\n";
        }
        cout<<to_string(size)<<","<<to_string(impresos);
        return content;
    }
string ABB::GetLabels(Nodo<jugadores> *Central){
        string content = "";
        content += to_string(Central->getValue().getID()) + "[label=\"" + Central->getValue().getName() + "\"];\n";    
        if (Central->getPrevious() != NULL)//Left
        {        
        content += GetLabels(Central->getPrevious());
        }
        if (Central->getNext() != NULL)//Right
        {
            content += GetLabels(Central->getNext());
        }
        return content;
    }