//System Needs
#include <fstream>
#include <typeinfo>
#include <ncurses.h>
#include <random>
#include "json.hpp"
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
using json = nlohmann::json;
//Estrucutras y Variables Globales
ABB *arbol =  new ABB();
ListaSimple<Casillas> *casillasEspeciales = new ListaSimple<Casillas>();
ListaSimple<jugadores> *JugadoresOrdenados;
ListaDobleCircular<string> *diccionario = new ListaDobleCircular<string>();
Cola<Ficha> *Bolsa = new Cola<Ficha>();
int DimensionTablero;
jugadores jugador1, jugador2;
//Métodos de acceso a EDD
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
		 cout<<to_string(i)<<". "<< JugadoresOrdenados->ElementAt(i)->getValue().getName()<<endl;
	 }
	}

//Métodos del juego
void Jugar(){

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
	}else{
		jugador1=JugadoresOrdenados->ElementAt(player1)->getValue();
		jugador2=JugadoresOrdenados->ElementAt(player2)->getValue();
		cout<<"jugador1: "<<jugador1.getName()<<" jugador2: "<<jugador2.getName()<<endl;
		Jugar();
	}
	}
void LlenarCola(){
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
	string nombre;
	cin>>nombre;
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
		diccionario->Insertar(j3.at("diccionario")[i].at("palabra"));
	}

	Nodo<Casillas> *aux = casillasEspeciales->GetCabeza();
	for(int i=0;i<casillasEspeciales->GetSize();i++){
		cout << to_string(aux->getValue().getX()) <<","<<to_string(aux->getValue().getY())<<endl;
		aux=aux->getNext();
	}
	}
void CrearJugador(){
	string nombre;
	cout<<"Ingresa el nombre del  nuevo jugador: ";
	cin>>nombre;
	if(arbol->Insertar(nombre)){
		cout<<"Ingreso Correcto del nuevo jugador\n";
		arbol->Imprimir();
	}else{
		cout<<"El jugador ya existe\n";
	}
	char c = getch();	
	}
void Menu(){
	cout <<"\nBienvenido a Scrabble++\n"<<endl<<"Escoje la opción que deseas ejecutar:\n1.Jugar\n2.Crear Jugador\n3.Cargar Archivo\n";
	int opcion;
	cin>>opcion;
	switch (opcion)
	{
	case 1:
		LlenarCola();
		EscogerJugadores();
		Menu();
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
	default:
		cout <<"Ingresa una opción Válida\n";
		Menu();
		break;
	}	
	}



int main(int argc, char ** argv)
	{
	/*
    MatrizDispersa<string> *matriz = new MatrizDispersa<string>("putas");
	matriz->Insertar("buenas", 0, 3);
	matriz->Insertar("buenas", 0, 2);
	matriz->Insertar("buenas", 1, 2);
	matriz->Insertar("buenas", 4, 2);
	matriz->Insertar("buenas", 0, 0);
	//cout<<PrintW(matriz);
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
	*/
		Menu();

	return 0;
	}


