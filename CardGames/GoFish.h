#pragma once
#include <iostream>
#include "DeckOfCards.h"
#include "Card.h"
#include <vector>
#include <string>
using namespace std;

class GoFish
{
private:
	const int FIRST_PLAYER = 0, NUM_DECKS = 1, MIN_PLAYERS = 2, MAX_PLAYERS = 4, BOOK = 4, NUM_STARS = 25;
	const int TWO_PLAYER_HAND = 7, THREE_PLAYER_HAND = 5;
	struct Player
	{
		string name;
		int numBooks;
		DeckOfCards hand;
		vector<Card> booksPile;
		//string notifications; // OPTIONAL TO DO
		Player(string name)
		{
			this->name = name;
			this->numBooks = 0;
			//this->notifications = ""; // OPTIONAL TO DO
		}
	};
	DeckOfCards deck{ NUM_DECKS };
	vector<Player> players;
	int numPlayers;
	int activePlayer;
	int handSize;
public:
	GoFish();
	void setup(); // Displays name of game, asks how many players, sets size of hand and sets up players vector.
	void run(); // Plays the game.
	void playRound(); // Deals cards, loops through turns until no more cards in play deck or players' hands.
	// Provide a buffer at beginning of turn, display hand and check for books. Draw cards if hand empty.
	// If targets available, ask Player who they want to target and what value they want. Handle moves accordingly.
	// If no targets available, draw card from deck or skip to next player if deck empty.
	void takeTurn(Player& player);
	void dealHands(); // Deals hands to all Players, then calls function to sort them in ascending order.
	void dealCard(Player& player); // Deals a card from play deck to a Player's hand.
	void displayAvailableTargets(); // Displays all Players (except activePlayer) who have cards in their hands in a list.
	void cleanup(); // Sets numPlayers and handSize back to -1, clears players vector, clear and repopulate deck.
	bool allHandsEmpty(); // If any player still has cards, return false. If all player hands empty, return true.
	bool targetsAvailable(); // Returns true if at least one other Player has cards in their hand.
	void checkForBooks(Player& player) const; // Checks Player's hand for books. Hands should be sorted any time changes are made so only 1 pass required.
	void decideWinner(); // Compares all players' numBooks scores and announces the winner(s).
	void displayAllBooks(); // Displays all players' bookPiles as well as their numBooks score.
	void displayCurrentHand(); // Displays hand of current activePlayer.
	void startTurnsBuffer() const; // Provides a buffer between Players' turns.
	void endTurnBuffer(); // Provide buffer at end of turn to keep screen from skipping forward.
	void displayTestingStuff(); // FOR TESTING PURPOSES: Displays players' hands, size of hands, booksPile, numBooks and playdeck size.
};

// HOW TO PLAY GO FISH
// Each player gets a hand with 5 cards (or 7 if only two players)
// Players ask for a value, if other player doesn't have, they draw a card
// If card drawn matches the value they asked for, they go again
// Player can only ask for value that exists in their hand
// Once a player has 4 of same value, they have a book
// Books are set aside and those cards can no longer be traded
// Player with the most books at end of game wins
// If Player runs out of cards, they draw 5 (or 7 if two players) cards from play deck
// If not enough cards left in play deck, take the remainder
// If there are no cards left in play deck, this Player sits out
// Game ends when no more cards in play deck or any of the Players' hands

// TO DO:
// make sure hands get sorted after every change to hand
// assign player at start of turn based on activePLayer instead of passing player by reference
// optional: take a turn to show players their cards after first dealing them?
// optional: consider adding a string to each Player that informs them of the moves taken against them between their turns
// optional: consider sorting books before displaying