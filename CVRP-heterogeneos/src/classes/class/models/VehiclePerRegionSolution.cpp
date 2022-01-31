#include "VehiclePerRegionSolution.hpp"

VehiclePerRegionSolution::VehiclePerRegionSolution(){

}

VehiclePerRegionSolution::VehiclePerRegionSolution(
        IloArray <IloNumArray> output,
        std::string message,
        std::vector<Packet> packets,
        std::vector<Vehicle> vehicles,
        int total_regions
){
    this->output = output;
    this->message = message;
    this->packets = packets;
    this->vehicles = vehicles;
    this->total_regions = total_regions;

    std::cout << "qtd Vehicles: " << vehicles.size() << std::endl;
    std::cout << "[";
    for(int r = 0; r < total_regions; r++){
        for(int v = 0; v < vehicles.size(); v++){
            std::cout << "\t" << vehicles[v].id << ",\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "[";
    for(int r = 0; r < total_regions; r++){
        for(int v = 0; v < vehicles.size(); v++){
            std::cout << "\t" << output[r][v] << ",\t";
            if(output[r][v] >= 0.9){
                //ver funcionamento do map
                vehiclePerRegion[r].push_back(vehicles[v]);
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

VehiclePerRegionSolution::VehiclePerRegionSolution(std::string message){
    this->message = message;
}

