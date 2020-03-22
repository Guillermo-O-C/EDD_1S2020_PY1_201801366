#include "EDD/ListaCircular_t.h"
#include "EDD/ListaDoble_t.h"
#include "EDD/ListaSimple_t.h"
#include "EDD/Pila_t.h"
#include "EDD/MatrizDispersa_t.h"
#include <fstream>

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
	int ENodes=0;
	string content ="digraph G { \n node[shape=box]\n ";
	string nivelador;
	string LastPosition;
	int Xcolumns=0;
	int YRows=0;
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
				Xcolumns++;
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
					content+="X"+to_string(temp->getX())+" -> F"+to_string(temp->getDown()->getX())+"C"+to_string(temp->getDown()->getY());
					content+="\n";
				}
            }else if(temp->getX()==-1){//cabeza filas
				YRows++;
				ENodes++;
				rankedLevel+="Y"+to_string(temp->getY());
				nivelador+="Y"+to_string(temp->getY())+"[group=1];\n";
				//content+= "pinting row head\n";
             //  cout<< temp->getY() <<"\n" ;
                //imprimir derecha
				if(temp->getNext()!=NULL){
					content+="Y"+to_string(temp->getY())+" -> F"+ to_string(temp->getNext()->getX())+"C"+to_string(temp->getNext()->getX());
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
				LastPosition="F"+to_string(temp->getX())+"C"+to_string(temp->getY());
				rankedLevel+=";F"+to_string(temp->getX())+"C"+to_string(temp->getY());
				nivelador+="F"+to_string(temp->getX())+"C"+to_string(temp->getY())+"[group="+to_string(temp->getX()+1)+"];\n";
				//cout<<"contiene "+ temp->getValue()+"en ("<<to_string(temp->getX())+","+to_string(temp->getY())<<")";
             //  cout<< temp->getValue() <<" ";
			    //imprimir izquierda
				if(temp->getPrevious()!=NULL){
					if(temp->getPrevious()->getX()==-1){
						content+="F"+to_string(temp->getX())+"C"+to_string(temp->getY())+" -> Y"+to_string(temp->getPrevious()->getY());
						content+="\n";
					}else{
						content+="F"+to_string(temp->getX())+"C"+to_string(temp->getY())+" -> F"+to_string(temp->getPrevious()->getX())+"C"+to_string(temp->getPrevious()->getY());
						content+="\n";
					}
				}
                //imprimir derecha 
				if(temp->getNext()!=NULL){
					content+="F"+to_string(temp->getX())+"C"+to_string(temp->getY())+" -> F"+to_string(temp->getNext()->getX())+"C"+to_string(temp->getNext()->getY());
					content+="\n";
				}
			    //imprimir arriba
				if(temp->getUp()!=NULL){
					if(temp->getUp()->getY()==-1){
						content+="F"+to_string(temp->getX())+"C"+to_string(temp->getY())+" -> X"+to_string(temp->getUp()->getX());
						content+="\n";
					}else{
						content+="F"+to_string(temp->getX())+"C"+to_string(temp->getY())+" -> F"+to_string(temp->getUp()->getX())+"C"+to_string(temp->getUp()->getY());
						content+="\n";
					}
				}
				//imprimir abajo
				if(temp->getDown()!=NULL){
					content+="F"+to_string(temp->getX())+"C"+to_string(temp->getY())+" -> F"+to_string(temp->getDown()->getX())+"C"+to_string(temp->getDown()->getY());
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
int main(int argc, char ** argv)
{
    MatrizDispersa<string> *matriz = new MatrizDispersa<string>("putas");
	matriz->Insertar("buenas", 0, 0);
	matriz->Insertar("noches", 0, 1);
	//cout<<PrintW(matriz);
	matriz->ImprimirMatriz();
	ofstream graphFile;
    string name = "Tablero";
    graphFile.open(""+name+".txt");
    graphFile << PrintW(matriz);
    graphFile.close();
    std::string filePath="dot -Tpng "+name+".txt -o "+name+".png";
    system(filePath.c_str());
	return 0;
}
