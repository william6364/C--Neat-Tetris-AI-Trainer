//
// Created by william6364 on 26/3/2017.
//

#ifndef TETRISAI_CCONTROLLERV2_H
#define TETRISAI_CCONTROLLERV2_H

#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <thread>
#include <ctime>

#include "../brains/Brainv2.h"
#include "../player/CPlayer.h"
#include "../NEAT.1.2.1/neat.h"
#include "../NEAT.1.2.1/network.h"
#include "../NEAT.1.2.1/population.h"
#include "../NEAT.1.2.1/organism.h"
#include "../NEAT.1.2.1/genome.h"
#include "../NEAT.1.2.1/species.h"

using namespace std;

class CControllerv2 {

private:

    // Number of generations to run the algorithm for
    const int NUM_OF_GENERATIONS = 1000000000;

    // Number of games per player per generation
    const int gamesPerGeneration = 5;

    // Storage for players
    vector <CPlayer> Players;

    // Storage for threads for parallel programming
    vector <thread> Threads;

    // Number of players
    int iNumPlayers;

    // Highest score
    int highestScore;

    // Total scores of each brain across multiple games
    int* Scores;

    // Storage for score of individual games for output
    vector < vector <int> > GameScores;

public:

    // Constructor
    CControllerv2();

    // Run a generation
    void Update(Population *pop, int generation);


};

#endif //TETRISAI_CCONTROLLERV2_H
