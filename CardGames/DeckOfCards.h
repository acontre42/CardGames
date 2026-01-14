#pragma once
#include "Card.h"
#include <vector>
#include <algorithm>
#include <random>

class DeckOfCards
{
private:
	vector<Card> deck;
	int numDecks; // How many packs of 52 cards in deck.
public:	
	DeckOfCards(int numDecks = 0);
	//Getters
	int getNumDecks();
	size_t getSize(); // is size_t ok? 
	bool empty(); // Returns deck.empty().
	Card getCard(); // Returns a Card from the deck.
	// Returns Cards of specific value from deck.
	// Checks if deck has at least one card with that value. If yes, continuously swap it to back and pop it
	// off into a returnedCards vector until all cards of that value have been removed.
	vector<Card> getCardsOfValue(int value); 
	int getValueAt(int index); // Returns value of Card at a particular index in deck.
	void display(); // Display all cards in deck.
	void last(); // Display last card.
	void peek(); // Display front card.
	void populate(); // Populate the deck vector with the standard 52 cards in a deck of cards.
	void reshuffle(); // Shuffles deck if not empty.
	void addCard(Card card); // Add Card to deck.
	void addCard(int value, int symbol); // Create Card and add to deck. 
	void clear(); // Empty deck vector.
	bool contains(int value); // Returns true if value is found in deck.
	void sortByAscValue(); // Sorts deck vector in ascending order by value of Cards.
};

