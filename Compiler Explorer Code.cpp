#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

const int N = 3; // Size of the puzzle (3x3)
const int SIZE = N * N;

// Structure to represent a puzzle state
struct State {
    int puzzle[SIZE];
    int g; // Cost from the start state
    int h; // Heuristic value
};

// Function to calculate the minimum distance of misplaced tiles heuristic
int misplacedTilesHeuristic(const State& state) {
    int count = 0;
    for (int i = 0; i < SIZE; ++i) {
        if (state.puzzle[i] != i)
            ++count;
    }
    return count;
}

// Function to check if a state is the goal state
bool isGoalState(const State& state) {
    for (int i = 0; i < SIZE; ++i) {
        if (state.puzzle[i] != i)
            return false;
    }
    return true;
}

// Function to perform A* search
void solvePuzzleAStar(const State& initialState) {
    auto compare = [](const State& a, const State& b) {
        return a.g + a.h > b.g + b.h;
    };
    priority_queue<State, vector<State>, decltype(compare)> openSet(compare);
    map<vector<int>, int> gValues;

    openSet.push(initialState);
    gValues[vector<int>(initialState.puzzle, initialState.puzzle + SIZE)] = initialState.g;

    while (!openSet.empty()) {
        State current = openSet.top();
        openSet.pop();

        if (isGoalState(current)) {
            cout << "Solution Found!" << endl;
            cout << "Total Cost: " << current.g << endl;
            return;
        }

        int zeroPos;
        for (zeroPos = 0; zeroPos < SIZE; ++zeroPos) {
            if (current.puzzle[zeroPos] == 0)
                break;
        }

        const int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (int i = 0; i < 4; ++i) {
            int newRow = zeroPos / N + moves[i][0];
            int newCol = zeroPos % N + moves[i][1];

            if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N) {
                State newState = current;
                swap(newState.puzzle[zeroPos], newState.puzzle[newRow * N + newCol]);

                newState.g = current.g + 1;
                newState.h = misplacedTilesHeuristic(newState);

                vector<int> puzzleVector(newState.puzzle, newState.puzzle + SIZE);

                if (gValues.find(puzzleVector) == gValues.end() || newState.g < gValues[puzzleVector]) {
                    openSet.push(newState);
                    gValues[puzzleVector] = newState.g;
                }
            }
        }
    }

    cout << "No solution found." << endl;
}

int main() {
    State initialState = {{8, 0, 6, 5, 4, 7, 2, 3, 1}, 0, 0};
    solvePuzzleAStar(initialState);

    return 0;
}

