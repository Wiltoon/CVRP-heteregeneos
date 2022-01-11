#include <iostream>
#include <fstream>
#include <string>

#include "classes\class\CVRP.hpp"
#include "SolomonInstance.hpp"

SolomonInstance::SolomonInstance(){
}

void SolomonInstance::readInput(std::string filename){
    std::ifstream arquivo;
    arquivo.open(filename);
    if(!arquivo.is_open()){
        std::cout << "arquivo de entrada inválido" << std::endl;
        exit(0);
    } else {
        this->parse(arquivo);
        arquivo.close();
    }
}

void SolomonInstance::parse(std::ifstream* arquivo){
    std::string line;
    while(!(*arquivo).eof()){
        getline((*arquivo),line);
        std::cout << line << std::endl;
    }
}