#include <iostream>
#pragma once
using namespace std;

class jugadores
{
private:
    string name;
    int ID;

public:
    jugadores()
	{
		this->name="";
	}
    jugadores(string name)
	{
		this->name=name;
	}
    string getName()
	{
		return this->name;
	}
    void setName(string name)
	{
		this->name = name;
	}
    int getID()
	{
		return this->ID;
	}
    void setID(int ID1)
	{
		this->ID = ID1;
	}
};
