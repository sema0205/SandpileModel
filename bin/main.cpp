
#include "lib/headers/image.h"
#include "lib/headers/sandpile.h"

int main(int argc, char* argv[]) {

    // create a struct of specific sandpile
    Sandpile sandpile;
    sandpile.SetVariables(argc,argv);

    // add barriers
    Barrier barrier1 = {200,300,200,300};
    Barrier barrier2 = {200,300,600,700};
    Barrier barrier3 = {600,700,200,300};

    sandpile.barrier_massive = new Barrier[3];
    sandpile.barrier_massive_size = 3;

    // if you want 0 barriers - just leave this string sandpile.barrier_massive_size = 0; and comment others lines of code
    sandpile.barrier_massive[0] = barrier1;
    sandpile.barrier_massive[1] = barrier2;
    sandpile.barrier_massive[2] = barrier3;

    sandpile.MakeSandpile();

    return 0;
}
