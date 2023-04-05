//An element can only be an endpoint for a ladder or a snake, not both.

//A ladder should go to any element at least on the next level, for example if element 56 is the beginning of a ladder, it should at least go to 60 or higher.
//A snake should follow the same rule but going downwards.

//We are assuming that there are 8 ladders and 8 snakes.

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <tuple>
#include <chrono>
#include <thread>
#include "Source.h"

using namespace std;

unordered_map<int, tuple<int, int>> getSnakesAndLadders() {
	//1 = ladder, 2 = snake

	unordered_map<int, tuple<int, int>> boardEvents = {{1, {2, 23}}, {1, {8, 34}}, {1, {20, 77}}, {1, {32, 68}}, {1, {41, 79}}, {1, {74, 88}}, {1, {82, 100}}, {1, {85, 95}},
	{2, {74, 5}}, {2, {29, 9}}, {2, {38, 15}}, {2, {97, 25}}, {2, {53, 33}}, {2, {62, 37}}, {2, {86, 54}}, {2, {92, 70}}};

	return boardEvents;
}

int rollDice() {
	return rand() % 6 + 1;
}

int main() {
	srand(time(NULL));
	unordered_map<int, tuple<int, int>> boardEvents = getSnakesAndLadders();

	pair<int, int> playerStatuses[] = {{1, 0}, {2, 0}};
	int currentPlayer = 0;

	while (true) {
		int diceRoll = rollDice();
		cout << "Player " << playerStatuses[currentPlayer].first << " rolls a " << diceRoll << endl;

		playerStatuses[currentPlayer].second += diceRoll;
		if (playerStatuses[currentPlayer].second > 100) {
			playerStatuses[currentPlayer].second -= (playerStatuses[currentPlayer].second - 100)*2;
		}

		cout << "Player 1 on " << playerStatuses[0].second << endl;
		cout << "Player 2 on " << playerStatuses[1].second << endl;

		for (const auto& pair : boardEvents) {
			if (get<0>(pair.second) == playerStatuses[currentPlayer].second) {
				switch (pair.first) {
				case 1:
					playerStatuses[currentPlayer].second = get<1>(pair.second);
					cout << "You found a ladder! It takes you higher to " << get<1>(pair.second) << endl;
					break;
				case 2:
					playerStatuses[currentPlayer].second = get<1>(pair.second);
					cout << "You've encountered a snake! It gobbles you and spits you back out at " << get<1>(pair.second) << endl;
					break;
				}
				if (get<1>(pair.second) == 100) {
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

		this_thread::sleep_for(chrono::seconds(1));
	}
	

	return 0;
}