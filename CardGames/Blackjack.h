#pragma once
#include <iostream>
#include <vector>
#include "DeckOfCards.h"
#include "Card.h"
#include <string>
using namespace std;

const int BLACKJACK = 21, FIRST_PLAYER = 0, MIN_PLAYERS = 1, MAX_PLAYERS = 4, NUM_DECKS = 1;
const double STARTER_BANK = 100, MIN_BET = 1;

struct Player
{
	DeckOfCards deck;
	string name;
	double bank;
	int bet;
	bool stillIn;
	Player(string name)
	{
		this->name = name;
		this->bank = STARTER_BANK;
		this->bet = 0;
		this->stillIn = true;
	}
	int calcTotal() // Calculate the total value of a Player's hand.
	{
		int total = 0;
		int numAs = 0;
		for (int i = 0; i < deck.getSize(); i++)
		{
			int value = deck.getValueAt(i); 
			if (value == 1) // A's = 1 or 11 depending on whether or not the higher value would exceed a value of 21.
			{
				numAs++; // Since A's can have multiple values, just count them for now.
			}
			else if (value > 10)
			{
				total = total + 10; // Face cards = 10.
			}
			else
			{
				total = total + value;
			}
		}
		// Calculate values of Aces
		while (numAs != 0)
		{
			if (total + 11 <= 21) // Count Ace as 11 when it does not exceed 21.
			{
				total = total + 11;
			}
			else
			{
				total = total + 1;
			}
			numAs--;
		}
		return total;
	}
};

class Blackjack
{
private:
	DeckOfCards playDeck{ NUM_DECKS };
	Player dealer{ "Dealer" };
	vector<Player> players;
	int numPlayers;
	void setup(); // Welcomes player, displays some rules, takes input from player to setup game.
	void getBetsFromPlayers(); // Loops through players vector and calls the placeBet() function for each Player.
	void placeBet(Player& player); // Gets bets from players and validates amounts.
	void clearAllBets(); // Sets bet for all Players to 0. To be called after every round.
	void resetPlayersStatuses(); // Resets all players' stillIn variable back to true.
	void returnAllCards(); // Return all players' cards to the playDeck.
	// Distributes cards, displays cards/totals, loops through players vector to get
	// their moves, display dealer cards/move, check results, return cards, reset statuses.
	void playARound();
	void distributeTwoCards(Player& player); // Distribute 2 cards to a Player's deck.
	void getMovesFromPlayers(); // Loops through players vector and calls the getMove() function for each Player.
	void getMove(Player& player); // Gets input from Player to decide their move (Hit, Stay, Double, Surrender).
	// Displays dealer's cards and checks for Blackjack. If total <= 17, dealer hits. If not, stays.
	// If dealer busts, set dealer's stillIn variable to false.
	void dealersTurn();
	void checkResults(); // Compares the totals of the players who are stillIn to the Dealer's total. Adjusts banks.
	bool playersIn(); // Checks the players vector to see if any players are stillIn the round.
	void summary(); // Displays the amount of money lost/gained by each player throughout the game.
	void screenBuffer(); // Buffer to prevent screen from skipping forward.
public:
	Blackjack();
	void run(); // Plays the actual game.
};

// TO DO LIST:
// optional: Add a 1 second sleep between players' hands when displaying them? when displaying erros? (screenBuffer function)
// ERROR in simulation: on second round of two player game, second player doubled their bet and got a 21 but
// game did not update their bank or indicate that they won at end of round? only first player was updated
// what if player has $0 heading into next round? need to implement something for this

// BLACKJACK RULES:
// Natural Blackjack: When an ace and any 10-point card are the initial two cards dealt. Win an extra 50% of bet.
// Unnatural Blackjack A.K.A "21": Three or more cards totaling 21. Player automatically stays.