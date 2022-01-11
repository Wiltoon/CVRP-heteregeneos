#include "Vehicle.hpp"

void Vehicle::addPacket(Packet packet){
    packets_order.push_back(packet);
}

void Vehicle::useVehicle(bool use){
    used = true;
}