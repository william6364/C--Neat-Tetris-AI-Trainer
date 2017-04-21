//
// Created by william6364 on 26/3/2017.
//

#ifndef TETRISAI_BRAINV2_H
#define TETRISAI_BRAINV2_H

#include <vector>
#include "../NEAT.1.2.1/neat.h"
#include "../NEAT.1.2.1/network.h"
#include "../NEAT.1.2.1/population.h"
#include "../NEAT.1.2.1/organism.h"
#include "../NEAT.1.2.1/genome.h"
#include "../NEAT.1.2.1/species.h"

using namespace std;
using namespace NEAT;

class Brainv2{
private:
    // The neural network which forms this brain
    Network *net;

public:
    // Constructor
    Brainv2();

    // Setter
    void setNetwork(Network* newNet);

    // Get neural network output from given set of inputs
    double Update(double* inputs);

};
#endif //TETRISAI_BRAINV2_H
