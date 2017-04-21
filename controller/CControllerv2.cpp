//
// Created by william6364 on 26/3/2017.
//

#include "CControllerv2.h"

CControllerv2::CControllerv2() : iNumPlayers(NEAT::pop_size),
                                 highestScore(0) {

    //Initialise Everything
    Population *pop = 0;

    //Generate initial population from a single gene
    Genome *start_genome;

    ifstream iFile("../startgenes", ios::in);
    cout << "Reading in the start genome" << endl;
    start_genome = new Genome(0, iFile);
    iFile.close();

    cout << "Start Genome: " << start_genome << endl;

    //Spawn the Population
    cout << "Spawning Population off Genome" << endl;

    pop = new Population(start_genome, NEAT::pop_size);

    //Initialise players and score array
    cout << "Initialising Players" << endl;
    for (int i = 0; i < iNumPlayers; ++i) {
        Players.push_back(CPlayer());
    }
    Scores = (int*)malloc(iNumPlayers*sizeof(int));

    //Run the simulation
    cout << "Running Simulation" << endl;
    for (int i = 0; i < NUM_OF_GENERATIONS; i++) {
        Update(pop, i);
    }

}

void CControllerv2::Update(Population *pop, int generation) {
    int brainCount = 0;
    bool win = false;

    // Evaluate each organism by playing tetris!
    int seed = (unsigned) time(NULL);
    GameScores.clear();
    for (Organism* currentOrganism : pop->organisms) {
        // Insert the brain into each player first
        Players[brainCount].brain.setNetwork(currentOrganism->net);
        Scores[brainCount] = 0;
        GameScores.push_back(vector<int>());
        ++brainCount;
    }

    for(int i = 0; i < gamesPerGeneration; ++i){
        // Play the game on multiple threads
        for (int j = 0; j < (pop->organisms).size(); ++j) {
            Players[j].seed = seed + i;
            Threads.push_back(thread(&CPlayer::playGame, &Players[j]));
        }
        // Get back the results
        for (int j = 0; j < (pop->organisms).size(); ++j) {
            Threads[j].join();
            Scores[j] += Players[j].getFinalScore();
            GameScores[j].push_back(Players[j].getFinalScore());
        }
        Threads.clear();
    }

    // Update fitness of each organism
    brainCount = 0;
    for (Organism* currentOrganism : pop->organisms) {
        // Save the values if we have a new record
        if (Scores[brainCount] > highestScore) {
            highestScore = Scores[brainCount];
            win = true;
        }
        currentOrganism->fitness = (double) Scores[brainCount] / double(gamesPerGeneration);
        ++brainCount;
    }

    // Calculations for saving purposes
    for (Species* currentSpecies : pop->species) {
        currentSpecies->compute_average_fitness();
        currentSpecies->compute_max_fitness();
    }

    // Save and Print if we have a very good one
    if (win) {
        char str[20];
        sprintf(str,"%.1lf(%d).score.txt",(double) highestScore / double(gamesPerGeneration),generation);
        pop->print_to_file_by_species(str);
        FILE *out = fopen(str,"a");
        fprintf(out,"/* Game Results: */\n");
        brainCount = 0;
        for (Organism* currentOrganism : pop->organisms) {
            // Print the results of all the games
            fprintf(out, "/* Organism #%d:", brainCount);
            for(int i: GameScores[brainCount]){
                fprintf(out," %d",i);
            }
            fprintf(out," */\n");
            if (currentOrganism->winner) {
                cout << "/* WINNER IS #" << (currentOrganism->gnome)->genome_id << " */" << endl;
            }
            ++brainCount;
        }
        fclose(out);
    }

    //Run next generation in GA
    pop->epoch(generation);

    //Print timestamp
    time_t _tm = time(NULL);
    struct tm * curtime = localtime ( &_tm );
    cout << "Time: " << asctime(curtime) << endl;

}