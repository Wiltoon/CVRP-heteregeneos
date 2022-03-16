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
        problem.calculate_matrix_distance(N,true);
        for(int k = 0; k < K; k++){
            Vehicle vehicle = Vehicle(
                k,
                capacity_homogeneous,
                30,
                problem.packets[0]
            );
            problem.vehicles.push_back(vehicle);
        }
        stream.close();
    }
    return problem;
}

void LoggibudInstance::parse(
        std::ifstream& arquivo, 
        std::string filevehicle,
        int n_lcr, 
        CVRP& problem){
    std::string line;
    std::string textJson;
    while(!arquivo.eof()){
        getline(arquivo,line);
        textJson += line; 
    }
    Json::Value root;
    Json::Reader reader;
    reader.parse(textJson, root);
    nameInstance = root["name"].asString();
    region = root["region"].asString();
    capacity_homogeneous = root["vehicle_capacity"].asInt();
    Packet deposit = Packet(
        0, 
        root["origin"]["lng"].asDouble(),
        root["origin"]["lat"].asDouble(),
        0
    );
    problem.packets.push_back(deposit);
    const Json::Value deliveries = root["deliveries"];
    int N = deliveries.size();
    for(int i = 0; i < N; i++){
        int id = i+1;
        Packet packet = Packet(
            id,
            deliveries[i]["id"].asString(), 
            deliveries[i]["point"]["lng"].asDouble(), 
            deliveries[i]["point"]["lat"].asDouble(),
            deliveries[i]["size"].asInt() 
        );
        problem.packets.push_back(packet);
    }
    for(int i = 1; i <= N; i++){
        calculate_neighbors(lcr, i, problem);
    }
}

void LoggibudInstance::calculate_neighbors(
    int n_neighbors,
    int index_pac, 
    CVRP& problem
    ){
    // calcular distancia do deposito até o pacote [0][index_pac]
    distanceMax = matrix_distance[0][index_pac];
    std::vector<int> neighbors;
    for(int j = 1; j <= N; j++){
        if(distanceMax >= matrix_distance[index_pac][j]){
            neighbors.push_back(j);
        }
    }
    std::sort(neighbors.begin(), neighbors.end());
    for(int i = 0; i < n_neighbors && i < neighbors.size(); i++){
        problem.packets[index_pac].neighbors.push_back(j);
    }
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