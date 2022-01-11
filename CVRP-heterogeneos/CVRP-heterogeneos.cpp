#include <iostream>
#include "utils\SolomonInstance.hpp"
#include "classes\class\CVRP.hpp"
#include "src\CVRPSolution.hpp"
#include "CVRP-heterogeneos.h"
#include <string>

int main()
{
    char filename[] = "resource/Solomon/c101-0.0.txt";
    SolomonInstance reader;
    int i = reader.readInput(filename);
    //CVRPSolution solution = problem.solve();
    return 0;
}
