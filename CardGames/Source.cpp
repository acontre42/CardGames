#include <iostream>
#include <vector>
#include <algorithm>
#include "Card.h"
#include "DeckOfCards.h"
#include "Blackjack.h"
#include "GoFish.h"
using namespace std;

int main()
{
	Blackjack blackjack;
	GoFish gofish;
	char choice;

	while (true)
	{
		cout << "***** CARD GAMES *****" << endl;
		cout << "What card game would you like to play? Enter one of the options below or enter 'Q' to quit." << endl;
		cout << " a) Blackjack" << endl;
		cout << " b) Go Fish" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		choice = tolower(choice);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl;

		switch (choice)
		{
		case 'a':
			blackjack.run();
			break;
		case 'b':
			gofish.run();
			break;
		case 'q':
			cout << "Goodbye!" << endl;
			exit(0);
		default:
			cout << "Invalid choice." << endl << endl;
		}
	}
	
	return 0;
}