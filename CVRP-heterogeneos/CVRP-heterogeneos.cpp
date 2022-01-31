#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// #include "libs/matplotlibcpp.h"

#include "CVRP-heterogeneos.h"
#include "src/KMeans.hpp"
#include "utils/SolomonInstance.hpp"

using namespace std;
// namespace plt = matplotlibcpp;
// ALPHA é a porcentagem da capacidade maxima de um veiculo que será ativada 
// para o calculo do preço de um pedido

int main()
{
    double ALPHA = 0.5;    // deve ser entre 0 e 1
    int N = 101;            // Número de Packets
    int K = 10;             // Número de veículos
    int timeOrder = 20;     // Tempo para resolver ORDER
    int timeVRP = 20;       // Tempo para resolver o VRP
    string filename("resource\\Solomon\\c101-0.0.txt");
    string filevehicle("resource\\Solomon\\vei-homo.txt");
    SolomonInstance reader = SolomonInstance();
    CVRP problem = reader.readInput(filename, filevehicle, N, K);

    Solution solution = problem.solveWithKmeans(
        timeOrder,timeVRP, N, K, ALPHA);
    // solution.partial.printerOrderSolution(N, K);
    return 0;
}
