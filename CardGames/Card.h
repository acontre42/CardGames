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
	Card(int value, int symbol)
	{
		this->value = value;
		this->symbol = symbol;
		if (symbol >= CLUB && symbol <= SPADE)
		{
			color = BLACK;
		}
		else
		{
			color = RED;
		}
		//cout << "CONSTRUCTOR" << endl;
	}
	~Card()
	{
		//cout << "DESTRUCTOR" << endl;
	}
	//Getters
	int getColor()
	{
		return color;
	}
	int getSymbol()
	{
		return symbol;
	}
	int getValue()
	{
		return value;
	}
	// Other Methods
	// Display symbol and value of card in easy-to-read format.
	void display()
	{
		switch (value)
		{
		case J:
			cout << "Jack";
			break;
		case Q:
			cout << "Queen";
			break;
		case K:
			cout << "King";
			break;
		case A:
			cout << "Ace";
			break;
		default:
			cout << value;
		}
		cout << " of ";
		switch (symbol)
		{
		case CLUB:
			cout << "Clubs";
			break;
		case SPADE:
			cout << "Spades";
			break;
		case DIAMOND:
			cout << "Diamonds";
			break;
		case HEART:
			cout << "Hearts";
			break;
		}
	}
};

