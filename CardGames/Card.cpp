#include "Card.h"

Card::Card(int value, int suit) 
{
	this->value = value;
	this->suit = suit;
	if (suit >= CLUB && suit <= SPADE)
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

int Card::getColor() const
{
	return color;
}

int Card::getSuit() const
{
	return suit;
}

int Card::getValue() const
{
	return value;
}

void Card::display() const
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
	switch (suit)
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

string Card::valueToRank(int value)
{
	if (value < A || value > K)
	{
		return "Error: N/A";
	}

	switch (value)
	{
	case J:
		return "Jack";
	case Q:
		return "Queen";
	case K:
		return "King";
	case A:
		return "Ace";
	default:
		return to_string(value);
	}
}