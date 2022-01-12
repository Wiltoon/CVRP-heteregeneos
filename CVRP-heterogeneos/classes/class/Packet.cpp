#include "Packet.hpp"
#include <iostream>

Packet::Packet(int id, int loc_x, int loc_y, int charge)
{
	this->id = id;
	this->loc_x = loc_x;
	this->loc_y = loc_y;
	this->charge = charge;
}
