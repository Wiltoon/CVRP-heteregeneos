#include "VehiclePerRegionSolution.hpp"

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

    for(int r = 0; r < total_regions; r++){
        for(int v = 0; v < vehicles.size(); v++){
            if(output[r][v] >= 0.9){
                //ver funcionamento do map
                vehiclePerRegion[r].push_back(vehicles[v]);
            }
        }
    }
}

VehiclePerRegion::VehiclePerRegionSolution(std::string message){
    this->message = message;
}

