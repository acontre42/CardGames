#include <iostream>
#include <vector>
#include <algorithm>
#include "Card.h"
#include "DeckOfCards.h"
#include "BlackjackGame.h"
#include "GoFish.h"
using namespace std;

int main()
{
	char choice = ' ';
	bool validChoice = false;
	BlackjackGame blackjack;
	GoFish gofish;

	while (choice != 'q')
	{
		choice = ' ';

		while (!validChoice)
		{
			cout << "***** CARD GAMES *****" << endl;
			cout << "What card game would you like to play? Enter an option or 'Q' to quit." << endl;
			cout << "a) Blackjack" << endl;
			cout << "b) Go Fish" << endl;
			cout << "Enter your choice: ";
			cin >> choice;
			choice = tolower(choice);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << endl;
			if (choice == 'a' || choice == 'b' || choice == 'q')
			{
				validChoice = true;
			}
			else
			{
				cout << "Invalid choice." << endl << endl;
			}
		}
		validChoice = false;

		switch (choice)
		{
		case 'a':
			blackjack.run();
			break;
		case 'b':
			gofish.run();
			break;
		default:
			cout << "Goodbye!" << endl;
		}
	}
	
	return 0;
}