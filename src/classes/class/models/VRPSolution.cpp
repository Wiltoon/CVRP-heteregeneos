#include "VRPSolution.hpp"

VRPSolution::VRPSolution() {

}

VRPSolution::VRPSolution(
	IloArray <IloArray <IloNumArray>> & x,
	std::string nameInstance,
	IloArray <IloNumArray> & u,
	std::vector<Vehicle> vehicles,
	std::vector<Packet> deliveries
) {
	this->nameInstance = nameInstance;
	this->xSol = x;
	this->uSol = u;
	this->vehicles = vehicles;
	this->deliveries = deliveries;
	buildSolution();
}

VRPSolution::VRPSolution(
	IloArray <IloArray <IloNumArray>> & x,
	IloArray <IloNumArray> & u,
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
		origin = DEPOSIT;
		// std::cout << "k: " << std::to_string(k) << std::endl;
		for(int dest = 0; dest < deliveries.size(); dest++) {
			// std::cout << xSol[k][origin][dest] << ",\t";
			if(xSol[k][origin][dest] >= 0.9) {
				vehicles[k].addPacket(deliveries[dest]);
				origin = dest;
				dest = DEPOSIT-1;
				// std::cout << std::endl;
			}
		}
		// std::cout << std::endl;
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
			if(p.id != 0){
				textJson += "\t\t\t\t{\n";
				textJson += "\t\t\t\t\t\"id\": "+ p.id_s + ",\n";
				textJson += "\t\t\t\t\t\"point\": {\n";
				textJson += "\t\t\t\t\t\t\"lng\": " + std::to_string(p.loc_x) + ",\n";
				textJson += "\t\t\t\t\t\t\"lat\": " + std::to_string(p.loc_y) + "\n";
				textJson += "\t\t\t\t\t},\n";
				textJson += "\t\t\t\t\t\"size\": " + std::to_string(p.charge) + "\n";
				textJson += "\t\t\t\t}\n";
			}
		}
		textJson += "\t\t\t]\n";
		textJson += "\t\t}\n";
	}
	textJson += "\t]\n";
	textJson += "}\n";
	return textJson;
}