#include "Vehicle.hpp"

Vehicle::Vehicle(int id, int cap, int cust, Packet deposit){
    this->id = id;
    this->charge_max = cap;
    this->cust = cust;
    this->origin = deposit;
    this->used = false;
}

void Vehicle::addPacket(Packet packet){
    deliveries.push_back(packet);
    this->current_charge += packet.charge;
}

void Vehicle::useVehicle(bool use){
    used = true;
}