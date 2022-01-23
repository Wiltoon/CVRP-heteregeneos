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

void OrderSolution::printerOrderSolution(int N, int K){
    std::map<int, std::vector<int>> organizeVehicles;
    std::vector<int> bagVehicle;
    for(int k = 0; k < K; k++){
        for(int i = 0; i < N; i++){
            if (output[k][i] > 0.9) {
                bagVehicle.push_back(i);
            }
        }
        if (!bagVehicle.empty()) {
            organizeVehicles[k] = bagVehicle;
            bagVehicle.clear();
            std::cout << "Vehicle: " << k << std::endl;
            std::cout << "Capacity Used: ";
            int capUsed = 0;
            for (int j = 0; j < organizeVehicles.at(k).size(); j++) {
                capUsed += packets[organizeVehicles.at(k)[j]].charge;
            }
            std::cout << capUsed << "/" << vehicles[k].charge_max << std::endl;
            std::cout << "Packets: [ ";
            for (int j = 0; j < organizeVehicles.at(k).size(); j++) {
                if (j < (organizeVehicles.at(k).size() - 1)) {
                    std::cout << organizeVehicles.at(k)[j] << ", ";
                }
                else {
                    std::cout << organizeVehicles.at(k)[j] << "]" << std::endl << std::endl;
                }
            }
        }
    }
}