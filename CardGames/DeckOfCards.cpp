#include "DeckOfCards.h"

DeckOfCards::DeckOfCards(int numDecks)
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

int DeckOfCards::getNumDecks()
{
	return numDecks;
}

size_t DeckOfCards::getSize()
{
	return deck.size();
}

bool DeckOfCards::empty()
{
	return (deck.empty() ? true : false);
}

Card DeckOfCards::getCard()
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

vector<Card> DeckOfCards::getCardsOfValue(int value)
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

int DeckOfCards::getValueAt(int index)
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

void DeckOfCards::display()
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

void DeckOfCards::last()
{
	Card card = deck.back();
	card.display();
}

void DeckOfCards::peek()
{
	Card card = deck.front();
	card.display();
	cout << endl;
}

void DeckOfCards::populate()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j <= 13; j++)
		{
			deck.emplace_back(j, i);
		}
	}
}

void DeckOfCards::reshuffle()
{
	if (numDecks > 0)
	{
		shuffle(deck.begin(), deck.end(), random_device());
	}
}

void DeckOfCards::addCard(Card card)
{
	deck.push_back(card);
}

void DeckOfCards::addCard(int value, int symbol)
{
	deck.emplace_back(value, symbol);
}

void DeckOfCards::clear()
{
	if (deck.size() > 0)
	{
		deck.clear();
	}
}

bool DeckOfCards::contains(int value)
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

void DeckOfCards::sortByAscValue()
{
	sort(deck.begin(), deck.end(), [](Card a, Card b) {return a.getValue() < b.getValue();});
}