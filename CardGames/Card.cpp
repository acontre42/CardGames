#include "Card.h"

Card::Card(int value, int symbol) 
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
}

Card::~Card()
{
	//cout << "DESTRUCTOR" << endl;
}

int Card::getColor()
{
	return color;
}

int Card::getSymbol()
{
	return symbol;
}

int Card::getValue()
{
	return value;
}

void Card::display()
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