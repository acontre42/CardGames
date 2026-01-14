#include "Blackjack.h"

Blackjack::Blackjack()
{
	numPlayers = -1;
}

void Blackjack::setup()
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
	for (int i = 1; i <= numPlayers; i++)
	{
		string name = "Player " + to_string(i);
		players.emplace_back(name); // Adds Players to players vector.
	}
	cout << endl;
}

void Blackjack::run()
{
	setup();
	char choice;
	do
	{
		getBetsFromPlayers();
		playARound();
		if (!playersIn())
		{
			cout << "There are no players left with enough money to place a bet!" << endl << endl;
			break;
		}
		cout << "Would you like to play again? Enter 'Q' to quit or any key to play again." << endl;
		cin >> choice;
		choice = tolower(choice);
		cout << endl;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	} while (choice != 'q');
	summary();
	cleanup(); // In case player quits and then returns.
}

void Blackjack::getBetsFromPlayers()
{
	cout << "***PLACE YOUR BETS***" << endl << "Note: Bet amounts must be whole numbers." << endl << endl;
	for (auto& player : players)
	{
		if (player.stillIn)
		{
			placeBet(player);
		}
	}
	cout << "All bets placed.\n\n";
}

void Blackjack::placeBet(Player& player)
{
	int betAmount = 0;
	do
	{
		cout << player.name << "'s bank account: $" << player.bank << endl;
		cout << "How much would you like to bet? ";
		cin >> betAmount;
		if (!cin.fail())
		{
			if (betAmount < MIN_BET)
			{
				cout << "Bets must be at least $" << MIN_BET << "." << endl;
			}
			if (betAmount > player.bank)
			{
				cout << "You cannot bet more money than you have in the bank." << endl;
			}
		}
		else
		{
			cout << "Please enter a valid number." << endl;
			cin.clear();
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	} while (betAmount < MIN_BET || betAmount > player.bank);

	player.bet = betAmount;
	cout << player.name << " has bet $" << player.bet << "." << endl << endl;
}

void Blackjack::playARound()
{
	// Distribute cards to Dealer and Players.
	distributeTwoCards(dealer);
	for (auto& player : players)
	{
		if (player.stillIn)
		{
			distributeTwoCards(player);
		}
	}
	cout << "***PLAYERS' CARDS***" << endl << endl;
	cout << "Dealer's cards: " << endl;
	dealer.deck.peek();
	cout << "One card face down." << endl << endl;
	for (auto& player : players)
	{
		if (player.stillIn)
		{
			cout << player.name << "'s cards: " << endl;
			player.deck.display();
			cout << "Total: " << player.calcTotal();
			if (player.calcTotal() == BLACKJACK)
			{
				cout << " Blackjack!";
			}
			cout << endl << endl;
		}
	}
	screenBuffer();
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
	// Reset for next round
	reset();
}

void Blackjack::distributeTwoCards(Player& player)
{
	player.deck.addCard(playDeck.getCard());
	player.deck.addCard(playDeck.getCard());
}

void Blackjack::getMovesFromPlayers()
{
	for (auto& player : players)
	{
		if (player.stillIn)
		{
			getMove(player);
			screenBuffer();
		}
	}
}

void Blackjack::getMove(Player& player)
{
	cout << "***" << player.name << "'s Move***" << endl;
	bool doneWithRound = false;

	if (player.calcTotal() == BLACKJACK)
	{
		doneWithRound = true;
		cout << "You have a Blackjack and have won an extra 50% of your bet." << endl;
		player.bank = player.bank + (player.bet * 1.5);
		cout << "You now have $" << player.bank << " in the bank." << endl << endl;
		player.stillIn = false;
	}

	bool canDouble = (player.bet * 2 <= player.bank ? true : false);
	char choice;
	bool validChoice;
	while (!doneWithRound)
	{
		validChoice = false;
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
				if (player.bank < MIN_BET)
				{
					cout << "\nUnfortunately, " << player.name << ", you do not have enough money left to place a bet and are out of the game.";
				}
				doneWithRound = true;
			}
			cout << endl << endl;
			break;
			// Stay. Player is still in round but has not chosen to draw anymore cards. Player's turn ends.
		case 'b':
			cout << "You have chosen to stay with a total of " << player.calcTotal() << "." << endl << endl;
			doneWithRound = true;
			break;
			// Surrender. Player is no longer in round, loses half their bet, and their turn ends.
		case 'c':
			player.stillIn = false;
			player.bank = player.bank - (player.bet / 2);
			cout << "You have surrendered. After losing half of your bet, you now have $";
			cout << player.bank << " in the bank." << endl << endl;
			doneWithRound = true;
			break;
			// Double. Doubles the bet and draws one card. If player busts, subtract bet from
			// their bank and set stillIn to false. Automatically ends turn.
		case 'd':
			player.bet = player.bet * 2;
			player.deck.addCard(playDeck.getCard());
			cout << "You have doubled your bet to $" << player.bet << " and have drawn a ";
			player.deck.last();
			cout << ".\nYour total is now " << player.calcTotal() << ".";
			if (player.calcTotal() > BLACKJACK)
			{
				cout << " Bust!" << endl;
				player.stillIn = false;
				player.bank = player.bank - player.bet;
				cout << "You now have $" << player.bank << " in the bank.";
				if (player.bank < MIN_BET)
				{
					cout << "\nUnfortunately, " << player.name << ", you do not have enough money left to place a bet and are out of the game.";
				}
			}
			cout << endl << endl;
			doneWithRound = true;
			break;
		}
	}
}

void Blackjack::dealersTurn()
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
				cout << " Bust!";
				dealer.stillIn = false;
				done = true;
			}
			cout << endl << endl;
		}
		else // Dealer stays on totals over 17.
		{
			cout << "Dealer has stayed with a total of " << dealer.calcTotal() << "." << endl << endl;
			done = true;
		}
	}
}

void Blackjack::checkResults()
{
	if (!dealer.stillIn)
	{
		for (auto& player : players)
		{
			if (player.stillIn)
			{
				player.bank += player.bet;
				cout << player.name << " has won $" << player.bet;
				cout << " for a bank total of $" << player.bank << "." << endl << endl;
			}
		}
	}
	else
	{
		int dealerTotal = dealer.calcTotal();

		for (auto& player : players)
		{
			if (player.stillIn)
			{
				int playerTotal = player.calcTotal();
				cout << player.name << "'s total of " << playerTotal << " is ";
				if (playerTotal == dealerTotal)
				{
					cout << "equal to dealer's total of " << dealerTotal << ".";
					cout << " Push! Your bank account remains $" << player.bank << "." << endl << endl;
				}
				else if (playerTotal > dealerTotal)
				{
					player.bank += player.bet;
					cout << "greater than dealer's total of " << dealerTotal << ".";
					cout << " You win $" << player.bet << " for a bank total of $";
					cout << player.bank << "." << endl << endl;
				}
				else
				{
					player.bank -= player.bet;
					cout << "less than dealer's total of " << dealerTotal << ".";
					cout << " You've lost $" << player.bet << " for a bank total of $";
					cout << player.bank << ".";
					if (player.bank < MIN_BET)
					{
						cout << "\nUnfortunately, " << player.name << ", you do not have enough money left to place a bet and are out of the game.";
						player.stillIn = false;
					}
					cout << endl << endl;
				}
			}
		}
	}
}

bool Blackjack::playersIn()
{
	for (auto& p : players)
	{
		if (p.stillIn)
		{
			return true;
		}
	}
	return false;
}

void Blackjack::summary()
{
	cout << "END OF GAME SUMMARY:" << endl;
	for (auto& player : players)
	{
		double total = player.bank - STARTER_BANK;

		cout << player.name;
		if (total == 0)
		{
			cout << " broke even this game. ";
		}
		else if (total > 0)
		{
			cout << " gained $" << total << " this game. ";
		}
		else
		{
			cout << " lost $" << abs(total) << " this game. ";
		}
		cout << "End game bank: $" << player.bank << endl << endl;
	}
}

void Blackjack::reset()
{
	// Return all players' cards to the playDeck and reshuffle.
	while (dealer.deck.getSize() != 0)
	{
		playDeck.addCard(dealer.deck.getCard());
	}
	for (auto& p : players)
	{
		while (p.deck.getSize() != 0)
		{
			playDeck.addCard(p.deck.getCard());
		}
	}
	playDeck.reshuffle();
	// Reset all players' bets and set their stillIn variable back to true if they have at least MIN_BET in the bank.
	dealer.stillIn = true;
	for (auto& player : players)
	{
		player.bet = 0;
		if (player.bank >= MIN_BET)
		{
			player.stillIn = true;
		}
	}
}

void Blackjack::cleanup()
{
	players.clear();
	numPlayers = -1;
}

void Blackjack::screenBuffer()
{
	char buffer;
	cout << "Enter any key to continue: ";
	cin >> buffer;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << endl << endl;
}