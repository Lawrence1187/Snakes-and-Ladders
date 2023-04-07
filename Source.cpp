//An element can only be an endpoint for a ladder or a snake, not both.

//A ladder should go to any element at least on the next level, for example if element 56 is the beginning of a ladder, it should at least go to 60 or higher.
//A snake should follow the same rule but going downwards.

//We are assuming that there are 10 ladders and 10 snakes.

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <tuple>
#include <chrono>
#include <thread>
#include <vector>
#include "Source.h"

using namespace std;

int higherDecimal(int i) {
	return (100 - (((i - 1) / 10) + 1) * 10);
}

int lowerDecimal(int j) {
	return ((j / 10) * 10);
}

void getSnakesAndLadders(map<tuple<int, int>, int>& boardEvents) {
	//1 = ladder, 2 = snake

	srand(time(NULL));

	int availableSquares[100];
	for (int i = 1; i <= 100; i++) {
		availableSquares[i - 1] = i;
	}
	for (int i = 10; i > 0; i--) {
		int startLadder = 1;
		int endLadder = 0;
		while (startLadder <= 1 || startLadder > 90) {
			startLadder = availableSquares[rand() % 100];
		}
		while (endLadder <= startLadder || endLadder < 1) {
			try {
				endLadder = availableSquares[(rand() % higherDecimal(startLadder)) + (100 - higherDecimal(startLadder))];
			}
			catch (const exception& e) {
				endLadder = 0;
			}
		}
		availableSquares[startLadder - 1] = 0;
		availableSquares[endLadder - 1] = 0;
		boardEvents.insert({{startLadder, endLadder}, 1});

		int startSnake = 0;
		int endSnake = 0;
		while (startSnake < 10 || startSnake > 99) {
			startSnake = availableSquares[rand() % 100];
		}
		while (endSnake >= startSnake || endSnake < 1) {
			try {
				endSnake = availableSquares[rand() % lowerDecimal(startSnake)];
			}
			catch (const exception& e) {
				endSnake = 1;
			}
		}
		availableSquares[startSnake - 1] = 0;
		availableSquares[endSnake - 1] = 0;
		boardEvents.insert({{startSnake, endSnake}, 2});
	}
}

int rollDice() {
	return rand() % 6 + 1;
}

int main() {
	srand(time(NULL));
	map<tuple<int, int>, int> boardEvents;
	getSnakesAndLadders(boardEvents);

	for (const auto& pair : boardEvents) {
		cout << "{" <<  "{" << get<0>(pair.first) << ", " << get<1>(pair.first) << "}, " << pair.second << "}" << endl;
	}

	pair<int, int> playerStatuses[] = {{1, 0}, {2, 0}};
	int currentPlayer = 0;

	while (true) {
		int diceRoll = rollDice();
		cout << "Player " << playerStatuses[currentPlayer].first << " rolls a " << diceRoll << endl;

		playerStatuses[currentPlayer].second += diceRoll;
		if (playerStatuses[currentPlayer].second > 100) {
			playerStatuses[currentPlayer].second -= (playerStatuses[currentPlayer].second - 100) * 2;
		}

		cout << "Player 1 on " << playerStatuses[0].second << endl;
		cout << "Player 2 on " << playerStatuses[1].second << endl;

		for (const auto& pair : boardEvents) {
			if (get<0>(pair.first) == playerStatuses[currentPlayer].second) {
				switch (pair.second) {
				case 1:
					playerStatuses[currentPlayer].second = get<1>(pair.first);
					cout << "Player " << playerStatuses[currentPlayer].first << " found a ladder! It takes you higher to " << get<1>(pair.first) << endl;
					break;
				case 2:
					playerStatuses[currentPlayer].second = get<1>(pair.first);
					cout << "Player " << playerStatuses[currentPlayer].first << " encountered a snake! It gobbles you and spits you back out at " << get<1>(pair.first) << endl;
					break;
				}
				if (get<1>(pair.first) == 100) {
					cout << "Player " << playerStatuses[currentPlayer].first << " has landed on 100 first and wins! Game Over." << endl;
					return 0;
				}
			}
		}
		if (playerStatuses[currentPlayer].second == 100) {
			cout << "Player " << playerStatuses[currentPlayer].first << " has landed on 100 first and wins! Game Over." << endl;
			return 0;
		}
		if (currentPlayer == 0) {
			currentPlayer = 1;
		}
		else {
			currentPlayer = 0;
		}

		//this_thread::sleep_for(chrono::seconds(1));
	}
	

	return 0;
}