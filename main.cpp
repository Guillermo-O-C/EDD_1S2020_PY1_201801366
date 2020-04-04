//System Needs
#include <string>
	#include <iostream>
	#include <fstream>
	#include <typeinfo>
	#include <random>
	#include "json.hpp"
	using json = nlohmann::json;
//Estructuras
	#include "EDD/ListaCircular_t.h"
	#include "EDD/ListaDoble_t.h"
	#include "EDD/ListaSimple_t.h"
	#include "EDD/Pila_t.h"
	#include "EDD/MatrizDispersa_t.h"
	#include "EDD/ABB_t.h"
	#include "EDD/ListaDobleCircular_t.h"
	#include "EDD/Cola_t.h"
//Clases Scrabble++
	#include "jugadores.h"
	#include "Casillas.h"
	#include "Ficha.h"
//Estrucutras y Variables Globales
	ABB *arbol =  new ABB();
	ListaSimple<Casillas> *casillasEspeciales = new ListaSimple<Casillas>();
	ListaSimple<jugadores> *JugadoresOrdenados;
	ListaDobleCircular<string> *diccionario = new ListaDobleCircular<string>;
	Cola<Ficha> *Bolsa = new Cola<Ficha>();
	MatrizDispersa<Ficha> *Tablero = new MatrizDispersa<Ficha>(Ficha("-", 0));	
	int DimensionTablero, punteoJugador1, punteoJugador2;
	jugadores jugador1, jugador2;
	bool ganador, Insertar, UpdateCasilla, Vacio, Iniciado, ArchivoEntrada;
//Métodos de acceso a EDD
void PrintScoreBoard(ListaSimple<jugadores> *lista){
	std::string graph ("digraph ReporteLD { graph [dpi=300]\n rankdir =\"LR\"; \n size=\"5\" \n node [shape = box]; \n");
	string labels;
	Nodo<jugadores> *aux = lista->GetCabeza();
    for(int i =0; i<lista->GetSize(); i++){
		if(aux->getValue().getScores()->GetSize()>0){
			labels +=to_string(aux->getValue().getID())+"[label=\""+aux->getValue().getName()+", "+to_string(aux->getValue().getScores()->GetCabeza()->getValue())+"\"];\n";
        }else{
				labels +=to_string(aux->getValue().getID())+"[label=\""+aux->getValue().getName()+", NP\"];\n";
		}
		if(i==0){
			if(lista->GetSize()==1){
           	 	graph +=  to_string(aux->getValue().getID()) + ";";
			}else{
            	graph +=  to_string(aux->getValue().getID()) + " -> ";
			}
        }
        else{
			if(i==lista->GetSize()-1){
                    graph +=  to_string(aux->getValue().getID()) + ";\n";
            }else{
					graph +=  to_string(aux->getValue().getID()) + " -> ";
			}
        }
        aux=aux->getNext();
    	}
	ofstream graphFile;
	graphFile.open("txtFiles/ScoreBoard.txt");
    graphFile << graph+labels+"}";
    graphFile.close();
    std::string filePath="dot -Tpng txtFiles/ScoreBoard.txt -o Imgs/ScoreBoard.png";
    system(filePath.c_str());
	}
bool Compare(Ficha creada, Ficha nueva){
	if(creada.getChar()==" "){
		if(UpdateCasilla){
			creada.setChar(nueva.getChar());
			creada.setValor(nueva.getValor());
			Vacio=true;
		}
		Insertar=true;
		return true;
	}
	if(creada.getChar()==nueva.getChar()){
	//	cout<<creada.getChar()<<"="<<nueva.getChar()<<endl;
		return true;
	} 
		cout<<creada.getChar()<<"!="<<nueva.getChar()<<endl;
	return false;
	}

bool SameColumnNode(Nodo<Ficha> *nuevo, Nodo<Ficha> *cabezaColumna){
    Nodo<Ficha> *aux = cabezaColumna;
    bool bandera = false;
    while (true)
    {
        if (aux->getX() == nuevo->getX())
        {
            if(Compare(aux->getValue(), nuevo->getValue())){
                    return true;
                }
			//creo que aca se tendría que poner el valor
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
bool SameFilaNode(Nodo<Ficha> *nuevo, Nodo<Ficha> *cabezaFila){
        Nodo<Ficha> *aux = cabezaFila;
        bool bandera = false;
        while (true)
        {
            if (aux->getY() == nuevo->getY())
            {
                if(Compare(aux->getValue(), nuevo->getValue())){
                    return true;
                }
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
bool SameNode(Ficha value, int x, int y){
    Nodo<Ficha> *nuevo= new Nodo<Ficha>(value, x, y);
    Nodo<Ficha> *NodoColumna, *NodoFila;
    NodoFila = Tablero->BuscarEnY(y);
    NodoColumna = Tablero->BuscarEnX(x);
    if (NodoColumna != NULL && NodoFila != NULL)
    {
        if(Tablero->ColumnaOcupada(nuevo, NodoFila) && Tablero->FilaOcupada(nuevo, NodoColumna)){
            if(SameColumnNode(nuevo, NodoFila) && SameFilaNode(nuevo, NodoColumna)){
                return true;
            }
        }
    }
    return false;
    }
string ImprimirTablero(MatrizDispersa<Ficha> *ma){
    string content ="digraph G { \n node[shape=box]\n ";
	string nivelador;
	string rankedLevel;
    string labels="";
	Nodo<Ficha> *auxiliar = ma->getRoot();    
	while(auxiliar!=NULL){
        Nodo<Ficha> *temp = auxiliar;
		nivelador="";
		rankedLevel="{rank =same;";
        while(temp!=NULL){
			
            if(temp->getX()==-1 && temp->getY()==-1){
				nivelador+="root[group=1];\n";
				rankedLevel+="root";			
				//content+= "pinting root\n";
                //imprimir derecha
				content+=" root[label=raiz]\n ";
                content+=" root -> X"+to_string(temp->getNext()->getX());
				content+="[constraint=true, dir=both];\n";
				//imprimir abajo
				content+=" root -> Y"+to_string(temp->getDown()->getY());
				content+="[dir=both];\n";				
            }else if(temp->getY()==-1){//cabeza columnas
				//content+= "pinting column head\n";
				rankedLevel+=";X"+to_string(temp->getX());
                labels+="X"+to_string(temp->getX())+"[label=\""+to_string(temp->getX())+"\"];\n";
				nivelador+="X"+to_string(temp->getX())+"[group="+to_string(temp->getX()+2)+"];\n";
              // cout<< temp->getX() <<"  " ; 
			   //imprimir izquierda
			  /* 	if(temp->getPrevious()!=NULL){
					if(temp->getPrevious()->getX()==-1 && temp->getPrevious()->getY()==-1){
						content+="X"+to_string(temp->getX())+" -> root[constraint=false]";
						content+=";\n";
					}else{
						content+="X"+to_string(temp->getX())+" -> X"+to_string(temp->getPrevious()->getX());
						content+="[constraint=false];\n";
					}
			   	}*/
                //imprimir derecha
				if(temp->getNext()!=NULL){
					content+="X"+to_string(temp->getX())+" -> X"+to_string(temp->getNext()->getX());
					content+="[dir=both, constraint=true];\n";
				}
				//imprimir abajo	
				if(temp->getDown()!=NULL){
					content+="X"+to_string(temp->getX())+" -> C"+to_string(temp->getDown()->getX())+"F"+to_string(temp->getDown()->getY());
					content+="[dir=both];\n";
				}
            }else if(temp->getX()==-1){//cabeza filas
                labels+="Y"+to_string(temp->getY())+"[label=\""+to_string(temp->getY())+"\"];\n";
				rankedLevel+="Y"+to_string(temp->getY());
				nivelador+="Y"+to_string(temp->getY())+"[group=1];\n";
				//content+= "pinting row head\n";
             //  cout<< temp->getY() <<"\n" ;
                //imprimir derecha
				if(temp->getNext()!=NULL){
					content+="Y"+to_string(temp->getY())+" -> C"+ to_string(temp->getNext()->getX())+"F"+to_string(temp->getNext()->getY());
					content+="[dir=both, constraint=true];\n";
				}
			    //imprimir arriba	
			/*	if(temp->getUp()!=NULL){
					if(temp->getUp()->getX()==-1 && temp->getUp()->getY()==-1){
						content+="Y"+to_string(temp->getY())+" -> root";
						content+=";\n";
					}else{
						content+="Y"+to_string(temp->getY())+" -> Y"+to_string(temp->getUp()->getY());
						content+=";\n";
					}
				}	*/
				//imprimir abajo
				if(temp->getDown()!=NULL){
					content+="Y"+to_string(temp->getY())+" -> Y"+to_string(temp->getDown()->getY());
					content+="[dir=both];\n";
				}	
            }else {//contenido de la matriz
                labels+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+"[label=\""+temp->getValue().getChar()+"\"];\n";
				rankedLevel+=";C"+to_string(temp->getX())+"F"+to_string(temp->getY());
				nivelador+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+"[group="+to_string(temp->getX()+2)+"];\n";
				//cout<<"contiene "+ temp->getValue()+"en ("<<to_string(temp->getX())+","+to_string(temp->getY())<<")";
             //  cout<< temp->getValue() <<" ";
			    //imprimir izquierda
			/*	if(temp->getPrevious()!=NULL){
					if(temp->getPrevious()->getX()==-1){
					//	content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> Y"+to_string(temp->getPrevious()->getY());
						content+="[constraint=false];\n";
					}else{
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getPrevious()->getX())+"F"+to_string(temp->getPrevious()->getY());
						content+="[constraint=false];\n";
					}
				}
               */ //imprimir derecha 
				if(temp->getNext()!=NULL){
					content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getNext()->getX())+"F"+to_string(temp->getNext()->getY());
					content+="[dir=both,constraint=true];\n";
				}
			 /*   //imprimir arriba
				if(temp->getUp()!=NULL){
					if(temp->getUp()->getY()==-1){
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> X"+to_string(temp->getUp()->getX());
						content+=";\n";
					}else{
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getUp()->getX())+"F"+to_string(temp->getUp()->getY());
						content+=";\n";
					}
				}*/
				//imprimir abajo
				if(temp->getDown()!=NULL){
					content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getDown()->getX())+"F"+to_string(temp->getDown()->getY());
					content+="[dir=both];\n";
				}
            }            
            temp=temp->getNext();
        }
		content+=nivelador+"\n";
		/*
		if(YRows-1>=0){
			content+=rankedLevel+";e"+to_string(YRows-1)+"}\n";
		}else{
		}
		*/		
		content+=rankedLevel+"}\n";
        auxiliar = auxiliar->getDown();
    }
	/*
	string extraNodes="";
	string YLeveler ="X"+to_string(Xcolumns-1);
	string RowsLevel="e"+to_string(ENodes-1)+"->"+LastPosition+";\n e"+to_string(ENodes-1)+" -> Y"+to_string(ENodes-1);
	for(int i =0;i<ENodes;i++){
		extraNodes+="e"+to_string(i)+"[shape=point, width=0];\n";
		YLeveler+="->e"+to_string(i);
	}
	YLeveler+="[dir=none];";
	*/
	Nodo<Casillas> *aux = casillasEspeciales->GetCabeza();
		for(int i =0;i<casillasEspeciales->GetSize();i++){
			if(aux->getValue().getTipo()=="Doble"){
				labels+="C"+to_string(aux->getValue().getX())+"F"+to_string(aux->getValue().getY())+"[color=deepskyblue style=filled];\n";
			}else if(aux->getValue().getTipo()=="Triple"){
				labels+="C"+to_string(aux->getValue().getX())+"F"+to_string(aux->getValue().getY())+"[color=salmon2 style=filled];\n";
			}
			aux=aux->getNext();
		}
	return content+labels+"\n}";
	}
string ImprimirMatriz(MatrizDispersa<string> *matriz){
	string content ="digraph G { \n splines = polyline;\n node[shape=record]\n ";
	Nodo<string> *auxiliar = matriz->getRoot();
    while(auxiliar!=NULL){
        Nodo<string> *temp = auxiliar;
        while(temp->getNext()!=NULL){
            if(temp->getX()==-1 && temp->getY()==-1){
				content+=" root[label=raiz]\n ";
                content+=" root -> "+temp->getNext()->getX();
				content+=" \n ";
                content+=" root -> "+temp->getDown()->getY();
				content+=" \n ";
            }else if(temp->getY()==-1){
                if(temp->getNext()!=NULL){
					content+=temp->getX()+" -> "+temp->getNext()->getX();
					content+=" \n ";
					content+=temp->getX()+" -> "+temp->getDown()->getX();
					content+="_"+temp->getDown()->getY();
					content+=" \n ";
					content+=temp->getNext()->getX()+" -> "+temp->getX();
					content+=" \n ";
				}else{
					content+=temp->getX()+" -> "+temp->getNext()->getX();
					content+=" \n ";
					content+=temp->getX()+" -> "+temp->getDown()->getX();
					content+="_"+temp->getDown()->getY();
					content+=" \n ";
				}
            }else if(temp->getX()==-1){
               if(temp->getDown()!=NULL){
					content+=temp->getY()+" -> "+temp->getDown()->getY();
					content+=" \n ";
					content+=temp->getY()+" -> "+temp->getNext()->getX();
					content+="_"+temp->getNext()->getY();
					content+=" \n ";
					content+=temp->getDown()->getX()+" -> "+temp->getX();
					content+=" \n ";
				}else{
					content+=temp->getY()+" -> "+temp->getNext()->getY();
					content+=" \n ";
					content+=temp->getY()+" -> "+temp->getNext()->getX();
					content+="_"+temp->getNext()->getY();
					content+=" \n ";
				}
            }else {
               if(temp->getNext()!=NULL){
				   content+=temp->getX()+"_"+temp->getY();
				   content+=" -> "+temp->getNext()->getX();
				   content+="_"+temp->getNext()->getY();
			   }
			   if(temp->getDown()!=NULL){
				   content+=temp->getX()+"_"+temp->getY();
				   content+=" -> "+temp->getDown()->getX();
				   content+="_"+temp->getDown()->getY();
			   }
			   if(temp->getUp()!=NULL){
				   content+=temp->getX()+"_"+temp->getY();
				   content+=" -> "+temp->getUp()->getX();
				   content+="_"+temp->getUp()->getY();
			   }
			   if(temp->getPrevious()!=NULL){
				   content+=temp->getX()+"_"+temp->getY();
				   content+=" -> "+temp->getPrevious()->getX();
				   content+="_"+temp->getPrevious()->getY();
			   }
            }            
            temp=temp->getNext();
        }
        //cout<<"\n";
        auxiliar = auxiliar->getDown();
    }
	return content;
	}
string PrintW(MatrizDispersa<string> *ma){
	string content ="digraph G { \n node[shape=box]\n ";
	string nivelador;
	string rankedLevel;
	Nodo<string> *auxiliar = ma->getRoot();    
	while(auxiliar!=NULL){
        Nodo<string> *temp = auxiliar;
		nivelador="";
		rankedLevel="{rank =same;";
        while(temp!=NULL){
			
            if(temp->getX()==-1 && temp->getY()==-1){
				nivelador+="root[group=1];\n";
				rankedLevel+="root";			
				//content+= "pinting root\n";
                //imprimir derecha
				content+=" root[label=raiz]\n ";
                content+=" root -> X"+to_string(temp->getNext()->getX());
				content+="\n";
				//imprimir abajo
				content+=" root -> Y"+to_string(temp->getDown()->getY());
				content+="\n";				
            }else if(temp->getY()==-1){//cabeza columnas
				//content+= "pinting column head\n";
				rankedLevel+=";X"+to_string(temp->getX());
				nivelador+="X"+to_string(temp->getX())+"[group="+to_string(temp->getX()+1)+"];\n";
              // cout<< temp->getX() <<"  " ; 
			   //imprimir izquierda
			   	if(temp->getPrevious()!=NULL){
					if(temp->getPrevious()->getX()==-1 && temp->getPrevious()->getY()==-1){
						content+="X"+to_string(temp->getX())+" -> root";
						content+="\n";
					}else{
						content+="X"+to_string(temp->getX())+" -> X"+to_string(temp->getPrevious()->getX());
						content+="\n";
					}
			   	}
                //imprimir derecha
				if(temp->getNext()!=NULL){
					content+="X"+to_string(temp->getX())+" -> X"+to_string(temp->getNext()->getX());
					content+="\n";
				}
				//imprimir abajo	
				if(temp->getDown()!=NULL){
					content+="X"+to_string(temp->getX())+" -> C"+to_string(temp->getDown()->getX())+"F"+to_string(temp->getDown()->getY());
					content+="\n";
				}
            }else if(temp->getX()==-1){//cabeza filas
				rankedLevel+="Y"+to_string(temp->getY());
				nivelador+="Y"+to_string(temp->getY())+"[group=1];\n";
				//content+= "pinting row head\n";
             //  cout<< temp->getY() <<"\n" ;
                //imprimir derecha
				if(temp->getNext()!=NULL){
					content+="Y"+to_string(temp->getY())+" -> C"+ to_string(temp->getNext()->getX())+"F"+to_string(temp->getNext()->getY());
					content+="\n";
				}
			    //imprimir arriba	
				if(temp->getUp()!=NULL){
					if(temp->getUp()->getX()==-1 && temp->getUp()->getY()==-1){
						content+="Y"+to_string(temp->getY())+" -> root";
						content+="\n";
					}else{
						content+="Y"+to_string(temp->getY())+" -> Y"+to_string(temp->getUp()->getY());
						content+="\n";
					}
				}	
				//imprimir abajo
				if(temp->getDown()!=NULL){
					content+="Y"+to_string(temp->getY())+" -> Y"+to_string(temp->getDown()->getY());
					content+="\n";
				}	
            }else {//contenido de la matriz
				rankedLevel+=";C"+to_string(temp->getX())+"F"+to_string(temp->getY());
				nivelador+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+"[group="+to_string(temp->getX()+1)+"];\n";
				//cout<<"contiene "+ temp->getValue()+"en ("<<to_string(temp->getX())+","+to_string(temp->getY())<<")";
             //  cout<< temp->getValue() <<" ";
			    //imprimir izquierda
				if(temp->getPrevious()!=NULL){
					if(temp->getPrevious()->getX()==-1){
					//	content+="/*Izq de nodo cont*/"+to_string(temp->getPrevious()->getX())+","+to_string(temp->getPrevious()->getY());
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> Y"+to_string(temp->getPrevious()->getY());
						content+="\n";
					}else{
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getPrevious()->getX())+"F"+to_string(temp->getPrevious()->getY());
						content+="\n";
					}
				}
                //imprimir derecha 
				if(temp->getNext()!=NULL){
					content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getNext()->getX())+"F"+to_string(temp->getNext()->getY());
					content+="\n";
				}
			    //imprimir arriba
				if(temp->getUp()!=NULL){
					if(temp->getUp()->getY()==-1){
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> X"+to_string(temp->getUp()->getX());
						content+="\n";
					}else{
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getUp()->getX())+"F"+to_string(temp->getUp()->getY());
						content+="\n";
					}
				}
				//imprimir abajo
				if(temp->getDown()!=NULL){
					content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getDown()->getX())+"F"+to_string(temp->getDown()->getY());
					content+="\n";
				}
            }            
            temp=temp->getNext();
        }
		content+=nivelador+"\n";
		/*
		if(YRows-1>=0){
			content+=rankedLevel+";e"+to_string(YRows-1)+"}\n";
		}else{
		}
		*/
		content+=rankedLevel+"}\n";
        auxiliar = auxiliar->getDown();
    }
	/*
	string extraNodes="";
	string YLeveler ="X"+to_string(Xcolumns-1);
	string RowsLevel="e"+to_string(ENodes-1)+"->"+LastPosition+";\n e"+to_string(ENodes-1)+" -> Y"+to_string(ENodes-1);
	for(int i =0;i<ENodes;i++){
		extraNodes+="e"+to_string(i)+"[shape=point, width=0];\n";
		YLeveler+="->e"+to_string(i);
	}
	YLeveler+="[dir=none];";
	*/
	return content+"\n}";
	}

string ImprimirArribaTab(MatrizDispersa<Ficha> *ma){
    string content ="digraph G { \n node[shape=box]\n ";
	string nivelador;
	string rankedLevel;
    string labels="";
	Nodo<Ficha> *auxiliar = ma->getRoot();    
	while(auxiliar!=NULL){
        Nodo<Ficha> *temp = auxiliar;
		nivelador="";
		rankedLevel="{rank =same;";
        while(temp!=NULL){
			
            if(temp->getX()==-1 && temp->getY()==-1){
				nivelador+="root[group=1];\n";
				rankedLevel+="root";			
				//content+= "pinting root\n";
                //imprimir derecha
				content+=" root[label=raiz]\n ";
              //  content+=" root -> X"+to_string(temp->getNext()->getX());
			//	content+="[constraint=false, dir=both];\n";
			/**/	//imprimir abajo
			//	content+=" root -> Y"+to_string(temp->getDown()->getY());
			//	content+="[dir=both];\n";	/**/			
            }else if(temp->getY()==-1){//cabeza columnas
				//content+= "pinting column head\n";
				rankedLevel+=";X"+to_string(temp->getX());
                labels+="X"+to_string(temp->getX())+"[label=\""+to_string(temp->getX())+"\"];\n";
				nivelador+="X"+to_string(temp->getX())+"[group="+to_string(temp->getX()+2)+"];\n";
              // cout<< temp->getX() <<"  " ; 
			   //imprimir izquierda
			   	if(temp->getPrevious()!=NULL){
					if(temp->getPrevious()->getX()==-1 && temp->getPrevious()->getY()==-1){
						content+="X"+to_string(temp->getX())+" -> root[constraint=false, dir=both];\n";
					}else{
						content+="X"+to_string(temp->getX())+" -> X"+to_string(temp->getPrevious()->getX());
						content+="[constraint=false, dir=both];\n";
					}
			   	}
                //imprimir derecha
			/*	if(temp->getNext()!=NULL){
					content+="X"+to_string(temp->getX())+" -> X"+to_string(temp->getNext()->getX());
					content+="[dir=both, constraint=false];\n";
				}
				//imprimir abajo	
				if(temp->getDown()!=NULL){
					content+="X"+to_string(temp->getX())+" -> C"+to_string(temp->getDown()->getX())+"F"+to_string(temp->getDown()->getY());
					content+="[dir=both];\n";
				}*/
            }else if(temp->getX()==-1){//cabeza filas
                labels+="Y"+to_string(temp->getY())+"[label=\""+to_string(temp->getY())+"\"];\n";
				rankedLevel+="Y"+to_string(temp->getY());
				nivelador+="Y"+to_string(temp->getY())+"[group=1];\n";
				//content+= "pinting row head\n";
             //  cout<< temp->getY() <<"\n" ;
                //imprimir derecha
			/*	if(temp->getNext()!=NULL){
					content+="Y"+to_string(temp->getY())+" -> C"+ to_string(temp->getNext()->getX())+"F"+to_string(temp->getNext()->getY());
					content+="[dir=both, constraint=false];\n";
				}*/
			    //imprimir arriba	
				if(temp->getUp()!=NULL){
					if(temp->getUp()->getX()==-1 && temp->getUp()->getY()==-1){
						content+="Y"+to_string(temp->getY())+" -> root[dir=both];\n";
					}else{
						content+="Y"+to_string(temp->getY())+" -> Y"+to_string(temp->getUp()->getY());
						content+="[dir=both];\n";
					}
				}	
				//imprimir abajo
			/*	if(temp->getDown()!=NULL){
					content+="Y"+to_string(temp->getY())+" -> Y"+to_string(temp->getDown()->getY());
					content+="[dir=both];\n";
				}*/	
            }else {//contenido de la matriz
                labels+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+"[label=\""+temp->getValue().getChar()+"\"];\n";
				rankedLevel+=";C"+to_string(temp->getX())+"F"+to_string(temp->getY());
				nivelador+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+"[group="+to_string(temp->getX()+2)+"];\n";
				//cout<<"contiene "+ temp->getValue()+"en ("<<to_string(temp->getX())+","+to_string(temp->getY())<<")";
             //  cout<< temp->getValue() <<" ";
			    //imprimir izquierda
				if(temp->getPrevious()!=NULL){
					if(temp->getPrevious()->getX()==-1){
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> Y"+to_string(temp->getPrevious()->getY());
						content+="[constraint=false, dir=both];\n";
					}else{
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getPrevious()->getX())+"F"+to_string(temp->getPrevious()->getY());
						content+="[constraint=false, dir=both];\n";
					}
				}
                //imprimir derecha 
			/*	if(temp->getNext()!=NULL){
					content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getNext()->getX())+"F"+to_string(temp->getNext()->getY());
					content+="[dir=both,constraint=false];\n";
				}
			*/    //imprimir arriba
				if(temp->getUp()!=NULL){
					if(temp->getUp()->getY()==-1){
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> X"+to_string(temp->getUp()->getX());
						content+="[dir=both];\n";
					}else{
						content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getUp()->getX())+"F"+to_string(temp->getUp()->getY());
						content+="[dir=both];\n";
					}
				}/*
				//imprimir abajo
				if(temp->getDown()!=NULL){
					content+="C"+to_string(temp->getX())+"F"+to_string(temp->getY())+" -> C"+to_string(temp->getDown()->getX())+"F"+to_string(temp->getDown()->getY());
					content+="[dir=both];\n";
				}*/
            }            
            temp=temp->getDown();
        }
		content+=nivelador+"\n";
		/*
		if(YRows-1>=0){
			content+=rankedLevel+";e"+to_string(YRows-1)+"}\n";
		}else{
		}
		*/		
		content+=rankedLevel+"}\n";
        auxiliar = auxiliar->getNext();
    }
	/*
	string extraNodes="";
	string YLeveler ="X"+to_string(Xcolumns-1);
	string RowsLevel="e"+to_string(ENodes-1)+"->"+LastPosition+";\n e"+to_string(ENodes-1)+" -> Y"+to_string(ENodes-1);
	for(int i =0;i<ENodes;i++){
		extraNodes+="e"+to_string(i)+"[shape=point, width=0];\n";
		YLeveler+="->e"+to_string(i);
	}
	YLeveler+="[dir=none];";
	*/
	Nodo<Casillas> *aux = casillasEspeciales->GetCabeza();
		for(int i =0;i<casillasEspeciales->GetSize();i++){
			if(aux->getValue().getTipo()=="Doble"){
				labels+="C"+to_string(aux->getValue().getX())+"F"+to_string(aux->getValue().getY())+"[color=deepskyblue style=filled];\n";
			}else if(aux->getValue().getTipo()=="Triple"){
				labels+="C"+to_string(aux->getValue().getX())+"F"+to_string(aux->getValue().getY())+"[color=salmon2 style=filled];\n";
			}
			aux=aux->getNext();
		}
	return content+labels+"\n}";
	}
void RecorrerArbol(Nodo<jugadores> *Central, ListaSimple<jugadores> *lista){
		if (Central->getPrevious() != NULL)//Left
        {          
            RecorrerArbol(Central->getPrevious(), lista); 
        }
        lista->Insertar(Central->getValue());   
        if (Central->getNext() != NULL)//Right
        {
            RecorrerArbol(Central->getNext(), lista);
        }
	} 
void PrintPlayers(){

	 JugadoresOrdenados = new ListaSimple<jugadores>();
	 RecorrerArbol(arbol->GetRoot(), JugadoresOrdenados);
	 for(int i=0;i<JugadoresOrdenados->GetSize();i++){
		 cout<<to_string(i+1)<<". "<< JugadoresOrdenados->ElementAt(i)->getValue().getName()<<endl;
	 }
	}
void GraphDoubleList(ListaDoble<Ficha> *lista, ListaDoble<Ficha> *lista2){
    std::string graph ("digraph ReporteLD { graph [dpi=300]\n rankdir =\"LR\"; \n size=\"5\" \n node [shape = box]; \n");
    graph+="subgraph cluster_gc_1 { label = \"Fichas del "+jugador1.getName()+" "+to_string(punteoJugador1)+"\";\n";
	Nodo<Ficha> *aux = lista->GetCabeza();
    Nodo<Ficha> *previous = aux;
    for(int i =0; i<lista->GetSize(); i++){
        string number = "_1"+ std::to_string(i);
        string prevNumber = "_1"+std::to_string((i-1));
        //NULL pointers to the graph edges
        if(i==0){
            //graph +=  aux->getValue().getChar() + std::string( number + " -> NULL_;\n");
        }
        //The rest of the nodes
        else{
            previous = aux;
            aux=aux->getNext();
            graph += previous->getValue().getChar() + std::string(prevNumber + " -> " + aux->getValue().getChar() + number +";\n");
            graph += aux->getValue().getChar() + std::string( number + " -> " + previous->getValue().getChar() + prevNumber +";\n");
            if(i==lista->GetSize()-1){
                    //graph +=  aux->getValue() + std::string( number + " -> _NULL;\n}");
            }
        }
    	}
	aux = lista->GetCabeza();
	for(int i =0; i <lista->GetSize();i++){
		graph+=aux->getValue().getChar()+"_1"+to_string(i)+"[label="+aux->getValue().getChar()+"];\n";
		aux=aux->getNext();
		}
	graph+="}\n";
    graph+="subgraph cluster_gc_2 { label = \"Fichas del "+jugador2.getName()+" "+to_string(punteoJugador2)+"\";\n";
	aux = lista2->GetCabeza();
    previous = aux;
    for(int i =0; i<lista2->GetSize(); i++){
        string number = "_2"+ std::to_string(i);
        string prevNumber = "_2"+std::to_string((i-1));
        //NULL pointers to the graph edges
        if(i==0){
            //graph +=  aux->getValue().getChar() + std::string( number + " -> NULL_;\n");
        }
        //The rest of the nodes
        else{
            previous = aux;
            aux=aux->getNext();
            graph += previous->getValue().getChar() + std::string(prevNumber + " -> " + aux->getValue().getChar() + number +";\n");
            graph += aux->getValue().getChar() + std::string( number + " -> " + previous->getValue().getChar() + prevNumber +";\n");
            if(i==lista2->GetSize()-1){
                    //graph +=  aux->getValue() + std::string( number + " -> _NULL;\n}");
            }
        }
    }
	aux = lista2->GetCabeza();
	for(int i =0; i <lista2->GetSize();i++){
		graph+=aux->getValue().getChar()+"_2"+to_string(i)+"[label="+aux->getValue().getChar()+"];\n";
		aux=aux->getNext();
	}
	ofstream graphFile;
	graphFile.open("txtFiles/FichasEnJuego.txt");
    graphFile << graph+"}\n}";
    graphFile.close();
    std::string filePath="dot -Tpng txtFiles/FichasEnJuego.txt -o Imgs/FichasEnJuego.png";
    system(filePath.c_str());
	}
int SacarFicha(jugadores enTurno, string letra){
	for(int i =0;i<enTurno.getFichas()->GetSize();i++){
		if(enTurno.getFichas()->ElementAt(i)->getValue().getChar()==letra){
			return i;
		}
	}
	}
void ImprimirBolsa(){
	Nodo<Ficha> *aux = Bolsa->ReturnTop();
	std::string graph ("digraph ReporteLD { graph [dpi=1000]\n rankdir =\"TB\"; \n size=\"5\" \n node [shape = box]; \n");
	string labels;
	int i=0;
	while(aux!=NULL){
		labels+=to_string(i)+"[label=\""+aux->getValue().getChar()+" x "+to_string(aux->getValue().getValor())+"pts\"];\n";
		if(aux->getNext()!=NULL){
			graph+=to_string(i)+"->";
		}else{
			graph+=to_string(i)+";\n";
		}
		aux=aux->getNext();
		i++;
	}
	ofstream graphFile;
	graphFile.open("txtFiles/ColaDeFichas.txt");
    graphFile << graph+labels+"\n}";
    graphFile.close();
    std::string filePath="dot -Tpng txtFiles/ColaDeFichas.txt -o Imgs/ColaDeFichas.png";
    system(filePath.c_str());
	}
void ImprimirDiccionario(ListaDobleCircular<string> *lista){
    std::string graph ("digraph ReporteLD { graph [dpi=1000]\n rankdir =\"LR\"; \n size=\"5\" \n node [shape = box]; \n");
    graph+="label = \"Diccionario\";\n";
	Nodo<string> *aux = lista->GetCabeza();
    Nodo<string> *previous = aux;
    for(int i =0; i<lista->GetSize(); i++){
        string number =  std::to_string(i);
        string prevNumber = std::to_string((i-1));
        //NULL pointers to the graph edges
        if(i==0){
           graph +=  to_string(i) + "-> " +std::string( to_string(lista->GetSize()-1) + ";\n");
        }
        //The rest of the nodes
        else{
            previous = aux;
            aux=aux->getNext();
            graph += std::string(prevNumber + " -> " +  number +";\n");
            graph += std::string( number + " -> " + prevNumber +";\n");
            if(i==lista->GetSize()-1){
                    //graph +=  aux->getValue() + std::string( number + " -> _NULL;\n}");
           			graph +=  to_string(lista->GetSize()-1) + "-> " +std::string( to_string(0) + ";\n");
            }
        }
    	}
	aux = lista->GetCabeza();
	for(int i =0; i <lista->GetSize();i++){
		graph+=to_string(i)+"[label="+aux->getValue()+"];\n";
		aux=aux->getNext();
		}    
	ofstream graphFile;
	graphFile.open("txtFiles/Diccionario.txt");
    graphFile << graph+"}";
    graphFile.close();
    std::string filePath="dot -Tpng txtFiles/Diccionario.txt -o Imgs/Diccionario.png";
    system(filePath.c_str());
	}
void FichasEnJuego(){
		GraphDoubleList(jugador1.getFichas(), jugador2.getFichas());
	}
bool PalabraExistente(string palabra){
	//cout<<"Verificando si existe la palabra "<<palabra;
	Nodo<string> *insercion = diccionario->GetCabeza();
	for(int i =0;i <diccionario->GetSize();i++){
		if(insercion->getValue()==palabra){
			return true;
		}
		insercion=insercion->getNext();
	}
	return false;
	}
bool TieneEstaFicha(jugadores enTurno, string letra){	
		Nodo<Ficha> *aux= enTurno.getFichas()->GetCabeza();
		for(int e =0; e<enTurno.getFichas()->GetSize();e++){
				if(aux->getValue().getChar()[0]==letra[0]){
					return true;
				}
				aux=aux->getNext();
			}
		return false;
	}
int ValorDeFicha(string letra, jugadores enTurno){
	for(int i=0;i<enTurno.getFichas()->GetSize();i++){
		if(enTurno.getFichas()->ElementAt(i)->getValue().getChar()==letra){
			return enTurno.getFichas()->ElementAt(i)->getValue().getValor();
		}
	}
	return 0;
	}
void UpdateTablero(){
	ofstream graphFile;
    graphFile.open("txtFiles/Tablero.txt");
    graphFile << ImprimirTablero(Tablero);
    graphFile.close();
    std::string filePath="dot -Tpng txtFiles/Tablero.txt -o Imgs/Tablero.png";
    system(filePath.c_str());
	//prueba de otra forma de graficar
	/*graphFile.open("TableroArriba.txt");
    graphFile << ImprimirArribaTab(Tablero);
    graphFile.close();
    filePath="dot -Tpng TableroArriba.txt -o TableroArriba.png";
    system(filePath.c_str());*/
	}
void InsertarOrdenado(jugadores jugador, int puntaje){
	Nodo<int> *aux = jugador.getScores()->GetCabeza();
	if(aux==NULL){
		jugador.getScores()->Insertar(puntaje);
	}else{
		int i=0;
		while(puntaje<aux->getValue()){
		//	cout<<to_string(puntaje)<<" < "<<aux->getValue()<<endl;
			if(aux->getNext()!=NULL){
				i++;
				aux=aux->getNext();				
			}else{
				break;
			}
		}
		if(aux->getNext()==NULL){
			if(puntaje<aux->getValue()){
			//	cout<<to_string(puntaje)<<" se debe ingresar después de "<<aux->getValue()<<endl;
				jugador.getScores()->Insertar(puntaje);
			}else{	
			//	cout<<to_string(puntaje)<<", se debe ingresar antes de "<<aux->getValue()<<endl;
				jugador.getScores()->InsertBefore(puntaje, i);
			}
		}else{			
		//	cout<<to_string(puntaje)<<" se debe ingresar antes de "<<aux->getValue()<<endl;	
			jugador.getScores()->InsertBefore(puntaje, i);
		}
				
	}
	}
void OrdenarJugadores(ListaSimple<jugadores> *lista, jugadores jugador){
	Nodo<jugadores> *aux = lista->GetCabeza();
	if(aux==NULL){
		lista->Insertar(jugador);
	}else if(jugador.getScores()->GetSize()==0){
		lista->Insertar(jugador);
	}else if(aux->getValue().getScores()->GetSize()==0){
		lista->InsertBefore(jugador, 0);
	}else{
		int i=0;
		while(jugador.getScores()->GetCabeza()->getValue()<aux->getValue().getScores()->GetCabeza()->getValue()){
		//	cout<<to_string(jugador.getScores()->GetCabeza()->getValue())<<" < "<<to_string(aux->getValue().getScores()->GetCabeza()->getValue())<<endl;
			if(aux->getNext()!=NULL){
				i++;
				aux=aux->getNext();				
			}else{
				break;
			}
		}
		if(aux->getNext()==NULL){
			if(jugador.getScores()->GetCabeza()->getValue()<aux->getValue().getScores()->GetCabeza()->getValue()){
		//		cout<<to_string(jugador.getScores()->GetCabeza()->getValue())<<" se debe ingresar después de "<<to_string(aux->getValue().getScores()->GetCabeza()->getValue())<<endl;
				lista->Insertar(jugador);
			}else{	
				cout<<to_string(jugador.getScores()->GetCabeza()->getValue())<<", se debe ingresar antes de "<<to_string(aux->getValue().getScores()->GetCabeza()->getValue())<<endl;
				lista->InsertBefore(jugador, i);
			}
		}else{			
		//	cout<<to_string(jugador.getScores()->GetCabeza()->getValue())<<" se debe ingresar antes de "<<to_string(aux->getValue().getScores()->GetCabeza()->getValue())<<endl;	
			lista->InsertBefore(jugador, i);
		}
				
	}
	}
void OrdenarLista(ListaSimple<jugadores> *lista){
	Nodo<jugadores> *primero, *segundo;
	for(int j=0;j<lista->GetSize();j++){
		for(int i =0;i<lista->GetSize()-1;i++){
			primero=lista->ElementAt(i);
			cout<<"Comparando "<<primero->getValue().getName()<<" con "<<segundo->getValue().getName();
			segundo=lista->ElementAt(i+1);
			if(primero->getValue().getScores()->GetSize()>0 && segundo->getValue().getScores()->GetSize()>0){
				if(primero->getValue().getScores()->GetCabeza()>segundo->getValue().getScores()->GetCabeza()){
					jugadores temporal = segundo->getValue();
					segundo->setValue(primero->getValue());
					primero->setValue(temporal);
				}
			}else if(primero->getValue().getScores()->GetSize()>0 && segundo->getValue().getScores()->GetSize()==0){
				/*	jugadores temporal = segundo->getValue();
					segundo->setValue(primero->getValue());
					primero->setValue(temporal);*/
			}else if(segundo->getValue().getScores()->GetSize()>0 && primero->getValue().getScores()->GetSize()==0){
					jugadores temporal = segundo->getValue();
					segundo->setValue(primero->getValue());
					primero->setValue(temporal);
			}
		}
	}
	}
void PrintBestScores(){
	ListaSimple<jugadores> *temporal = new ListaSimple<jugadores>();
	RecorrerArbol(arbol->GetRoot(), temporal);
	//OrdenarLista(temporal);
	ListaSimple<jugadores> *ordenada = new ListaSimple<jugadores>();
	for(int i =0;i<temporal->GetSize();i++){
		OrdenarJugadores(ordenada, temporal->ElementAt(i)->getValue());
	}
	Nodo<jugadores> *temp = ordenada->GetCabeza();
	int i=1;
	while(temp!=NULL){
		cout<<to_string(i)<<". "<<temp->getValue().getName()<<"-----";
		if(temp->getValue().getScores()->GetCabeza()!=NULL){
			cout<<temp->getValue().getScores()->GetCabeza()->getValue()<<endl;
		}else{
			cout<<"no ha jugado ninuguna partida."<<endl;
		}
		i++;
		temp=temp->getNext();
	}
	PrintScoreBoard(ordenada);	
	}
void PrintBestPersonalScores(){

	}
int CasillaEspecial(int x, int y){
	Nodo<Casillas> *aux = casillasEspeciales->GetCabeza();
	for(int i =0;i<casillasEspeciales->GetSize();i++){
		if(aux->getValue().getX()==x && aux->getValue().getY()==y){
			if(aux->getValue().getTipo()=="Doble"){
				return 2;
			}else if(aux->getValue().getTipo()=="Triple"){
				return 3;
			}
		}
		aux=aux->getNext();
	}
	return 1;
	}
		//Métodos del juego
void NuevoTablero(){
	Tablero = new MatrizDispersa<Ficha>(Ficha("-", 0));
	Nodo<Casillas> *aux = casillasEspeciales->GetCabeza();
		for(int i=0;i<casillasEspeciales->GetSize();i++){
			//cout << to_string(aux->getValue().getX()) <<","<<to_string(aux->getValue().getY())<<endl;
			Tablero->Insertar(Ficha(" ", 0),aux->getValue().getX(),aux->getValue().getY());
			aux=aux->getNext();
		}
	}
int AdicionarPunteo(string letra, int x, int y){
	int valorletra = 0;
	if(letra=="Ñ"){
		valorletra=8;
		return valorletra*CasillaEspecial(x, y);
		}
	switch (letra[0])	{
		case 'A':
			valorletra=1;
			break;
		case 'B':
			valorletra=3;
			break;
		case 'C':
			valorletra=3;
			break;
		case 'D':
			valorletra=2;
			break;
		case 'E':
			valorletra=1;
			break;
		case 'F':
			valorletra=4;
			break;
		case 'G':
			valorletra=2;
			break;
		case 'H':
			valorletra=4;
			break;
		case 'I':
			valorletra=1;
			break;
		case 'J':
			valorletra=8;
			break;
		case 'L':
			valorletra=1;
			break;
		case 'M':
			valorletra=3;
			break;
		case 'N':
			valorletra=1;
			break;
		case 'O':
			valorletra=1;
			break;
		case 'P':
			valorletra=3;
			break;
		case 'Q':
			valorletra=5;
			break;
		case 'R':
			valorletra=1;
			break;
		case 'S':
			valorletra=1;
			break;
		case 'T':
			valorletra=1;
			break;
		case 'U':
			valorletra=1;
			break;
		case 'V':
			valorletra=4;
			break;
		case 'X':
			valorletra=8;
			break;
		case 'Y':
			valorletra=4;
			break;
		case 'Z':
			valorletra=10;
			break;
		}
	cout<<"Se suman "<<to_string(valorletra)<<"x"<<to_string(CasillaEspecial(x, y))<<" por la letra "<<letra<<endl;
	valorletra=valorletra*CasillaEspecial(x, y);

	return valorletra;
	}
bool InsertarTablero(string palabra, int x, int y, int VoH, jugadores enTurno, int *PunteoJugador){
	//cout<<"Ingresar Palabra "<<palabra<< "Verticalmente";
	string valor="";
	bool FichasNecesarias, puedoInsertar, Conectado;
	Nodo<Ficha> *aux;
	ListaSimple<Ficha> *Necesarias;
		Conectado=false;
		switch (VoH)
		{
		case 1://Ingreso Vertical
			puedoInsertar=true;
			Necesarias = new ListaSimple<Ficha>();
			for(int i=0;i<palabra.length();i++){//y es la que tiene que ir creciendo
				valor=palabra[i];
				if(Tablero->CasillaOcupada(Ficha(valor, ValorDeFicha(valor, enTurno)), x, y+i))//Verifico si la casilla está ocupada
				{	
				//	cout<<"comparando "<<valor<<" la ficha que está puesta"<<endl;
					Insertar=false;
					if(SameNode(Ficha(valor, ValorDeFicha(valor, enTurno)), x, y+i)){
						if(Iniciado){
							Conectado=true;
						}
					//	cout<<"La ficha es la misma"<<endl;
						if(Insertar){
							Necesarias->Insertar(Ficha(valor, ValorDeFicha(valor, enTurno)));
							Insertar=false;
						}
					}else{
						cout<<"La ficha NO es la misma"<<endl;
						puedoInsertar=false;
					}
				}else{
					//la casilla no está ocupada
					Necesarias->Insertar(Ficha(valor, ValorDeFicha(valor, enTurno)));
				}
			}
			
			FichasNecesarias=true;
			if(puedoInsertar){
				for(int i=0;i<Necesarias->GetSize();i++){
					if(!TieneEstaFicha(enTurno, Necesarias->ElementAt(i)->getValue().getChar())){
						FichasNecesarias=false;
					}
				}
			}else{
				cout<<"La palabra no puede ser incertada en esa posición"<<endl;
				return false;
			}
			UpdateCasilla=FichasNecesarias;

			aux = Necesarias->GetCabeza();
			cout<<"Las fichas necesarias son: ";
			for(int i =0;i<Necesarias->GetSize();i++){
				cout<<aux->getValue().getChar()<<" ";
				aux=aux->getNext();
			}
			cout<<endl;
			if(Bolsa->GetSize()>80){
				Conectado=true;
				}
			if(FichasNecesarias){
				if(Conectado){
				for(int i=0;i<palabra.length();i++){//y es la que tiene que ir creciendo
					valor=palabra[i];
					if(!Tablero->CasillaOcupada(Ficha(valor, ValorDeFicha(valor, enTurno)), x, y+i))//Verifico si la casilla está ocupada
					{	
						Tablero->Insertar(Ficha(valor, ValorDeFicha(valor, enTurno)), x, y+i);
						enTurno.getFichas()->SacarElemento(SacarFicha(enTurno, valor));
					}else
					{
						if(SameNode(Ficha(valor, ValorDeFicha(valor, enTurno)), x, y+i)){
							if(Vacio){
								Tablero->UpdateCasilla(Ficha(valor, ValorDeFicha(valor, enTurno)), x, y+i);
								enTurno.getFichas()->SacarElemento(SacarFicha(enTurno, valor));
							}
							Vacio=false;
						}
					}
					*PunteoJugador+=AdicionarPunteo(valor, x, y+i);
				}
				UpdateCasilla=false;
				return true;	
				}else{
					cout<<"La palabra no está conectada con las demás"<<endl;
					return false;
				}
			}else{
				cout<<"No tienes las fichas necesarias para esta palabra"<<endl;
				return false;
			}
			break;	
				
		case 2://Ingreso Horizonal
			puedoInsertar=true;
			Necesarias = new ListaSimple<Ficha>();
			for(int i=0;i<palabra.length();i++){//x es la que tiene que ir creciendo
				valor=palabra[i];
				if(Tablero->CasillaOcupada(Ficha(valor, ValorDeFicha(valor, enTurno)), x+i, y))//Verifico si la casilla está ocupada
				{	
					Insertar=false;
				//	cout<<"comparando "<<valor<<" la ficha que está puesta"<<endl;
					if(SameNode(Ficha(valor, ValorDeFicha(valor, enTurno)), x+i, y)){
						if(Iniciado){
								Conectado=true;
							}
					//	cout<<"La ficha es la misma"<<endl;
						if(Insertar){
							Necesarias->Insertar(Ficha(valor, ValorDeFicha(valor, enTurno)));
							Insertar=false;
						}
					}else{
						cout<<"La ficha NO es la misma"<<endl;
						puedoInsertar=false;
					}
				}else{
					//la casilla no está ocupada
					Necesarias->Insertar(Ficha(valor, ValorDeFicha(valor, enTurno)));
				}
			}
			
			FichasNecesarias=true;
			if(puedoInsertar){
				for(int i=0;i<Necesarias->GetSize();i++){
					if(!TieneEstaFicha(enTurno, Necesarias->ElementAt(i)->getValue().getChar())){
						FichasNecesarias=false;
					}
				}
			}else{
				cout<<"La palabra no puede ser incertada en esa posición";
				return false;
			}
			UpdateCasilla=FichasNecesarias;
			//codigo provicional para ver qué fichas necesito en cada incersión
			aux = Necesarias->GetCabeza();
			cout<<"Las fichas necesarias son: ";
			for(int i =0;i<Necesarias->GetSize();i++){
				cout<<aux->getValue().getChar()<<" ";
				aux=aux->getNext();
			}
			cout<<endl;


			if(FichasNecesarias){
				if(Conectado || Bolsa->GetSize()>80){
					for(int i=0;i<palabra.length();i++){//x es la que tiene que ir creciendo
						valor=palabra[i];
						if(!Tablero->CasillaOcupada(Ficha(valor, ValorDeFicha(valor, enTurno)), x+i, y))//Verifico si la casilla está ocupada
						{	
							Tablero->Insertar(Ficha(valor, ValorDeFicha(valor, enTurno)), x+i, y);
							enTurno.getFichas()->SacarElemento(SacarFicha(enTurno, valor));
						}else
						{
							if(SameNode(Ficha(valor, ValorDeFicha(valor, enTurno)), x+i, y)){
								if(Vacio){
									Tablero->UpdateCasilla(Ficha(valor, ValorDeFicha(valor, enTurno)), x+i, y);
									enTurno.getFichas()->SacarElemento(SacarFicha(enTurno, valor));
								}
								
							Vacio=false;
						}
					}
					*PunteoJugador+=AdicionarPunteo(valor, x+i, y);
				}
				UpdateCasilla=false;
					return true;
				}else{
					cout<<"La palabra no está conectada con las demás"<<endl;
					return false;
				}
			}else{
				cout<<"No tienes las fichas necesarias para esta palabra"<<endl;
				return false;
			}
			break;			
		}
	}
void RellenarFichero(jugadores enTurno){
	//	cout<<enTurno.getName()<<" tiene "<<to_string(enTurno.getFichas()->GetSize())<<" fichas"<<endl; 
	while(enTurno.getFichas()->GetSize()<7){
		enTurno.getFichas()->Insertar(Bolsa->Desencolar()->getValue());
	}
	FichasEnJuego();
	}
void DevolverFichas(jugadores enTurno){
	FichasEnJuego();
	if(enTurno.getFichas()->GetSize()>0){
		cout<<enTurno.getName()+" estas son tus fichas:"<<endl;
		Nodo<Ficha> *aux = enTurno.getFichas()->GetCabeza();
		for(int i=0;i<enTurno.getFichas()->GetSize();i++){
			cout<<to_string(i+1)+". "+aux->getValue().getChar()<<endl;
			aux=aux->getNext();
		}
		cout<<"Introduce el número de la ficha que deseas devolver: "<<endl;
		int fichaID;
		cin>>fichaID;
		if(fichaID<=enTurno.getFichas()->GetSize() && fichaID>0){
			//reintroducir ficha
			int posicionNueva = rand() % Bolsa->GetSize();
			Nodo<Ficha> *devuelto = enTurno.getFichas()->SacarElemento(fichaID-1);
			if(devuelto!=NULL){
				//	cout<<"la bolsa tenía "<<Bolsa->GetSize()<<endl;
					Bolsa->InsertAt(devuelto->getValue(),posicionNueva);
				//	cout<<"ahora tiene "<<Bolsa->GetSize()<<endl;
				}else{
					cout<<"No se pudo sacar la ficha de la posición indicada"<<endl;
				}
				int decision;
				cout<<"Se ha devuelto la ficha "+devuelto->getValue().getChar()+", deseas devolver otra ficha?\n 1.Si \n2.No"<<endl;
				cin>>decision;
				if(decision==1){
					DevolverFichas(enTurno);
				}else{
					//terminó su turno
				}
			}else{
				cout<<"La posición ingresada no es válida"<<endl;
			}
	}else{
		cout<<enTurno.getName()+" ya no tienes fichas"<<endl;
	}
	
	}
void Turno(jugadores enTurno, int *PunteoJugador){
	int x, y, VoH;
	cout<<enTurno.getName() <<" el valor de x donde comenzará tu palabra"<<endl;
	cin>>x;
	cout<<enTurno.getName() <<" el valor de y donde comenzará tu palabra"<<endl;
	cin>>y;
	if(x>DimensionTablero || y >DimensionTablero || x<0 || y<0){
		cout<<"La posición ("+to_string(x)+","+to_string(y)+") no está dentro del rango del tablero."<<endl;
	}else{
		cout<<enTurno.getName() <<" escoge la opción que desees\n1. Ingresar Vertical\n2. Ingresar Horizontal"<<endl;
		cin>>VoH;
		string palabra;
		cout<<"Introduce la palabra"<<endl;
		cin>>palabra;
		transform(palabra.begin(), palabra.end(), palabra.begin(), ::toupper);
		if(x+palabra.length()>DimensionTablero || y+palabra.length()>DimensionTablero){			
			cout<<"La palabra no cabe en el tablero."<<endl;
		}else{
			if(PalabraExistente(palabra)){
				//TieneEstaFicha(enTurno, palabra);
				InsertarTablero(palabra, x, y, VoH, enTurno, PunteoJugador);
			}else{
				cout<<"La palabra no es parte del lenguaje."<<endl;
			}
		}
	}
	
	}
void Jugar(){
		for(int i =0;i<7;i++){
			jugador1.getFichas()->Insertar(Bolsa->Desencolar()->getValue());
			}
		for(int i=0;i<7;i++){
			jugador2.getFichas()->Insertar(Bolsa->Desencolar()->getValue());
			}
		punteoJugador1=0;
		punteoJugador2=0;
		FichasEnJuego();
		ganador=false;
		bool SuTurno;//Si es true empieza el jugador 1
		int azar =rand() % 2;
		int Accion;		
		switch (azar)
			{
			case 0:
				azar=true;
				break;
			
			case 1:
				azar=false;
				break;
			}
		while(!ganador){
			ImprimirBolsa();
			if(Bolsa->GetSize()<80){
				Iniciado=true;
			}else{
				Iniciado=false;
			}
				if(SuTurno)//Turno del jugador 1
				{
					cout<< jugador1.getName()+" selecciona la acción que desees realizar\n1."+
												" Jugar Turno\n2. Cambiar Fichas\n3. Terminar Partida"<<endl;
					cin>>Accion;
					switch (Accion)
					{
					case 1:
						Turno(jugador1, &punteoJugador1);
					//	cout<<"Tu punteo al termiar tu turno es "<<to_string(punteoJugador1)<<endl;
						break;				
					case 2:
						DevolverFichas(jugador1);
						break;
					case 3:
						ganador=true;
						break;
					default:
						cout<<"No es una opción válida, pierdes tu turno"<<endl;
						break;
					}
					cout<<"Fin del trueno de "+jugador1.getName()<<endl<<endl;
					RellenarFichero(jugador1);
					SuTurno=false;
				}else{//Turno del jugador 2
					cout<< jugador2.getName()+" selecciona la acción que desees realizar\n1. "+
												"Jugar Turno\n2. Cambiar Fichas\n3. Terminar Partida"<<endl;
					cin>>Accion;
					switch (Accion)
					{
					case 1:
						Turno(jugador2, &punteoJugador2);
					//	cout<<"Tu punteo al termiar tu turno es "<<to_string(punteoJugador2)<<endl;
						break;				
					case 2:
						DevolverFichas(jugador2);
						break;
					case 3:
						ganador=true;
						break;
					default:
						cout<<"No es una opción válida, pierdes tu turno"<<endl;
						break;
					}
					cout<<"Fin del trueno de "+jugador2.getName()<<endl<<endl;
					RellenarFichero(jugador2);
					SuTurno=true;
				}
				if(Bolsa->Empty()){
					break;
					}
					UpdateTablero();
			}
		InsertarOrdenado(jugador1, punteoJugador1);
		InsertarOrdenado(jugador2, punteoJugador2);
		}
void EscogerJugadores(){
    system("clear");
	PrintPlayers();
	cout<<"Escoge el jugador 1"<<endl;
	int player1, player2;
	cin>>player1;
	cout<<"Escoge el jugador 2"<<endl;
	cin>>player2;
	if(player1==player2){
		cout<<"No puedes escoger 2 veces al mismo jugador";
	}else if(player1>JugadoresOrdenados->GetSize() || player1<=0){
		cout<<"Error, "<<to_string(player1)<<" no es una opción válida."<<endl;
	}else if(player2>JugadoresOrdenados->GetSize()|| player2<=0){
		cout<<"Error, "<<to_string(player2)<<" no es una opción válida."<<endl;
	}else{
		jugador1=JugadoresOrdenados->ElementAt(player1-1)->getValue();
		jugador2=JugadoresOrdenados->ElementAt(player2-1)->getValue();
		cout<<"Jugador 1: "<<jugador1.getName()<<" Jugador 2: "<<jugador2.getName()<<endl;
		Jugar();
	}
	}
void LlenarCola(){
	jugador1.getFichas()->Vaciar();
	jugador2.getFichas()->Vaciar();
	Bolsa->Vaciar();
	ListaSimple<Ficha> *TemporalBag = new ListaSimple<Ficha>();
	int FichasTotales =95;
	int A,B,C,D,E,F,G,H,I,J,K,L,M,N,Enie,O,P,Q,R,S,T,U,V,W,X,Y,Z;
	A=12;B=2;C=4;D=5;E=12;F=1;G=2;H=2;I=6;J=1;L=4;M=2;N=5;Enie=1;O=9;P=2;Q=1;R=5;S=6;T=4;U=5;V=1;X=1;Y=1;Z=1;
	srand((unsigned) time(0));
	int azar;
	for(int i =0; i<95;i++){	
			if(A>0){
				TemporalBag->Insertar(Ficha("A", 1));
				A--;
				FichasTotales--;
			}else if(B>0){
				TemporalBag->Insertar(Ficha("B", 3));
				B--;
				FichasTotales--;
			}else if(C>0){
				TemporalBag->Insertar(Ficha("C", 3));
				C--;
				FichasTotales--;
			}else if(D>0){
				TemporalBag->Insertar(Ficha("D", 1));
				D--;
				FichasTotales--;
			}else if(E>0){
				TemporalBag->Insertar(Ficha("E", 1));
				E--;
				FichasTotales--;
			}else if(F>0){
				TemporalBag->Insertar(Ficha("F", 4));
				F--;
				FichasTotales--;
			}else if(G>0){
				TemporalBag->Insertar(Ficha("G", 2));
				G--;
				FichasTotales--;
			}else if(H>0){
				TemporalBag->Insertar(Ficha("H", 4));
				H--;
				FichasTotales--;
			}else if(I>0){
				TemporalBag->Insertar(Ficha("I", 1));
				I--;
				FichasTotales--;
			}else if(J>0){
				TemporalBag->Insertar(Ficha("J", 8));
				J--;
				FichasTotales--;
			}else if(L>0){
				TemporalBag->Insertar(Ficha("L", 1));
				L--;
				FichasTotales--;
			}else if(M>0){
				TemporalBag->Insertar(Ficha("M", 3));
				M--;
				FichasTotales--;
			}else if(N>0){
				TemporalBag->Insertar(Ficha("N", 1));
				N--;
				FichasTotales--;
			}else if(Enie>0){
				TemporalBag->Insertar(Ficha("Ñ", 8));
				Enie--;
				FichasTotales--;
			}else if(O>0){
				TemporalBag->Insertar(Ficha("O", 1));
				O--;
			}else if(P>0){
				TemporalBag->Insertar(Ficha("P", 3));
				P--;
				FichasTotales--;
			}else if(Q>0){
				TemporalBag->Insertar(Ficha("Q", 5));
				Q--;
				FichasTotales--;
			}else if(R>0){
				TemporalBag->Insertar(Ficha("R", 1));
				R--;
				FichasTotales--;
			}else if(S>0){
				TemporalBag->Insertar(Ficha("S", 1));
				S--;
				FichasTotales--;
			}else if(T>0){
				TemporalBag->Insertar(Ficha("T", 1));
				T--;
				FichasTotales--;
			}else if(U>0){
				TemporalBag->Insertar(Ficha("U", 1));
				U--;
				FichasTotales--;
			}else if(V>0){
				TemporalBag->Insertar(Ficha("V", 4));
				V--;
				FichasTotales--;
			}else if(X>0){
				TemporalBag->Insertar(Ficha("X", 8));
				X--;
				FichasTotales--;
			}else if(Y>0){
				TemporalBag->Insertar(Ficha("Y", 4));
				Y--;
				FichasTotales--;
			}else if(Z>0){
				TemporalBag->Insertar(Ficha("Z", 10));
				Z--;
				FichasTotales--;
			}
		}
	for(int i =0;i<95;i++){
		srand((unsigned) time(0));
		int azar;
		azar= rand() % TemporalBag->GetSize();
		Nodo<Ficha> *TemporalFicha =  TemporalBag->SacarElemento(azar);
		Bolsa->Encolar(TemporalFicha->getValue());
	}
	}
void CargarArchivo(){
	diccionario = new ListaDobleCircular<string>;
	casillasEspeciales =  new ListaSimple<Casillas>();
	string nombre;
	cin>>nombre;
	try{
		ifstream ifs(nombre);
		json j3;
		ifs >> j3;
		cout << j3.at("dimension")<<endl;
		DimensionTablero =  j3.at("dimension").get<int>();
		for(int i = 0; i< j3.at("casillas").at("dobles").size();i++){
			cout <<  j3.at("casillas").at("dobles")[i].at("x").get<int>()<<",";
			cout <<  j3.at("casillas").at("dobles")[i].at("y").get<int>()<<endl;
			casillasEspeciales->Insertar( Casillas(j3.at("casillas").at("dobles")[i].at("x").get<int>(),
			j3.at("casillas").at("dobles")[i].at("y").get<int>(), Casillas::Tipo::Doble));
		}
		for(int i = 0; i< j3.at("casillas").at("triples").size();i++){
			cout <<  j3.at("casillas").at("triples")[i].at("x").get<int>()<<",";
			cout <<  j3.at("casillas").at("triples")[i].at("y").get<int>()<<endl;
			casillasEspeciales->Insertar(Casillas(j3.at("casillas").at("triples")[i].at("x").get<int>(),j3.at("casillas").at("triples")[i].at("y").get<int>(),
										Casillas::Tipo::Triple));
		}
		for(int i = 0; i< j3.at("diccionario").size();i++){
			cout <<  j3.at("diccionario")[i].at("palabra") <<endl;
			string palabra = j3.at("diccionario")[i].at("palabra");
			transform(palabra.begin(), palabra.end(), palabra.begin(), ::toupper);
			diccionario->Insertar(palabra);
		}

		Nodo<Casillas> *aux = casillasEspeciales->GetCabeza();
		for(int i=0;i<casillasEspeciales->GetSize();i++){
			cout << to_string(aux->getValue().getX()) <<","<<to_string(aux->getValue().getY())<<endl;
			Tablero->Insertar(Ficha(" ", 0),aux->getValue().getX(),aux->getValue().getY());
			aux=aux->getNext();
		}
		ImprimirDiccionario(diccionario);
		ArchivoEntrada=true;
	}catch(exception& e){
		cout<<"No se encontró el archivo."<<endl;
	}
	}
void CrearJugador(){
	string nombre;
	cout<<"Ingresa el nombre del  nuevo jugador: ";
	//cin.getline(nombre, 50);
	cin.ignore();
	std::getline(cin, nombre);
	if(arbol->Insertar(nombre)){
		cout<<endl<<"Ingreso Correcto del nuevo jugador\n";
		arbol->Imprimir();
	}else{
		cout<<endl<<"El jugador ya existe\n";
	}	
	}
void IniciarEDD(){
	Tablero = new MatrizDispersa<Ficha>(Ficha("-", 0));
	Bolsa = new Cola<Ficha>();
	casillasEspeciales= new ListaSimple<Casillas>();
	diccionario = new ListaDobleCircular<string>();
	}
void Menu(){
	NuevoTablero();
	cout <<"\nBienvenido a Scrabble++\n"<<endl<<"Escoje la opción que deseas ejecutar:\n1.Jugar"
												<<"\n2.Crear Jugador\n3.Cargar Archivo\n4.Score Borad\n"
												<<"5.Historial de Puntajes\n";
	int opcion;
	cin>>opcion;
	switch (opcion)
	{
	case 1:
		if(ArchivoEntrada){
			if(arbol->GetSize()>1){
				LlenarCola();
				EscogerJugadores();
				Menu();
			}else{
				cout<<"/-/-/-/-/-ERROR-/-/-/-/-/\nNo hay suficientes jugadores registados"<<endl;
				Menu();
			}
		}else{
			cout<<"/-/-/-/-/-ERROR-/-/-/-/-/\nNo se ha cargado ningún archivo de entrada."<<endl;
			Menu();
		}		
		break;
	case 2:
    	system("clear");
		CrearJugador();
		Menu();
		break;
	case 3:
    	system("clear");		
		CargarArchivo();
		Menu();
		break;
	case 4:
		system("clear");
		if(arbol->GetSize()>0){
			try{
			PrintBestScores();
			}catch(exception e){
				cout<<"No se pueden imprimir los puntajes";
			}
		}else{
			cout<<"No se han ingresado jugadores al registro."<<endl;
		}
		Menu();
		break;
	case 5:	
		system("clear");
		if(arbol->GetSize()>0){
			PrintPlayers();
			cout<<"Ingresa el número del jugador del cual deseas conocer el historial de puntajes"<<endl;
			int opcion;
			cin>>opcion;
			if(opcion<=JugadoresOrdenados->GetSize() && opcion>0){
				jugadores player = JugadoresOrdenados->ElementAt(opcion-1)->getValue();
				if(player.getScores()->GetSize()>0){
					string graph="digraph ReporteLD { graph [dpi=300]\n rankdir =\"LR\"; \n size=\"5\" \n node [shape = box]; label=\"Puntajes de "+player.getName()+"\"";
					string labels;
					cout<<"Los puntajes de "<<player.getName()<<" son:"<<endl;
					for(int i =0;i< player.getScores()->GetSize();i++){
						cout<<to_string(i+1)<<". "<<to_string(player.getScores()->ElementAt(i)->getValue())<<endl;
						if(i!=player.getScores()->GetSize()-1){
							graph+=to_string(i+1)+"->";
						}else{
							graph+=to_string(i+1)+";\n";
						}
						labels+=to_string(i+1)+"[label=\""+to_string(player.getScores()->ElementAt(i)->getValue())+"\"];\n";
					}
					ofstream graphFile;
					graphFile.open("txtFiles/Puntajes.txt");
					graphFile << graph+labels+"}";
					graphFile.close();
					std::string filePath="dot -Tpng txtFiles/Puntajes.txt -o Imgs/Puntajes.png";
					system(filePath.c_str());
				}else{
					cout<<player.getName()<<" no ha jugado ninguna partida."<<endl;
				}
			}else{
				cout<<to_string(opcion)<<" no es una opción válida."<<endl;
			}
		}else{
			cout<<"No se han ingresado jugadores al registro."<<endl;
		}
		Menu();
		break;
	default:
		cout <<"Ingresa una opción Válida\n";
		Menu();
		break;
	}	
	}



int main(int argc, char ** argv){
	/*
		MatrizDispersa<string> *matriz = new MatrizDispersa<string>("*");
		matriz->Insertar("buenas", 0, 3);
		matriz->Insertar("tardes", 0, 2);
		matriz->ImprimirMatriz();
		matriz->Insertar("amigos", 1, 2);
		matriz->Insertar("mios", 4, 2);
		matriz->Insertar("si", 0, 0);*/
		/*
		//---------------------------Codigo Para incerción de fichas----------------------------
		Tablero->Insertar(Ficha("H", 1), 0, 0);
		Tablero->Insertar(Ficha("O", 1), 0, 1);
		Tablero->Insertar(Ficha("L", 1), 0, 2);
		Tablero->Insertar(Ficha("A", 1), 0, 3);
		Tablero->Insertar(Ficha("S", 1), 1, 1);
		Tablero->Insertar(Ficha("O", 1), 2, 1);
		if(!Tablero->Insertar(Ficha("K", 1), 1, 0)){
			cout<<"No se ingresó la ficha, porque ya está ocupada esa casilla"<<endl;
			if(Tablero->SameNode(Ficha("K", 1), 1, 0)){
				cout<<"La ficha es la misma"<<endl;
			}else{
				cout<<"La ficha NO es la misma"<<endl;
			}
		}
		if(!Tablero->Insertar(Ficha("H", 1), 0, 0)){
			cout<<"No se ingresó la ficha, porque ya está ocupada esa casilla"<<endl;
			if(SameNode(Ficha("H", 1), 0, 0)){
				cout<<"La ficha es la misma"<<endl;
			}else{
				cout<<"La ficha NO es la misma"<<endl;
			}
		}
		if(Tablero->CasillaOcupada(Ficha("H", 1), 0, 4)){
			cout<<"La casilla está ocupada"<<endl;		
		}else{
			cout<<"casilla libre";
		}
		//---------------------------------------------------------------------------------------
		ofstream graphFile;
		string name = "Tablero";
		graphFile.open(""+name+".txt");
		graphFile << ImprimirTablero(Tablero);
		graphFile.close();
		std::string filePath="dot -Tpng "+name+".txt -o "+name+".png";
		system(filePath.c_str());
		*/
		/*//cout<<PrintW(matriz);
		matriz->ImprimirMatriz();
		ofstream graphFile;
		string name = "Tablero";
		graphFile.open(""+name+".txt");
		graphFile << PrintW(matriz);
		graphFile.close();
		std::string filePath="dot -Tpng "+name+".txt -o "+name+".png";
		system(filePath.c_str());
		ABB *arbol = new ABB();
		arbol->Insertar("Maria");
		arbol->Insertar("Alex");
		arbol->Insertar("Pamela");
		arbol->Insertar("Lucia");
		arbol->Insertar("Gabriela");
		arbol->Insertar("Karla");
		arbol->Imprimir(1);
		
			ABB *arbol = new ABB();
		while(true){
			string nombre;
			cin>>nombre;
			arbol->Insertar(nombre);
			arbol->Imprimir();
			cout<<arbol->GetSize();
		}
	

		ArchivoEntrada=false;
	arbol->Insertar("Mario Orellana");
	arbol->Insertar("Josue Orellana");
	arbol->Insertar("Walter Orellana");
	Nodo<jugadores> *temp = arbol->GetRoot();
	Nodo<jugadores> *temp1 = temp->getPrevious();
	Nodo<jugadores> *temp2 = temp->getNext();
	jugadores temporal = temp->getValue();//mario
	jugadores temporal1 = temp1->getValue();//josue
	jugadores temporal2 = temp2->getValue();//walter
	InsertarOrdenado(temporal, 0);
	InsertarOrdenado(temporal, 1);
	InsertarOrdenado(temporal, 3);
	InsertarOrdenado(temporal, 2);
	InsertarOrdenado(temporal1, 9);
	InsertarOrdenado(temporal1, 3);
	InsertarOrdenado(temporal1, 7);
	InsertarOrdenado(temporal2, 1);
	InsertarOrdenado(temporal2, 5);
	InsertarOrdenado(temporal2, 10);
	InsertarOrdenado(temporal2, 25);
	InsertarOrdenado(temporal2, 1);
	InsertarOrdenado(temporal2, 54);
	InsertarOrdenado(temporal2, 4);*/
	Menu();
	return 0;
	}


