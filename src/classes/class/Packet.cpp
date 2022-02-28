#include "Packet.hpp"

Packet::Packet(){
	
}

Packet::Packet(int id, double loc_x, double loc_y, int charge) {
	this->id = id;
	this->loc_x = loc_x;
	this->loc_y = loc_y;
	this->charge = charge;
	std::vector<double> coordinates;
	coordinates.push_back(loc_x);
	coordinates.push_back(loc_y);
	this->pt = Point(id, coordinates, "Packet_"+std::to_string(id));
}

Packet::Packet(int id, std::string id_s, double loc_x, double loc_y, int charge) {
	this->id = id;
	this->id_s = id_s;
	this->loc_x = loc_x;
	this->loc_y = loc_y;
	this->charge = charge;
	std::vector<double> coordinates;
	coordinates.push_back(loc_x);
	coordinates.push_back(loc_y);
	this->pt = Point(id, coordinates, "Packet_"+id);
}
