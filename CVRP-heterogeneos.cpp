#include <stdio.h>
#include <time.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

// #include "libs/matplotlibcpp.h"
#include <json/json.h>

#include "CVRP-heterogeneos.h"
#include "src/KMeans.hpp"
#include "utils/SolomonInstance.hpp"
#include "utils/LoggibudInstance.hpp"
#include "src/classes/class/Packet.hpp"


using namespace std;
// namespace plt = matplotlibcpp;
// ALPHA é a porcentagem da capacidade maxima de um veiculo que será ativada 
// para o calculo do preço de um pedido

int main()
{
    double ALPHA = 0.3;     // deve ser entre 0 e 1
    int N;            // Número de Packets
    int K = 50;             // Número de veículos
    int timeOrder = 20;     // Tempo para resolver ORDER
    int timeVRP = 20;       // Tempo para resolver o VRP
    time_t start, end;
    for(auto& p : filesystem::directory_iterator("resource\\Loggibud\\KMeans")){
        cout << p << endl; 
    }

    string nameInstance("cvrp-0-pa-0");
    string filekmeans(
        "resource\\Loggibud\\KMeans\\"+nameInstance+"-kmeans.json"
    );
    // string filename("resource\\Solomon\\c101-0.0.txt");
    string filename("resource\\Loggibud\\"+nameInstance+".json");
    string filevehicle("resource\\Solomon\\vei-homo.txt");
    ifstream stream(filename);
    string line;
    string textJson;
    while(!stream.eof()){
        getline(stream,line);
        textJson += line; 
    }
    Json::Value root;
    Json::Reader readerJson;
    readerJson.parse(textJson, root);
    const Json::Value deliveries = root["deliveries"];
    LoggibudInstance reader = LoggibudInstance();
    N = deliveries.size()+1;
    CVRP problem = reader.readInput(filename, filevehicle, N, K);
    problem.solveKmeansParallel(
        filekmeans, 
        timeOrder,
        timeVRP,
        ALPHA,
        nameInstance
    );
    problem.outputJson(problem.clusters_solved, nameInstance);
    // Solution solution = problem.solveWithKmeans(
    //     timeOrder,timeVRP, N, K, ALPHA, root["name"].asString()
    // );
    return 0;
}
