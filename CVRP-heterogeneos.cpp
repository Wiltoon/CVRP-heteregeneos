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
    int K = 250;                 // Número de veículos
    int timeOrder = 10;         // Tempo para resolver ORDER
    int timeVRP = 10;           // Tempo para resolver o VRP
    int totalDays = 3;         // Dias percorridos
    int totalDays = 24;         // Dias percorridos
    int instanceInit = 90;
    int instanceEnd = 120;
    
    vector<string> cities;
    time_t start, end;
    /*for(auto& p : filesystem::directory_iterator("resource\\Loggibud\\KMeans")){
        cout << p << endl; 
    }*/
    cities.push_back("pa-");
    // cities.push_back("df-");
    // cities.push_back("rj-");
    for(string city : cities){
        string nameLocalInstance("cvrp-0-"+city);
        // int DD = (instanceInit +totalDays);
        int DD = instanceEnd;
        for (int day = instanceInit; day < DD; day++) {
            try {
                string nameInstance(nameLocalInstance + to_string(day));
                string filekmeans(
                    "resource\\Loggibud\\KMeans\\" + city + "0\\" + nameInstance + "-kmeans.json"
                );
                // string filename("resource\\Solomon\\c101-0.0.txt");
                string filename("resource\\Loggibud\\cvrp-instances-1.0\\dev\\" + city + "0\\" + nameInstance + ".json");
                string filevehicle("resource\\Solomon\\vei-homo.txt");
                ifstream stream(filename);
                string line;
                string textJson("");
                int ittt = 0;
                cout << filename << endl;
                while (!stream.fail()) {
                    getline(stream, line);
                    textJson += line;
                }
                Json::Value root;
                Json::Reader readerJson;
                readerJson.parse(textJson, root);
                const Json::Value deliveries = root["deliveries"];
                LoggibudInstance reader = LoggibudInstance();
                N = deliveries.size() + 1;
                CVRP problem = reader.readInput(filename, filevehicle, N, K);
                double time_execution = problem.solveKmeansSeriableMIP(
                    filekmeans,
                    timeOrder,
                    timeVRP,
                    ALPHA,
                    nameInstance,
                    problem.matrix_distance
                );
                problem.outputJson(
                    city,
                    problem.clusters_solved,
                    nameInstance,
                    time_execution
                );
            }
            catch (exception& e) {
                cout << e.what() << endl;
            }
        }
    }
    // Solution solution = problem.solveWithKmeans(
    //     timeOrder,timeVRP, N, K, ALPHA, root["name"].asString()
    // );
    return 0;
}
