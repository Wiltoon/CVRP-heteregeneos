#include "Vehicle.hpp"

void Vehicle::addPacket(Packet packet){
    packets_order.push_back(packet);
    this->current_charge += packet.charge;
}

void Vehicle::useVehicle(bool use){
    used = true;
}