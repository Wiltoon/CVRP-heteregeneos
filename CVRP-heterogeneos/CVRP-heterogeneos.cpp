#include <iostream>
#include "utils\SolomonInstance.hpp"
#include "classes/class/CVRP.hpp"
#include "src\CVRPSolution.hpp"
#include "CVRP-heterogeneos.h"
#include <string>

using namespace std;
// ALPHA é a porcentagem da capacidade maxima de um veiculo que será ativada 
// para o calculo do preço de um pedido

int main()
{
    double ALPHA = 0.85;    // deve ser entre 0 e 1
    int N = 101;            // Número de Packets
    int K = 22;             // Número de veículos
    int timeOrder = 20;     // Tempo para resolver ORDER
    int timeVRP = 20;       // Tempo para resolver o VRP
    string filename("resource\\Solomon\\c101-0.0.txt");
    SolomonInstance reader;
    CVRP problem = reader.readInput(filename, ALPHA, N, K);
    Solution solution = problem.solve(timeOrder,timeVRP);
    solution.partial.printerOrderSolution(N,K);
    return 0;
}
