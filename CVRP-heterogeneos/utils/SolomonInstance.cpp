#include <iostream>
#include <fstream>
#include <string>

#include "classes/class/CVRP.hpp"
#include "SolomonInstance.hpp"

SolomonInstance::SolomonInstance(){
};

int SolomonInstance::readInput(char* filename){
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

CVRP SolomonInstance::parse(std::ifstream arquivo){
    CVRP problem;
    std::cout << arquivo << std::endl;
    return problem;
};