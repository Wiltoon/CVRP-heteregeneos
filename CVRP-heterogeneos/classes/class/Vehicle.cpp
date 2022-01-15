#include "Vehicle.hpp"

Vehicle::Vehicle(int id, int cap, int cust){
    this->id = id;
    this->charge_max = cap;
    this->cust = cust;
    this->used = false;
}

void Vehicle::addPacket(Packet packet){
    packets_order.push_back(packet);
    this->current_charge += packet.charge;
}

void Vehicle::useVehicle(bool use){
    used = true;
}