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
    double ALPHA = 0.3;         // deve ser entre 0 e 1
    int N;                      // Número de Packets
    int K = 50;                 // Número de veículos
    int timeOrder = 20;         // Tempo para resolver ORDER
    int timeVRP = 20;           // Tempo para resolver o VRP
    int totalDays = 90;         // Dias percorridos
    time_t start, end;
    /*for(auto& p : filesystem::directory_iterator("resource\\Loggibud\\KMeans")){
        cout << p << endl; 
    }*/
    string city("pa-");
    string nameLocalInstance("cvrp-0-"+city);
    for (int day = 62; day < totalDays; day++) {
        string nameInstance(nameLocalInstance + to_string(day));
        string filekmeans(
            "resource\\Loggibud\\KMeans\\"+nameInstance+"-kmeans.json"
        );
        // string filename("resource\\Solomon\\c101-0.0.txt");
        string filename("resource\\Loggibud\\cvrp-instances-1.0\\train\\pa-0\\"+nameInstance+".json");
        string filevehicle("resource\\Solomon\\vei-homo.txt");
        ifstream stream(filename);
        string line;
        string textJson("");
        int ittt = 0;
        cout << filename << endl;
        while(!stream.fail()){
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
        double time_execution = problem.solveRF();
        // double time_execution = problem.solveKmeansSeriable(
        //     filekmeans, 
        //     timeOrder,
        //     timeVRP,
        //     ALPHA,
        //     nameInstance
        // );
        problem.outputJson(problem.clusters_solved, nameInstance, time_execution);
    }
    // Solution solution = problem.solveWithKmeans(
    //     timeOrder,timeVRP, N, K, ALPHA, root["name"].asString()
    // );
    return 0;
}
