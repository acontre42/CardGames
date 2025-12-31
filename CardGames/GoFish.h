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
	void takeTurn(Player& player)
	{
		startTurnsBuffer();
		displayCurrentHand();
		checkForBooks(player);

		bool goAgain = false;
		do
		{
			if (player.hand.empty() && deck.empty())
			{
				cout << "Sorry, " << player.name << ", there are no more cards in your hand or in the play deck! ";
				cout << "Skipping to the next player..." << endl;
				return;
			}
			
			if (player.hand.empty())
			{
				cout << "You're out of cards! Now drawing more cards from the play deck..." << endl;
				while (!deck.empty() && player.hand.getSize() < handSize)
				{
					dealCard(player);
				}
				cout << "You drew " << player.hand.getSize() << " cards from the deck. ";
				if (deck.empty())
				{
					cout << "Deck is now empty.";
				}
				cout << endl;
				player.hand.sortByAscValue();
				displayCurrentHand();
			}
			
			if (targetsAvailable())
			{
				int target = -1;
				int value = -1;
				bool validValue = false, validTarget = false;
				if (players.size() == 2)
				{
					target = (activePlayer + 1) % 2;
				}
				else
				{
					cout << "Which player would you like to target?" << endl;
					displayAvailableTargets();
					while (!validTarget)
					{
						cout << "Enter number of target: ";
						cin >> target;
						if (cin.fail())
						{
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							target = -1;
						}

						target = target - 1; // To match with index.
						if (target < 0 || target > players.size() - 1)
						{
							cout << "Please enter a valid target." << endl;
						}
						else if (target == activePlayer)
						{
							cout << "You can't target yourself! :(" << endl;
						}
						else if (players[target].hand.empty())
						{
							cout << players[target].name << " is not a valid target as they do not currently have any cards!" << endl;
						}
						else
						{
							validTarget = true;
						}
					}
				}

				cout << "What card value do you want to ask " << players.at(target).name << " for? Value must already exist in your hand." << endl;
				cout << "Note: Ace = 1, 2 - 10 = 2 - 10 , Jack = 11, Queen = 12, King = 13." << endl;
				while (!validValue)
				{
					cout << "Enter number of value you want:";
					cin >> value;
					if (cin.fail())
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						value = -1;
					}
					else if (value < Card::A || value > Card::K)
					{
						cout << "\nInvalid value." << endl;
					}
					else if (!player.hand.contains(value))
					{
						cout << "\nYou cannot ask for a value that does not already exist in your hand." << endl;
					}
					else
					{
						validValue = true;
					}
				}
				cout << "Checking " << players.at(target).name << "'s hand for " << value << "s..." << endl;
				if (players.at(target).hand.contains(value))
				{
					vector<Card> gimmeThose = players.at(target).hand.getCardsOfValue(value);
					players.at(target).hand.sortByAscValue(); // Re-sort target's hand after taking cards.
					cout << "You took " << gimmeThose.size() << " card(s) of value " << value << " from " << players.at(target).name << "." << endl;
					int gimmeThoseSize = gimmeThose.size();
					for (int i = 0; i < gimmeThoseSize; i++)
					{
						player.hand.addCard(gimmeThose.back());
						gimmeThose.pop_back();
					}
					goAgain = true;
				}
				else
				{
					cout << "GO FISH!" << endl;
					if (deck.getSize() > 0) // If play deck is not empty, draw card from it. Else, end turn.
					{
						player.hand.addCard(deck.getCard());
						cout << "You drew a ";
						player.hand.last();
						cout << "." << endl;
						if (player.hand.getValueAt(player.hand.getSize() - 1) == value) // Go again if value drawn was value asked for.
						{
							goAgain = true;
						}
						else
						{
							goAgain = false;
						}
					}
					else
					{
						cout << "No more cards in play deck." << endl;
						goAgain = false;
					}
				}
				player.hand.sortByAscValue();
				displayCurrentHand();
				checkForBooks(player);
			}
			else
			{
				cout << "There are currently no suitable players to target.\n";
				if (deck.empty())
				{
					cout << "Since the play deck is empty, now skipping to next player's turn...\n";
				}
				else
				{
					cout << "Drawing a card from the play deck..." << endl;
					dealCard(player);
					cout << "You drew a ";
					player.hand.last();
					cout << "." << endl;
					player.hand.sortByAscValue();
					displayCurrentHand();
					checkForBooks(player);
				}
				goAgain = false;
			}
			//TESTING STUFF DELETE WHEN DONE
			//displayTestingStuff();
		} while (goAgain);

		endTurnBuffer();
	}
	// Deals hands to all Players, then calls function to sort them in ascending order.
	void dealHands()
	{
		for (int i = 0; i < handSize; i++)
		{
			for (int i = 0; i < players.size(); i++)
			{
				dealCard(players.at(i));
			}
			deck.reshuffle();
		}
		for (Player& p : players)
		{
			p.hand.sortByAscValue();
		}

		// TESTING STUFF
		//displayTestingStuff();
	}
	// Deals a card from play deck to a Player's hand.
	void dealCard(Player& player)
	{
		player.hand.addCard(deck.getCard());
	}
	// Displays all Players (except activePlayer) who have cards in their hands in a list.
	void displayAvailableTargets()
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (i != activePlayer && !players[i].hand.empty())
			{
				cout << players.at(i).name << endl;
			}
		}
	}
	// Sets numPlayers and handSize back to -1, clears players vector, clear and repopulate deck.
	void cleanup()
	{
		players.clear();
		numPlayers = -1;
		activePlayer = FIRST_PLAYER;
		handSize = -1;
		deck.clear();
		deck.populate();
	}
	// If any player still has cards, return false. If all player hands empty, return true.
	bool allHandsEmpty()
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (!(players[i].hand.empty()))
			{
				return false;
			}
		}
		return true;
	}
	// Returns true if at least one other Player has cards in their hand.
	bool targetsAvailable()
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (i != activePlayer && !(players[i].hand.empty()))
			{
				return true;
			}
		}
		return false;
	}
	// Checks Player's hand for books (4 of any value). If found, move to book vector and increment numBooks.
	// If player has less than 4 cards, just return.
	void checkForBooks(Player& player)
	{
		if (player.hand.getSize() < BOOK)
		{
			return;
		}
		int value = player.hand.getValueAt(0);
		int handSize = player.hand.getSize();
		int nextValue = -1;
		int count = 0;
		vector<int> bookValues;

		// Checks for 4 of any value.
		for (int i = 1; i < handSize; i++)
		{
			nextValue = player.hand.getValueAt(i);
			if (nextValue == value)
			{
				count++;
				if (count == 3) // First value counts as the 1st instance out of potential 4 so only need to count to 3.
				{
					bookValues.push_back(value);
					count = 0;
				}
			}
			else
			{
				value = nextValue;
				count = 0;
			}
		}
		// If books found, inform Player, move Cards to booksPile, increment numBooks.
		int bookValuesSize = bookValues.size();
		if (bookValuesSize > 0)
		{
			for (int i = 0; i < bookValuesSize; i++)
			{
				int value = bookValues.at(i);
				vector<Card> book = player.hand.getCardsOfValue(value);
				int bookSize = book.size();
				for (int j = 0; j < bookSize; j++)
				{
					player.booksPile.push_back(book.back());
					book.pop_back();
				}
				player.numBooks++;
			}
			cout << "\nYou have " << bookValues.size() << " book(s) in your hand! You now have ";
			cout << player.numBooks << " point(s)! ";
			cout << "After removing all books from hand, your hand now looks like this:" << endl;
			player.hand.sortByAscValue();
			player.hand.display();
		}
	}
	// Compares all players' numBooks scores and announces the winner(s).
	void decideWinner()
	{
		int highestScore = -1;
		vector<int> winnerIndices; // In the event of a tie.
		for (int i = 0; i < players.size(); i++)
		{
			if (players.at(i).numBooks > highestScore)
			{
				highestScore = players.at(i).numBooks;
				winnerIndices.clear();
				winnerIndices.push_back(i);
			}
			else if (players.at(i).numBooks == highestScore)
			{
				winnerIndices.push_back(i);
			}
		}

		if (winnerIndices.size() == 1)
		{
			cout << "The winner is " << players.at(winnerIndices[0]).name << "!" << endl;
		}
		else
		{
			cout << "There is a TIE between the following players: " << endl;
			for (auto i : winnerIndices)
			{
				cout << players[i].name << endl;
			}
		}
	}
	// Displays all players' bookPiles as well as their numBooks score.
	void displayAllBooks()
	{
		cout << "Displaying all players' books..." << endl;
		for (auto p : players)
		{
			cout << p.name << "'s book pile: ";
			int cardIndex = 0;
			for (auto b : p.booksPile)
			{
				b.display();
				if (cardIndex < p.booksPile.size() - 1)
				{
					cout << ", ";
				}
				cardIndex++;
			}
			cout << "\nTotal books: " << p.numBooks << endl << endl;
		}
	}
	// Displays hand of current activePlayer.
	void displayCurrentHand()
	{
		cout << "Your current hand:" << endl;
		players[activePlayer].hand.display();
	}
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