#include <iostream>
#pragma once
using namespace std;
class Ficha
{
private:
    string letra;
    int valor;    
public:
    Ficha();
    Ficha(char letra, int valor);
    Ficha(string letraEspecial, int valor);
    string getChar();
    int getValor();
};
Ficha::Ficha()
{
}
Ficha::Ficha(string letra, int valor)
{
    this->letra=letra;
    this->valor=valor;
}
string Ficha::getChar(){
    return this->letra;
}
int Ficha::getValor(){
    return this->valor;
}