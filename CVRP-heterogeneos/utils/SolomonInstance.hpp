#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "classes\class\CVRP.hpp"


class SolomonInstance{
private:
    std::vector<Packet> packets;
public:
    SolomonInstance(){}
    CVRP readInput(std::string filename){
        std::ifstream arquivo;
        CVRP problem;
        arquivo.open(filename);
        if(!arquivo.is_open()){
                std::cout << "arquivo de entrada inválido" << std::endl;
                exit(0);
        } else {
            this->parse(&arquivo, &problem);
            // problem.calculate_matrix_distance();
            // problem.calculate_matrix_price();
            // arquivo.close();
        }
        return problem;
    };

    void parse(std::ifstream* arquivo, CVRP* problem){
        std::string line;
        int num_lim = 0;
        while(!(*arquivo).eof()){
            getline((*arquivo),line);
            if(num_lim != 0){
                int id;
                int loc_x;
                int loc_y;
                int cap;
                std::vector<std::string> values = split(line,',');
                int element = 0;
                for(std::string str : values){
                    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
                    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
                    switch(element){
                        case 0:
                            id = stoi(str);
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
        }
    };
    std::vector<std::string> split(const std::string& text, char sep)
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
    };
};