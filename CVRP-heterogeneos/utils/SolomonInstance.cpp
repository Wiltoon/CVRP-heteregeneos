#include <iostream>
#include <fstream>
#include <string>

#include "classes/class/CVRP.hpp"
#include "SolomonInstance.hpp"

SolomonInstance::SolomonInstance(){
    CVRP readInput(std::string filename){
        ifstream arquivo;
        arquivo.open(filename);
        if(!arquivo.is_open){
            std::cout << "arquivo de entrada inválido" << std::endl;
            exit(0);
        } else {
            CVRP problem = parse(arquivo);
            arquivo.close();
        }
    }

    CVRP parse(ifstream arquivo){
        CVRP problem;
        std::cout << std::endl;
    }
}