//
// Created by william6364 on 25/3/2017.
//

#ifndef TETRISAI_CPLAYER_H
#define TETRISAI_CPLAYER_H

#include <vector>
#include <random>
#include <iostream>
#include "../brains/Brainv2.h"


/*
 * Piece 0, Orientation 0:
	XX
	XX

Piece 1, Orientation 0:
	X
	X
	X
	X

Piece 1, Orientation 1:
	XXXX

Piece 2, Orientation 0:
	X
	X
	XX

Piece 2, Orientation 1:
	XXX
	X

Piece 2, Orientation 2:
	XX
	 X
	 X

Piece 2, Orientation 3:
	  X
	XXX

Piece 3, Orientation 0:
	 X
	 X
	XX

Piece 3, Orientation 1:
	X
	XXX

Piece 3, Orientation 2:
	XX
	X
	X

Piece 3, Orientation 3:
	XXX
	  X

Piece 4, Orientation 0:
	X
	XX
	X

Piece 4, Orientation 1:
	XXX
	 X

Piece 4, Orientation 2:
	 X
	XX
	 X

Piece 4, Orientation 3:
	 X
	XXX

Piece 5, Orientation 0:
	 XX
	XX

Piece 5, Orientation 1:
	X
	XX
	 X

Piece 6, Orientation 0:
	XX
	 XX

Piece 6, Orientation 1:
	 X
	XX
	X
 * */

using namespace std;

class CPlayer{
private:
    // Number of distinct pieces
    const int NUM_PIECES = 7;

    // Dimensions of board
    const int NUM_ROWS = 20;
    const int NUM_COLS = 10;

    // Maximum number of pieces to be played before force end
    const int TOTAL_GAME_COUNT = 1000000000;
    int gameCount = TOTAL_GAME_COUNT;

    // Array to store the board state as an array of integers
    int* rows;

    // Array to store original rows array for resetting purposes after each possible move
    int* currentRows;

    // Array to store height of each column
    int* top;

    // Array to store original top array for resetting purposes after each possible move
    int* currentTop;

    // Vector to store the coordinates taken up by the new piece
    vector <pair<int,int> >filledCoordinates;

    // Number of inputs to be passed into the neural network
    const int numInputs = 9;

    // Array of inputs to be passed into the neural network
    double* inputs;

    // Tile specific feature values
    int wallContact = 0;
    int floorContact = 0;
    int pieceContact = 0;

    // Number of lines cleared
    int score = 0;

    // Random piece generator
    default_random_engine randomEngine;
    uniform_int_distribution<int> randomDistribution;

    // Note that the first index is rotation, the second index is position.
    pair<int, int> findNextMove(int tileType);

    // Returns the number of rows cleared with this move.
    // Returns -1 if failed to place a tile (due to game over).
    int performMove(int pieceIndex, int rotationIndex, int leftPosition);

public:
    // Constructor
    CPlayer();

    // Assign a brain and a seed before playing a game.
    Brainv2 brain = Brainv2();
    int seed;

    // Set to true if printing the game at each step is needed.
    bool shouldPrint = false;

    // Actually playing a game.
    void playGame();

    // Gets the final score of the game.
    int getFinalScore();

    // Resets the current state of the game.
    void reset();

    // Prints the state of the game.
    void print(int currentPieceIndex);

    ~CPlayer();
};

#endif //TETRISAI_CPLAYER_H
