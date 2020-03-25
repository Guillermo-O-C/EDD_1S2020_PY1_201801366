
class Casillas
{    
public:
    enum Tipo {
            Doble, Triple
    };

private:    
    Tipo tipo;
    int x;
    int y;
    
public:
    Casillas();
    Casillas(int x, int y, Casillas::Tipo tipo);
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
};
Casillas::Casillas(){

};
Casillas::Casillas(int x, int y, Casillas::Tipo tipo)
{
    this->x=x;
    this->y=y;
    this->tipo=tipo;
}
	 void Casillas::setX(int x){
				this->x = x;
			}
	 void Casillas::setY(int y){
				this->y = y;
			}
	int Casillas::getX(){
				return this->x;
			}
	int Casillas::getY(){
				return this->y;
			}