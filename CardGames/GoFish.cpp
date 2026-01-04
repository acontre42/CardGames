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

void GoFish::takeTurn(Player& player)
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
	} while (goAgain);

	endTurnBuffer();
}

void GoFish::dealHands()
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
}

void GoFish::dealCard(Player& player)
{
	player.hand.addCard(deck.getCard());
}

void GoFish::displayAvailableTargets()
{
	for (int i = 0; i < players.size(); i++)
	{
		if (i != activePlayer && !players[i].hand.empty())
		{
			cout << players.at(i).name << endl;
		}
	}
}

void GoFish::cleanup()
{
	players.clear();
	numPlayers = -1;
	activePlayer = FIRST_PLAYER;
	handSize = -1;
	deck.clear();
	deck.populate();
}

bool GoFish::allHandsEmpty()
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

bool GoFish::targetsAvailable()
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

void GoFish::checkForBooks(Player& player) const
{
	if (player.hand.getSize() < BOOK) // If player has less than 4 cards, just return.
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
	// If books found, move Cards to booksPile, increment numBooks, and inform Player.
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

void GoFish::decideWinner()
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

void GoFish::displayAllBooks()
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

void GoFish::displayCurrentHand()
{
	cout << "Your current hand:" << endl;
	players[activePlayer].hand.display();
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