#ifndef VRPSOLUTION_H_INCLUDED
#define VRPSOLUTION_H_INCLUDED

#pragma once

#include <iostream>
#include "../Packet.hpp"
#include "../Vehicle.hpp"

#include <ilcplex/ilocplex.h>

class VRPSolution {
public:
	std::string nameInstance;
	IloArray <IloArray <IloNumArray>> xSol;
    IloArray <IloNumArray> uSol;
	IloArray <IloNumArray> zSol;
	std::vector<Vehicle> vehicles;
	std::vector<Packet> deliveries;

	int DEPOSIT = 0;
	int N = 0;

	VRPSolution();
	VRPSolution(
		IloArray <IloArray <IloNumArray>> & x,
		IloArray <IloNumArray> & u,
		std::vector<Vehicle> vehicles,
		std::vector<Packet> deliveries
	);
	void buildSolution();
	void printerSolution();
	std::string textJsonVRPSolution();
};

#endif // VRP_SOLUTION_H_INCLUDED