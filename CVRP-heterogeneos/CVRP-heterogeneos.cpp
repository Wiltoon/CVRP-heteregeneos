#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "CVRP-heterogeneos.h"
#include "utils/SolomonInstance.hpp"

using namespace std;
// ALPHA é a porcentagem da capacidade maxima de um veiculo que será ativada 
// para o calculo do preço de um pedido

int main()
{
    double ALPHA = 0.20;    // deve ser entre 0 e 1
    int N = 101;            // Número de Packets
    int K = 11;             // Número de veículos
    int timeOrder = 20;     // Tempo para resolver ORDER
    int timeVRP = 20;       // Tempo para resolver o VRP
    string filename("resource\\Solomon\\c101-0.0.txt");
    string filevehicle("resource\\Solomon\\vei-homo.txt");
    SolomonInstance reader = SolomonInstance();
    CVRP problem = reader.readInput(filename, filevehicle, ALPHA, N, K);
    Solution solution = problem.solve(timeOrder,timeVRP, N, K);
    solution.partial.printerOrderSolution(N, K);
    return 0;
}
