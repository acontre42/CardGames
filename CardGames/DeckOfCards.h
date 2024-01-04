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
	DeckOfCards(int numDecks = 0)
	{
		this->numDecks = numDecks;
		if (numDecks > 0)
		{
			for (int i = 1; i <= numDecks; i++)
			{
				populate();
			}
			shuffle(deck.begin(), deck.end(), random_device());
		}
	}
	//Getters
	int getNumDecks()
	{
		return numDecks;
	}
	size_t getSize() // is size_t ok?
	{
		return deck.size();
	}
	// Returns true if deck.empty() returns true.
	bool empty()
	{
		if (deck.empty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// Returns a Card from the deck.
	Card getCard()
	{
		if (!deck.empty())
		{
			Card card = deck.back();
			deck.pop_back();
			return card;
		}
		else
		{
			cout << "Cannot get card from empty deck." << endl;
			exit(1);
		}
	}
	// Returns Cards of specific value from deck.
	// Checks if deck has at least one card with that value. If yes, continuously swap it to back and pop it
	// off into a returnedCards vector until all cards of that value have been removed.
	vector<Card> getCardsOfValue(int value)
	{
		vector<Card> returnedCards;
		if (contains(value))
		{
			while (contains(value))
			{
				for (int i = 0; i < deck.size(); i++)
				{
					if (deck[i].getValue() == value)
					{
						swap(deck[i], deck.back());
						break;
					}
				}
				returnedCards.push_back(deck.back());
				deck.pop_back();
			}
		}
		else
		{
			cout << "No cards in deck contain desired value." << endl;
		}
		return returnedCards;
	}
	// Returns value of Card at a particular index in deck.
	int getValueAt(int index)
	{
		if (index > deck.size())
		{
			cout << "ERROR: index greater than size of deck." << endl;
			exit(1);
		}
		else
		{
			return deck.at(index).getValue();
		}
	}
	// Display all cards in deck.
	void display()
	{
		if (deck.empty())
		{
			cout << "N/A" << endl;
		}
		else
		{
			for (auto d : deck)
			{
				d.display();
				cout << endl;
			}
		}
	}
	// Display last card.
	void last()
	{
		Card card = deck.back();
		card.display();
	}
	// Display top card.
	void peek()
	{
		Card card = deck.front();
		card.display();
		cout << endl;
	}
	// Populate the deck vector with the standard 52 cards in a deck of cards.
	void populate()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j <= 13; j++)
			{
				deck.emplace_back(j, i);
			}
		}
	}
	// Shuffles deck if not empty.
	void reshuffle()
	{
		if (numDecks > 0)
		{
			shuffle(deck.begin(), deck.end(), random_device());
		}
	}
	// Add Card to deck.
	void addCard(Card card)
	{
		deck.push_back(card);
	}
	void addCard(int value, int symbol)
	{
		deck.emplace_back(value, symbol);
	}
	// Empty deck vector.
	void clear()
	{
		if (deck.size() > 0)
		{
			deck.clear();
		}
	}
	// Returns true if value is found in deck.
	bool contains(int value)
	{
		for (auto& card : deck)
		{
			if (card.getValue() == value)
			{
				return true;
			}
		}
		return false;
	}
	// Sorts deck vector in ascending order by value of Cards.
	void sortByAscValue()
	{
		sort(deck.begin(), deck.end(), [](Card a, Card b) {return a.getValue() < b.getValue();});
	}
};

