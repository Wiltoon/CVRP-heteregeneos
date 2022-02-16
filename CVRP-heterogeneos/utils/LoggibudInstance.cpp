#include "LoggibudInstance.hpp"

LoggibudInstance::LoggibudInstance(){

}

CVRP LoggibudInstance::readInput(
        std::string filename, 
        std::string filevehicle, 
        int N, int K
) {
    std::ifstream stream(filename);
    CVRP problem = CVRP(N);
    if(!stream){ 
        std::cout << "arquivo de entrada inválido" << std::endl;
        exit(0);
    } else {
        this->parse(stream, filevehicle, problem);
        stream.close();
    }
    return problem;
}

void LoggibudInstance::parse(
        std::ifstream& arquivo, 
        std::string filevehicle, 
        CVRP& problem){
    std::string line;
    std::string textJson;
    int cont = 0;
    while(!arquivo.eof()){
        getline(arquivo,line);
        textJson += line; 
    }
    Json::Value root;
    Json::Reader reader;
    reader.parse(textJson, root);

}

std::vector<std::string> LoggibudInstance::split(const std::string& text, char sep){
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