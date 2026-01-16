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
	this->rank = valueToRank(value);
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
	cout << rank << " of ";
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