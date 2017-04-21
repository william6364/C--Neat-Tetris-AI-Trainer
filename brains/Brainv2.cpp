//
// Created by william6364 on 26/3/2017.
//

#include "Brainv2.h"

Brainv2::Brainv2():net(NULL){
}

void Brainv2::setNetwork(Network* newNet){
    net = newNet;
}

double Brainv2::Update(double* inputs){
    //Run the inputs through the NN to get an output
    int depth = net->max_depth();
    net->load_sensors(inputs);
    net->activate();
    for(int relax = 0; relax <= depth; relax++){
        net->activate();
    }
    double ans = (*(net->outputs.begin()))->activation;
    net->flush();
    return ans;
}