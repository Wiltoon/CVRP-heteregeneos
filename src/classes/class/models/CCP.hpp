#ifndef CCP_H_INCLUDED
#define CCP_H_INCLUDED

#pragma once

#include "Model.hpp"

class CCP final: public Model {
private:
    std::vector<Packet> deliveries;
    std::vector<Vehicle> vehicles;
public:
    double** matrix_distance;

    static double euclidean(Point p1, Point p2);
    void generate_solution(std::string nameFile);
};