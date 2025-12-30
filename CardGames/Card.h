#pragma once
#include <iostream>
using namespace std;

class Card
{
private:
	int color;
	int symbol;
	int value;
public:
	enum COLORS { BLACK, RED };
	enum SYMBOLS { CLUB, SPADE, DIAMOND, HEART };
	enum VALUES { A = 1, J = 11, Q = 12, K = 13 }; // In most cases, 'A' has value of 1.
	Card(int value, int symbol);
	~Card();
	//Getters
	int getColor();
	int getSymbol();
	int getValue();
	// Other Methods
	// Display symbol and value of card in easy-to-read format.
	void display();
};

