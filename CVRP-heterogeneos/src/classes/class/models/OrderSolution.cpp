#include <map>

#include "OrderSolution.hpp"

OrderSolution::OrderSolution(
    IloArray <IloNumArray> output, 
    std::string message, 
    std::vector<Packet> packets,
    std::vector<Vehicle> vehicles
) {
    this->output = output;
    this->message = message;
    this->packets = packets;
    this->vehicles = vehicles;
}

OrderSolution::OrderSolution(IloArray <IloNumArray> output, std::string message){
    this->output = output;
    this->message = message;
}

OrderSolution::OrderSolution(std::string messageFail){
    this->message = messageFail;
}

void OrderSolution::printerOrderSolutionGlobal(
    std::vector<Vehicle> g_vehicles,
    std::vector<Packet> g_packets_k
){
    int K = g_vehicles.size();
    int N = g_packets_k.size();
    std::map<int, std::vector<Packet>> organizeVehicles;
    std::vector<Packet> bagVehicle;
    for(int k = 0; k < K; k++) {
        for(int i = 0; i < N; i++){
            if (output[k][i] > 0.9) {
                bagVehicle.push_back(g_packets_k[i]);
            }
        }
        if (!bagVehicle.empty()) {
            organizeVehicles[k] = bagVehicle;
            std::cout << "Vehicle: " << g_vehicles[k].id << std::endl;
            std::cout << "Capacity Used: ";
            int capUsed = 0;
            for (int j = 0; j < organizeVehicles.at(k).size(); j++) {
                capUsed += organizeVehicles.at(k)[j].charge;
            }
            bagVehicle.clear();
            std::cout << capUsed << "/" << g_vehicles[k].charge_max << std::endl;
            std::cout << "Packets: [ ";
            for (int j = 0; j < organizeVehicles.at(k).size(); j++) {
                if (j < (organizeVehicles.at(k).size() - 1)) {
                    std::cout << organizeVehicles.at(k)[j].id << ", ";
                }
                else {
                    std::cout << organizeVehicles.at(k)[j].id << "]" << std::endl << std::endl;
                }
            }
        }
    }
}

void OrderSolution::printerOrderSolution(int N, int K){
    std::map<int, std::vector<Packet>> organizeVehicles;
    std::vector<Packet> bagVehicle;
    for(int k = 0; k < K; k++){
        for(int j = 0; j < N; j++){
            if (output[k][j] > 0.9) {
                bagVehicle.push_back(packets[j]);
            }
        }
        if (!bagVehicle.empty()) {
            organizeVehicles[k] = bagVehicle;
            bagVehicle.clear();
            std::cout << "Vehicle: " << k << std::endl;
            std::cout << "Capacity Used: ";
            int capUsed = 0;
            for (int j = 0; j < organizeVehicles.at(k).size(); j++) {
                capUsed += packets[organizeVehicles.at(k)[j].id].charge;
            }
            std::cout << capUsed << "/" << vehicles[k].charge_max << std::endl;
            std::cout << "Packets: [ ";
            for (int j = 0; j < organizeVehicles.at(k).size(); j++) {
                if (j < (organizeVehicles.at(k).size() - 1)) {
                    std::cout << organizeVehicles.at(k)[j].id << ", ";
                }
                else {
                    std::cout << organizeVehicles.at(k)[j].id << "]" << std::endl << std::endl;
                }
            }
        }
    }
}