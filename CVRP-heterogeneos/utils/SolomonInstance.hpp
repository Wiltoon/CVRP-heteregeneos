#pragma once
#include <iostream>

#include "classes/class/CVRP.hpp"
#include <fstream>

class SolomonInstance {
public:
    SolomonInstance() {};
    int readInput(std::string filename){
        std::ifstream arquivo;
        arquivo.open(filename);
        if(!arquivo.is_open){
            std::cout << "arquivo de entrada inválido" << std::endl;
            exit(0);
        } else {
            CVRP problem = parse(arquivo);
            arquivo.close();
        }
        return 0;
    };
    CVRP parse(std::ifstream content);
};