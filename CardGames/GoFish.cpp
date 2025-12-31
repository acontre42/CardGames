#include "GoFish.h"

GoFish::GoFish()
{
	numPlayers = -1;
	activePlayer = FIRST_PLAYER;
	handSize = -1;
}

void GoFish::setup()
{
	cout << "*****GO FISH*****" << endl;
	while (numPlayers < MIN_PLAYERS || numPlayers > MAX_PLAYERS)
	{
		cout << "Please enter number of players(2-4):";
		cin >> numPlayers;
		if (cin.fail())
		{
			cout << "\nError: Must be a number.\n";
			cin.clear();
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	handSize = (numPlayers == MIN_PLAYERS ? TWO_PLAYER_HAND : THREE_PLAYER_HAND);

	for (int i = 1; i <= numPlayers; i++)
	{
		string name = "Player " + to_string(i);
		players.emplace_back(name);
	}
}

void GoFish::run()
{
	char choice;

	while (true) 
	{
		setup();
		playRound();
		// Play again?
		cout << "Would you like to play again? Enter 'Q' to quit or any other key to play again." << endl;
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		choice = tolower(choice);

		if (choice == 'q') 
		{
			return;
		}

		cleanup();
	}
}

void GoFish::playRound()
{
	dealHands();

	while (!deck.empty() || !allHandsEmpty())
	{
		if (players[activePlayer].hand.empty() && deck.empty())
		{
			cout << "Sorry, " << players[activePlayer].name << ", there are no more cards in your hand or in the play deck! ";
			cout << "Skipping to the next player..." << endl;
		}
		else
		{
			takeTurn(players[activePlayer]);
		}

		activePlayer = (activePlayer + 1) % numPlayers;
	}
	cout << "All cards gone!" << endl;
	displayAllBooks();
	decideWinner();
}




void GoFish::startTurnsBuffer() const
{
	char buffer;
	for (int i = 0; i < NUM_STARS; i++)
	{
		cout << "\n*\n";
	}
	cout << players[activePlayer].name << "'s Turn" << endl;
	cout << "Enter any key to continue:";
	cin >> buffer;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void GoFish::endTurnBuffer()
{
	char buffer;
	cout << "*****END OF TURN*****" << endl;
	cout << "Enter any key to pass to the next player's turn: ";
	cin >> buffer;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void GoFish::displayTestingStuff()
{
	cout << "\n\n***********TESTING STUFF*********" << endl;
	cout << "PLAYER CARDS " << endl;
	for (auto p : players)
	{
		cout << p.name << "'s cards:" << endl;
		p.hand.display();
		cout << "Total: " << p.hand.getSize() << endl;
		cout << "Book pile: " << endl;
		for (auto b : p.booksPile)
		{
			b.display();
			cout << endl;
		}
		cout << "Num. of books: " << p.numBooks << endl << endl;
	}
	cout << "Deck size: " << deck.getSize() << endl;
	cout << "\n********************************\n\n";
}