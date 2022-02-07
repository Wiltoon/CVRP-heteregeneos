#include "VRPSolution.hpp"

VRPSolution::VRPSolution() {

}

VRPSolution::VRPSolution(
	IloArray <IloArray <IloNumArray>> x,
	IloArray <IloNumArray> u,
	std::vector<Vehicle> vehicles,
	std::vector<Packet> deliveries
) {
	this->xSol = x;
	this->uSol = u;
	this->vehicles = vehicles;
	this->deliveries = deliveries;
	buildSolution();
}

void VRPSolution::buildSolution() {
	std::cout << "Build Solution" << std::endl;
	int origin = DEPOSIT;
	for(int k = 0; k < vehicles.size(); k++) {
		for(int dest = 0; dest < deliveries.size(); dest++) {
			if(this->xSol[k][origin][dest] >= 0.9) {
				vehicles[k].addPacket(deliveries[dest]);
				origin = dest;
				dest = DEPOSIT;
			}
		}
	}
}

void VRPSolution::printerSolution(){
	std::cout << "Printer Solution =>" << std::endl;
	for(Vehicle v : vehicles) {
		std::cout << "\tVehicle => " << v.id << std::endl;
		std::cout << "\tDeliveries => " << v.id << std::endl;
		std::cout << "\t\t[ " << DEPOSIT << ", "; 
		for(Packet p : v.deliveries){
			std::cout << p.id << ", ";
		}
		std::cout << "]" << std::endl;
	}
}

std::string VRPSolution::textJsonVRPSolution() {
	std::string textJson;
	textJson = "{\n";
	textJson += "\t\"name\": \"" + nameInstance + "\"\n";
	textJson += "\t\"vehicles\": [\n";
	for(Vehicle v : vehicles) {
		textJson += "\t\t{\n";
		textJson += "\t\t\t\"origin\": {\n";
		textJson += "\t\t\t\t\"lng\": " + std::to_string(v.origin.loc_x) + ",\n";
		textJson += "\t\t\t\t\"lat\": " + std::to_string(v.origin.loc_y) + "\n";
		textJson += "\t\t\t}\n";
		textJson += "\t\t\t\"deliveries\": [\n";
		for(Packet p : v.deliveries) {
			textJson += "\t\t\t\t{\n";
			textJson += "\t\t\t\t\t\"id\": "+ std::to_string(p.id) + ",\n";
			textJson += "\t\t\t\t\t\"point\": {\n";
			textJson += "\t\t\t\t\t\t\"lng\": " + std::to_string(p.loc_x) + ",\n";
			textJson += "\t\t\t\t\t\t\"lat\": " + std::to_string(p.loc_y) + "\n";
			textJson += "\t\t\t\t\t},\n";
			textJson += "\t\t\t\t\t\"size\": " + std::to_string(p.charge) + "\n";
			textJson += "\t\t\t\t}\n";
		}
		textJson += "\t\t\t]\n";
		textJson += "\t\t}\n";
	}
	textJson += "\t]\n";
	textJson += "}\n";
	return textJson;
}