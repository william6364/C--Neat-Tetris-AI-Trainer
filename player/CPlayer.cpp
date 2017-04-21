//
// Created by william6364 on 25/3/2017.
//

#include <iostream>
#include "CPlayer.h"
#include <algorithm>

/* Shape Descriptions */
//the next several arrays define the piece vocabulary in detail
//width of the pieces [piece ID][rotationIndexation]
static vector<vector<int>> pWidth = {
        {2},
        {1, 4},
        {2, 3, 2, 3},
        {2, 3, 2, 3},
        {2, 3, 2, 3},
        {3, 2},
        {3, 2}
};

//height of the pieces [piece ID][rotationIndexation]
static vector<vector<int>> pHeight = {
        {2},
        {4, 1},
        {3, 2, 3, 2},
        {3, 2, 3, 2},
        {3, 2, 3, 2},
        {2, 3},
        {2, 3}
};

static vector<int> pRotations = {
        1, 2, 4, 4, 4, 2, 2
};

// [piece type][rotationIndexation][bottom position from left to right]
static vector<vector<vector<int>>> pBottom = {
        {{0, 0}},
        {{0},       {0, 0, 0, 0}},
        {{0, 0},    {0, 1, 1}, {2, 0}, {0, 0, 0}},
        {{0, 0},    {0, 0, 0}, {0, 2}, {1, 1, 0}},
        {{0, 1},    {1, 0, 1}, {1, 0}, {0, 0, 0}},
        {{0, 0, 1}, {1, 0}},
        {{1, 0, 0}, {0, 1}}
};

// [piece type][rotationIndexation][top position from left to right]
static vector<vector<vector<int>>> pTop = {
        {{2, 2}},
        {{4},       {1, 1, 1, 1}},
        {{3, 1},    {2, 2, 2}, {3, 3}, {1, 1, 2}},
        {{1, 3},    {2, 1, 1}, {3, 3}, {2, 2, 2}},
        {{3, 2},    {2, 2, 2}, {2, 3}, {1, 2, 1}},
        {{1, 2, 2}, {3, 2}},
        {{2, 2, 1}, {2, 3}}
};

/* Constructors */
CPlayer::CPlayer() {
    // Declare arrays
    rows = (int *) malloc(NUM_ROWS * sizeof(int));
    currentRows = (int *) malloc(NUM_ROWS * sizeof(int));
    top = (int *) malloc(NUM_COLS * sizeof(int));
    currentTop = (int *) malloc(NUM_COLS * sizeof(int));
    inputs = (double *) malloc(numInputs * sizeof(double));

    // Random function to generate tetris piece
    randomDistribution = uniform_int_distribution<int>(0, NUM_PIECES - 1);
}

void CPlayer::playGame() {
    // Reset game
    reset();

    // Stop if maximum number of pieces is reached
    while (gameCount-- > 0) {
        // Generate random piece
        int tileChosen = randomDistribution(randomEngine);

        // Find and perform best move
        pair<int, int> nextMove = findNextMove(tileChosen);
        int numRowsCleared = performMove(tileChosen, nextMove.first, nextMove.second);

        if (numRowsCleared == -1) {
            // If game is lost
            if (shouldPrint) {
                print(tileChosen);
            }
            return;
        } else {
            // Add number of lines cleared to score
            score += numRowsCleared;
        }

        // Print if print flag is true
        if (shouldPrint) {
            print(tileChosen);
        }
    }

}

int CPlayer::performMove(int pieceIndex, int rotationIndex, int leftPosition) {
    // Reset tile specific feature values
    wallContact = 0;
    floorContact = 0;
    pieceContact = 0;

    // Height if the first column makes contact
    int height = top[leftPosition] - pBottom[pieceIndex][rotationIndex][0];

    // For each column beyond the first in the piece
    for (int c = 0; c < pWidth[pieceIndex][rotationIndex]; c++) {
        // Height if this column makes contact
        height = max(height, top[leftPosition + c] - pBottom[pieceIndex][rotationIndex][c]);

        // Floor Contact
        if (top[leftPosition + c] == -1 && pBottom[pieceIndex][rotationIndex][c] == 0) {
            floorContact++;
        }
    }

    // Check if game ends after performing this move
    if (height + pHeight[pieceIndex][rotationIndex] >= NUM_ROWS) {
        return -1;
    }

    // For each column in the piece - fill in the appropriate blocks
    filledCoordinates.clear();
    for (int i = 0; i < pWidth[pieceIndex][rotationIndex]; i++) {
        // From bottom to top of brick
        for (int h = height + pBottom[pieceIndex][rotationIndex][i] + 1;
             h <= height + pTop[pieceIndex][rotationIndex][i]; h++) {
            rows[h] |= (1 << (i + leftPosition)); // Fill the square in the board
            filledCoordinates.push_back(
                    make_pair(h, i + leftPosition)); // Store the coordinates filled by the new piece
        }
    }


    // Wall Contact
    // If piece hugs left wall
    if (leftPosition == 0) {
        wallContact += pTop[pieceIndex][rotationIndex][0] - pBottom[pieceIndex][rotationIndex][0];
    }
    // If piece hugs right wall
    if (leftPosition + pWidth[pieceIndex][rotationIndex] - 1 == NUM_COLS - 1) {
        wallContact += pTop[pieceIndex][rotationIndex][pWidth[pieceIndex][rotationIndex] - 1] -
                            pBottom[pieceIndex][rotationIndex][pWidth[pieceIndex][rotationIndex] - 1];
    }

    // Piece Contact
    for (pair<int, int> coord : filledCoordinates) {
        // Find original coordinate from integer
        int row = coord.first;
        int col = coord.second;

        // Check if the squares next to each piece square were already filled
        pair<int, int> left, right, down;
        left = make_pair(row, col - 1);
        right = make_pair(row, col + 1);
        down = make_pair(row - 1, col);

        // left side
        if (col != 0 && find(filledCoordinates.begin(), filledCoordinates.end(), left) == filledCoordinates.end() &&
            (rows[left.first] & (1 << left.second))) {
            pieceContact++;
        }

        // right side
        if (col != NUM_COLS - 1 &&
            find(filledCoordinates.begin(), filledCoordinates.end(), right) == filledCoordinates.end() &&
            (rows[right.first] & (1 << right.second))) {
            pieceContact++;
        }

        // down side
        if (row != 0 && find(filledCoordinates.begin(), filledCoordinates.end(), down) == filledCoordinates.end() &&
            (rows[down.first] & (1 << down.second))) {
            pieceContact++;
        }
    }

    // Calculate new board after rows are cleared
    int rowsCleared = 0;
    for (int r = height + 1; r < NUM_ROWS; r++) {
        // If row is full
        if (rows[r] + 1 == (1 << NUM_COLS)) {
            rowsCleared++;
        }
            // Otherwise, shift row down by number of lines cleared
        else if (rowsCleared > 0) {
            rows[r - rowsCleared] = rows[r];
        }
    }

    // Reset top rows based on number of rows cleared
    for (int r = NUM_ROWS - 1; r >= NUM_ROWS - rowsCleared; r--) {
        rows[r] = 0;
    }

    // Reset top array then calculate new values of top array
    int hasBlocked = 0;
    for (int c = 0; c < NUM_COLS; c++) {
        top[c] = -1;
    }

    // Search downwards
    for (int r = NUM_ROWS - 1; r >= 0; r--) {
        // Find which columns are filled in this row that have not been filled before
        int topSquares = (rows[r] & ~hasBlocked);
        // Update top array for each column that is now filled but was not filled before
        while (topSquares > 0) {
            top[__builtin_ctz(topSquares)] = r;
            topSquares ^= (topSquares & (-topSquares));
        }
        // Update which columns have already been filled
        hasBlocked |= rows[r];
    }
    return rowsCleared;
};

pair<int, int> CPlayer::findNextMove(int tileType) {
    // Store best move and its score
    pair<int, int> bestMove = make_pair(-1, -1);
    double bestOutput = 0.0;
    //Save current board
    for (int i = 0; i < NUM_COLS; i++) {
        currentTop[i] = top[i];
    }
    for (int r = 0; r < NUM_ROWS; r++) {
        currentRows[r] = rows[r];
    }
    //Try all possible moves
    for (int i = 0; i < pRotations[tileType]; ++i) {
        for (int j = 0; j <= NUM_COLS - pWidth[tileType][i]; ++j) {

            // If losing state, number of lines cleared is -10000.
            int linesCleared = performMove(tileType, i, j);
            if (linesCleared < 0) continue; // losing move, do not consider.

            // Reset feature values then proceed to calculate all the feature values
            for (int k = 0; k < numInputs; k++) inputs[k] = 0.0;

            // 0. 2 Power of Lines Cleared.
            inputs[0] = (double) (1 << linesCleared);
            for (int c = 0; c < NUM_COLS; c++) {
                // 1. Sum of Heights of each column
                inputs[1] += (double) ((top[c] + 1));

                // 2. Bumpiness
                if (c > 0) {
                    inputs[2] += (double) ((top[c] - top[c - 1]) * (top[c] - top[c - 1]));
                }
            }
            int hasBlocked = 0;
            for (int r = NUM_ROWS - 1; r >= 0; r--) {
                // 3. Sum of Heights of each block
                inputs[3] += (double) ((r + 1) * __builtin_popcount(rows[r]));

                // 5. Number of Holes
                inputs[5] += (double) (__builtin_popcount(hasBlocked & ~rows[r]));
                hasBlocked |= rows[r];
            }
            int hasHoles = 0;
            for (int r = 0; r < NUM_ROWS; r++) {
                // 4. Blockades
                inputs[4] += (double) (__builtin_popcount(hasHoles & rows[r]));
                hasHoles |= (~rows[r]);
            }

            // Piece-Specific Features
            inputs[6] = (double) (wallContact);
            inputs[7] = (double) (floorContact);
            inputs[8] = (double) (pieceContact);

            // Get evaluation of board from neural network and update if move is better or if it is first legal move
            double output = brain.Update(inputs);
            if (bestMove.first == -1 || output > bestOutput) {
                bestMove = make_pair(i,j);
                bestOutput = output;
            }

            // Reset board for simulation of next move
            for (int r = 0; r < NUM_ROWS; r++) {
                rows[r] = currentRows[r];
            }
            for (int c = 0; c < NUM_COLS; c++) {
                top[c] = currentTop[c];
            }
        }
    }
    // If all moves are losing moves, return first move otherwise return best move
    if (bestMove.first == -1) bestMove = make_pair(0, 0);
    return bestMove;
}

int CPlayer::getFinalScore() {
    // Return number of lines cleared
    return score;
}

void CPlayer::reset() {
    // Reset seed
    randomDistribution.reset();
    randomEngine.seed((unsigned int) seed);

    // Reset board
    for (int j = 0; j < NUM_ROWS; j++) {
        rows[j] = 0;
    }
    for (int i = 0; i < NUM_COLS; i++) {
        top[i] = -1;
    }

    // Reset score
    score = 0;

    // Reset pieces played
    gameCount = TOTAL_GAME_COUNT;
}

void CPlayer::print(int currentPieceIndex) {
    cout << endl;
    cout << "Current Score:  " << score << endl;
    cout << "Pieces Generated: " << TOTAL_GAME_COUNT - gameCount << endl;
    cout << "Current Piece: " << currentPieceIndex << endl;
    cout << endl;

    for (int row = NUM_ROWS - 1; row >= 0; row--) {
        for (int col = 0; col < NUM_COLS; col++) {
            cout << ((rows[row] & (1 << col)) ? "X" : ".") << " ";
        }
        cout << endl;
    }
    cout << "0 1 2 3 4 5 6 7 8 9" << endl << endl;
    for (int col = 0; col < NUM_COLS; col++) {
        cout << top[col] << " ";
    }
    cout << endl;
}

CPlayer::~CPlayer() {

}