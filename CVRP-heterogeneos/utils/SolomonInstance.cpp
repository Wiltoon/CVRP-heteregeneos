#include <iostream>
#include <fstream>
#include <string>

#include "..\classes\class\CVRP.hpp"
#include "SolomonInstance.hpp"

SolomonInstance::SolomonInstance(){
}

CVRP SolomonInstance::readInput(std::string filename) {
    std::ifstream arquivo;
    CVRP problem = CVRP(102);
    arquivo.open(filename);
    if (!arquivo.is_open()) {
        std::cout << "arquivo de entrada inválido" << std::endl;
        exit(0);
    }
    else {
        this->parse(&arquivo, &problem);
        problem.calculate_matrix_distance();
        /*for (int i = 0; i < problem.packets.size(); i++) {
            for (int j = 0; j < problem.packets.size(); j++) {
                std::cout << problem.matrix_distance[i][j] << " ";
            }
            std::cout << std::endl;
        }*/
        // problem.calculate_matrix_price();
        // arquivo.close();
    }
    return problem;
}

void SolomonInstance::parse(std::ifstream* arquivo, CVRP* problem) {
    std::string line;
    int num_lim = 0;
    int id = 0;
    int loc_x = 0;
    int loc_y = 0;
    int cap = 0;
    while (!(*arquivo).eof()) {
        getline((*arquivo), line);
        if (num_lim != 0) {
            std::vector<std::string> values = split(line, ',');
            int element = 0;
            for (std::string str : values) {
                str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
                str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
                str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
                switch (element) {
                case 0:
                    //std::cout << "str: " << str << std::endl;
                    if (str.size() != 0) {
                        id = stoi(str);
                    }
                    break;
                case 1:
                    loc_x = stoi(str);
                    break;
                case 2:
                    loc_y = stoi(str);
                    break;
                case 3:
                    cap = stoi(str);
                    break;
                default:
                    break;
                }
                element++;
            }
            Packet packet = Packet(id, loc_x, loc_y, cap);
            (problem)->packets.push_back(packet);
        }
        num_lim++;
        //std::cout << "size: " << (problem)->packets.size() << std::endl;
    }
}
std::vector<std::string> SolomonInstance::split(const std::string& text, char sep)
{
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;

    while ((end = text.find(sep, start)) != std::string::npos)
    {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }

    tokens.push_back(text.substr(start));
    return tokens;
}
