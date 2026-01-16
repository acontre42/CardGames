#pragma once
#include <iostream>
#include <string>
using namespace std;

class Card
{
private:
	int color;
	int suit;
	int value;
public:
	enum COLORS { BLACK, RED };
	enum SUITS { CLUB, SPADE, DIAMOND, HEART };
	enum VALUES { A = 1, J = 11, Q = 12, K = 13 }; // In most cases, 'A' has value of 1.
	Card(int value, int suit);
	~Card();
	//Getters
	int getColor() const;
	int getSuit() const;
	int getValue() const;
	// Other Methods
	void display() const; // Display suit and value of card in easy-to-read format.
	static string valueToRank(int value); // Return rank for Card value as a string.
};

