#include "main.h"
#include "player/CPlayer.h"
#include "controller/CControllerv2.h"

int main() {
    srand( (unsigned)time( NULL ) );

    //Initialise parameters
    NEAT::load_neat_params("../params.ne",true);

    //Initialise Controller
    CControllerv2 tetrisAI = CControllerv2();
    return 0;
}