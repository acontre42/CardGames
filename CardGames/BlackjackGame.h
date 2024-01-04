#pragma once
#include <iostream>
#include <vector>
#include "DeckOfCards.h"
#include "Card.h"
using namespace std;

const int BLACKJACK = 21, FIRST_PLAYER = 0, MIN_PLAYERS = 1, MAX_PLAYERS = 4, NUM_DECKS = 1;
const double STARTER_BANK = 100, MIN_BET = 1;

struct Player
{
	DeckOfCards deck;
	double bank;
	double bet;
	bool stillIn;
	Player()
	{
		bank = STARTER_BANK;
		bet = 0;
		stillIn = true;
	}
	// Calculate the total value of a Player's hand. Face cards = 10. A's = 1 or 11 depending on
	// whether or not the higher value would exceed a value of 21.
	int calcTotal()
	{
		int total = 0;
		int numAs = 0;
		for (int i = 0; i < deck.getSize(); i++)
		{
			int value = deck.getValueAt(i);
			if (value == 1)
			{
				numAs++;
			}
			else if (value > 10)
			{
				total = total + 10;
			}
			else
			{
				total = total + value;
			}
		}
		// This Blackjack game will count an Ace as an 11 when it gives the Player a Blackjack
		// or when doing so does not exceed 21.
		while (numAs != 0)
		{
			if (total + 11 <= 21)
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

class BlackjackGame
{
private:
	DeckOfCards playDeck{ NUM_DECKS };
	Player dealer;
	vector<Player> players;
	int activePlayer;
	int numPlayers;

public:
	BlackjackGame()
	{
		activePlayer = FIRST_PLAYER;
		numPlayers = -1;
	}
	// Welcomes player, displays some rules, takes input from player to setup game.
	void setup()
	{
		cout << "*****BLACKJACK*****" << endl;
		cout << "Note: Blackjack = instant win (no push). \n\tBlackjacks reward player with an extra 50% of their bet." << endl;
		cout << "\tDealer hits on 17. \n\tSplitting is not allowed." << endl;

		while (numPlayers < MIN_PLAYERS || numPlayers > MAX_PLAYERS)
		{
			cout << "Enter number of players(1-4): ";
			cin >> numPlayers;
			if (cin.fail())
			{
				cout << "\nMust be a number.\n";
				cin.clear();
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		for (int i = 0; i < numPlayers; i++)
		{
			players.emplace_back(); // Adds Players to players vector.
		}
		cout << endl;
	}
	// Plays the actual game.
	void run()
	{
		setup();
		char choice = ' ';
		do
		{
			getBetsFromPlayers();
			playARound();
			cout << "Would you like to play again? Enter 'Q' to quit or any key to play again." << endl;
			cin >> choice;
			choice = tolower(choice);
			cout << endl;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} while (choice != 'q');
		summary();
		cleanup();
	}
	// Loops through players vector and calls the placeBet() function for each Player. Calls
	// resetActivePlayer() function when done.
	void getBetsFromPlayers()
	{
		for (int i = 0; i < players.size(); i++)
		{
			activePlayer = i;
			placeBet(players.at(i));
		}
		cout << "All bets placed." << endl << endl;
		resetActivePlayer();
	}
	// Prompts user to enter how much they want to bet. Checks to make sure player's bet is within
	// appropriate range.
	void placeBet(Player &player)
	{
		double betAmount = 0;
		do
		{
			cout << "Player " << activePlayer + 1 << "'s bank account: $" << player.bank << endl;
			cout << "How much would you like to bet? ";
			cin >> betAmount;
			if (!cin.fail())
			{
				if (betAmount < MIN_BET)
				{
					cout << endl << "Bets must be at least $" << MIN_BET << "." << endl;
				}
				if (betAmount > player.bank)
				{
					cout << endl << "You cannot bet more money than you have in the bank." << endl;
				}
			}
			else
			{
				cout << "Please enter a valid number." << endl;
				cin.clear();
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << endl;
		} while (betAmount < MIN_BET || betAmount > player.bank);
		player.bet = betAmount;
	}
	// Sets bet for all Players to 0. To be called after every round.
	void clearAllBets()
	{
		for (auto &p : players)
		{
			p.bet = 0;
		}
	}
	// Resets activePlayer back to 0 (FIRST_PLAYER) if activePlayer is not currently set to 0.
	void resetActivePlayer()
	{
		if (activePlayer != FIRST_PLAYER)
		{
			activePlayer = FIRST_PLAYER;
		}
	}
	// Resets all players' stillIn variable back to true.
	void resetPlayersStatuses()
	{
		dealer.stillIn = true;
		for (auto &p : players)
		{
			p.stillIn = true;
		}
	}
	// Return all players' cards to the playDeck.
	void returnAllCards()
	{
		while (dealer.deck.getSize() != 0)
		{
			playDeck.addCard(dealer.deck.getCard());
		}
		for (int i = 0; i < numPlayers; i++)
		{
			while (players.at(i).deck.getSize() != 0)
			{
				playDeck.addCard(players.at(i).deck.getCard());
			}
		}
	}
	// Distributes cards, displays cards/totals, loops through players vector to get
	// their moves, display dealer cards/move, check results, return cards, reset statuses.
	void playARound()
	{
		//Distribute cards to Dealer and Players.
		distributeTwoCards(dealer);
		for (int i = 0; i < players.size(); i++)
		{
			distributeTwoCards(players.at(i));
		}
		cout << "Dealer's cards: " << endl << "One card face down." << endl;
		dealer.deck.peek();
		cout << endl;
		for (int i = 0; i < players.size(); i++)
		{
			cout << "Player " << i + 1 << "'s cards: " << endl;
			players.at(i).deck.display();
			cout << "Total: " << players.at(i).calcTotal();
			if (players.at(i).calcTotal() == BLACKJACK)
			{
				cout << " Blackjack!";
			}
			cout << endl << endl;
		}
		// Get moves from players.
		getMovesFromPlayers();
		// If there are still players in the round, Dealer takes their turn and player results
		// are checked. If no players remain in round, display dealer's hidden card.
		if (playersIn())
		{
			dealersTurn();
			checkResults();
		}
		else
		{
			cout << "Dealer's cards were:" << endl;
			dealer.deck.display();
			cout << endl;
		}
		// Clean up
		returnAllCards();
		resetPlayersStatuses();
		clearAllBets();
	}
	// If playDeck has at least 2 cards, distribute 2 to a Player's deck.
	void distributeTwoCards(Player& player)
	{
		if (playDeck.getSize() >= 2)
		{
			player.deck.addCard(playDeck.getCard());
			player.deck.addCard(playDeck.getCard());
		}
		else
		{
			cout << "Not enough cards in play deck to play." << endl;
			exit(1);
		}
	}
	// Loops through players vector and calls the getMove() function for each Player. Calls
	// resetActivePlayer() function when done.
	void getMovesFromPlayers()
	{
		for (int i = 0; i < players.size(); i++)
		{
			activePlayer = i;
			getMove(players.at(i));
		}
		resetActivePlayer();
	}
	// Gets input from Player to decide their move (Hit, Stay, Double, Surrender).
	void getMove(Player& player)
	{
		char choice = ' ';
		bool doneWithRound = false;
		cout << "***Player " << activePlayer + 1 << "'s Move***" << endl;
		
		if (player.calcTotal() == BLACKJACK)
		{
			doneWithRound = true;
			cout << "You have a Blackjack and have won an extra 50% of your bet." << endl;
			player.bank = player.bank + (player.bet * 1.5);
			cout << "You now have $" << player.bank << " in the bank." << endl << endl;
			player.stillIn = false;
		}
		
		bool canDouble = false, validChoice = false;
		if (player.bet * 2 < player.bank)
		{
			canDouble = true;
		}
		while (!doneWithRound)
		{
			do
			{
				cout << "Your cards: " << endl;
				player.deck.display();
				cout << "Your current total: " << player.calcTotal() << endl;
				cout << "Possible moves: \n A. Hit \n B. Stay \n C. Surrender \n";
				if (canDouble)
				{
					cout << " D. Double \n";
				}
				cout << "What would you like to do? ";
				cin >> choice;
				choice = tolower(choice);
				if (choice == 'a' || choice == 'b' || choice == 'c' || (choice == 'd' && canDouble))
				{
					validChoice = true;
				}
				else
				{
					cout << "Please enter a valid choice." << endl;
				}
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			} while (!validChoice);

			switch (choice)
			{
				// Hit. Player draws a card. Player's turn does not end until they either get a
				// 21, bust, stay, surrender, or double.
			case 'a':
				player.deck.addCard(playDeck.getCard());
				cout << "You have drawn a ";
				player.deck.last();
				cout << " which brings your total to " << player.calcTotal() << ".";
				if (player.calcTotal() == BLACKJACK)
				{
					cout << " Because you have a total of 21, you must now stay.";
					doneWithRound = true;
				}
				if (player.calcTotal() > BLACKJACK)
				{
					cout << " Bust!" << endl;
					player.stillIn = false;
					player.bank = player.bank - player.bet;
					cout << "You now have $" << player.bank << " in the bank.";
					doneWithRound = true;
				}
				cout << endl << endl;
				break;
				// Stay. Player is still in round but has not chosen to draw anymore cards. doneWithRound.
			case 'b':
				cout << "You have chosen to stay with a total of " << player.calcTotal() << "." << endl << endl;
				doneWithRound = true;
				break;
				// Surrender. Player is no longer in round, loses half their bet, and is doneWithRound.
			case 'c':
				player.stillIn = false;
				player.bank = player.bank - (player.bet / 2);
				cout << "You have surrendered. After losing half of your bet, you now have $";
				cout << player.bank << " in the bank." << endl << endl;
				doneWithRound = true;
				break;
				// Double. Doubles the bet and draws one card. If player busts, subtract bet from
				// their bank and set stillIn to false. Automatically doneWithRound.
			case 'd':
				player.bet = player.bet * 2;
				player.deck.addCard(playDeck.getCard());
				cout << "You have doubled your bet to $" << player.bet << " and have drawn a ";
				player.deck.last();
				cout << ". \n Your total is now " << player.calcTotal() << ".";
				if (player.calcTotal() > BLACKJACK)
				{
					cout << " Bust!" << endl;
					player.stillIn = false;
					player.bank = player.bank - player.bet;
					cout << "You now have $" << player.bank << " in the bank.";
				}
				cout << endl << endl;
				doneWithRound = true;
				break;
			}
			choice = ' ';
			validChoice = false;
		}
	}
	// Displays dealer's cards and checks for Blackjack. If total <= 17, dealer hits. If not, stays.
	// If dealer busts, set dealer's stillIn variable to false.
	void dealersTurn()
	{
		bool done = false;

		cout << "***Dealer's Turn***" << endl;
		if (dealer.calcTotal() == BLACKJACK)
		{
			cout << "Dealer's cards: " << endl;
			dealer.deck.display();
			cout << "Total: " << dealer.calcTotal();
			cout << " Blackjack!" << endl;
			done = true;
		}
		
		while (done == false)
		{
			cout << "Dealer's cards: " << endl;
			dealer.deck.display();
			cout << "Total: " << dealer.calcTotal() << endl;
			if (dealer.calcTotal() <= 17) // Dealer hits on 17 and under.
			{
				dealer.deck.addCard(playDeck.getCard());
				cout << "Dealer drew a ";
				dealer.deck.last();
				cout << " bringing their total to " << dealer.calcTotal() << ".";
				if (dealer.calcTotal() > BLACKJACK)
				{
					cout << " Bust!" << endl << endl;
					dealer.stillIn = false;
					done = true;
				}
				cout << endl;
			}
			else // Dealer stays on totals over 17.
			{
				cout << "Dealer has stayed with a total of " << dealer.calcTotal() << "." << endl << endl;
				done = true;
			}
		}
	}
	// Compares the totals of the players who are stillIn to the Dealer's total. Adjusts banks.
	void checkResults()
	{
		if (!dealer.stillIn)
		{
			for (int i = 0; i < players.size(); i++)
			{
				activePlayer = i;
				if (players.at(i).stillIn)
				{
					players.at(i).bank = players.at(i).bank + players.at(i).bet;
					cout << "Player " << activePlayer + 1 << " has won $" << players.at(i).bet;
					cout << " for a bank total of $" << players.at(i).bank << "." << endl << endl;
				}
			}
		}
		else
		{
			int dealerTotal = dealer.calcTotal();

			for (int i = 0; i < players.size(); i++)
			{
				activePlayer = i;
				if (players.at(i).stillIn)
				{
					int playerTotal = players.at(i).calcTotal();
					cout << "Player " << activePlayer + 1 << "'s total of " << playerTotal << " is ";
					if (playerTotal == dealerTotal)
					{
						cout << "equal to dealer's total of " << dealerTotal << ".";
						cout << " Push! Your bank account remains $" << players.at(i).bank << "." << endl << endl;
					}
					else if (playerTotal > dealerTotal)
					{
						players.at(i).bank = players.at(i).bank + players.at(i).bet;
						cout << "greater than dealer's total of " << dealerTotal << ".";
						cout << " You win $" << players.at(i).bet << " for a bank total of $";
						cout << players.at(i).bank << "." << endl << endl;
					}
					else
					{
						players.at(i).bank = players.at(i).bank - players.at(i).bet;
						cout << "less than dealer's total of " << dealerTotal << ".";
						cout << " You've lost $" << players.at(i).bet << " for a bank total of $";
						cout << players.at(i).bank << "." << endl << endl;
					}
				}
			}
		}
		resetActivePlayer();
	}
	// Checks the players vector to see if any players are stillIn the round.
	bool playersIn()
	{
		for (auto p : players)
		{
			if (p.stillIn)
			{
				return true;
			}
		}
		return false;
	}
	// Displays the amount of money lost/gained by each player throughout the game.
	void summary()
	{
		cout << "END OF GAME SUMMARY:" << endl;
		for (int i = 0; i < numPlayers; i++)
		{
			double total = players.at(i).bank - STARTER_BANK;
			if (total == 0)
			{
				cout << "Player " << i + 1 << " did not gain or lose any money this game." << endl;
			}
			else if (total > 0)
			{
				cout << "Player " << i + 1 << " gained $" << total << " this game." << endl;
			}
			else
			{
				cout << "Player " << i + 1 << " lost $" << abs(total) << " this game." << endl;
			}
		}
		cout << endl;
	}
	// Clears Players in players vectors, sets numPlayers back to -1.
	void cleanup()
	{
		players.clear();
		numPlayers = -1;
	}
};

// TO DO LIST:
// optional: Add a 1 second sleep between players' hands when displaying them? when displaying erros?
// ERROR in simulation: on second round of two player game, second player doubled their bet and got a 21 but
// game did not update their bank or indicate that they won at end of round? only first player was updated
// what if player has $0 heading into next round? need to implement something for this