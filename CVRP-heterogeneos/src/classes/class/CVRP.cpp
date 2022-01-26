#include "CVRP.hpp"

CVRP::CVRP(int N, int K){
    matrix_distance = (double**) malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        matrix_distance[i] = (double*)malloc(N * sizeof(double));
    }
    matrix_price = (double**) malloc(K * sizeof(double));
    for (int k = 0; k < K; k++){
        matrix_price[k] = (double*)malloc(N * sizeof(double));
    }
}

void CVRP::calculate_matrix_distance(int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if (i == j) {
                matrix_distance[i][j] = 999999.0;
            }
            else {
                matrix_distance[i][j] = distance_euclidian(
                    this->packets[i], this->packets[j]
                );
            }
        }
    }
}
double CVRP::distance_euclidian(Packet origin, Packet destiny){
    double dx = (double)destiny.loc_x - (double)origin.loc_x;
    double dy = (double)destiny.loc_y - (double)origin.loc_y;
    return sqrt(dx * dx + dy * dy);
}
void CVRP::calculate_matrix_price(double alpha, int N, int K){
    //matrix de preço para cada veiculo
    //O preço a se pagar para entregar o packet pelo veiculo K
    for(int i = 0; i < N; i++){
        for(int k = 0; k < K; k++){
            matrix_price[k][i] = price_packet_per_vehicle(
                this->packets[i], this->vehicles[k], alpha
            );
        }
    }
}

Solution CVRP::solveWithKmeans(int timeOrder, int timeVRP, int N, int K){
    // Aplicar o KMeans retornara um vector de clusteres.
    std::vector<Point> coordinates_packets;
    std::vector<Solution> clusters_solved;

    for(int i = 1; i < N; i++){
        coordinates_packets.push_back(packets[i].pt);
    }
    // How to choose the best "k" clusters
    double bestSolutionK = 0.0;
    std::vector<KMeans> possiblesKs;
    for (int i = K/2; i <= K; i++){
        KMeans kmeans(i, N, 2, K);
        kmeans.run(coordinates_packets);
        possiblesKs.push_back(kmeans);
    }
    this->bestK = avaliateBestKmeans(possiblesKs);
    // Quais veículos utilizar para o cluster?
    VehiclePerRegionSolution mapRegion = optimizeVehicles(bestK);
    for(int k = 0; k < this->bestK.getClusters().size(); k++){
        std::vector<Vehicle> vehicles_used = mapRegion.vehiclePerRegion.at(k);
        Solution sol = solve(
            timeOrder,
            timeVRP,
            vehicles_used,
            this->bestKbestK.getCluster(k).getPackets(packets);
        );
        clusters_solved.push_back(sol);
    }
    // A cada cluster a gente tem que resolver um VRP com/sem RF
    for(Solution s : clusters_solved){
        // output é o W_{kj} do CVRP a ser resolvido
        // nova lista de solution
    }
    // Devera unir todas as solutions obtidas para printar corretamente
    // cada veiculo possui sua rota determinada em ordem {JSON?}
}

KMeans CVRP::avaliateBestKmeans(std::vector<KMeans> possiblesKs){
    double s;
    double f;
    double metric = 0.0;
    KMeans bestSolutionK;
    for(KMeans km : possiblesKs){
        s = km.avarageSilhouetteValue();
        f = km.fractionPointClustering();
        result = s*f;
        if(result > metric){
            bestSolutionK = km;
        }
    }
    return km;
}

Solution CVRP::solve(
    int timeOrder, int timeVRP, 
    std::vector<Vehicle> vehicles_used, std::vector<Packet> packs
){
    int N = packs.size();
    int K = vehicles_used.size();    
    //A resolução desse problema retorna os packets organizados por veículos
    Order organizePackets = Order(
        packs,
        vehicles_used,
        matrix_price,
        N, K
    );
    // resolver o problema da "mochila multipla"
    Solution sol = organizePackets.solve(timeOrder);
    // //passar dados para o VRP
    // VRP routing = VRP(partial);
    // //por fim resolver o VRP de cada veiculo 
    // routing.solve();
    return sol;
}

double CVRP::price_packet_per_vehicle(
    Packet& packet, Vehicle& vehicle, double alpha
){
    // distancia a percorrer do deposito ate ele, e os K mais proximos
    // ordenar os pacotes mais proximos do packet
    std::vector<NeighborPacket> sorted = sortPacketsAroundPacket(packet);
    double CAP_PROBABILITY = alpha * vehicle.charge_max;
    double price_dist = distance_euclidian(packets[0], packet);
    vehicle.addPacket(packet);
    int cap = vehicle.current_charge;
    for(NeighborPacket nbp : sorted){
        cap += nbp.getDestination().charge;
        if(cap < CAP_PROBABILITY){
            vehicle.addPacket(nbp.getDestination());
            price_dist += distance_euclidian(
                nbp.getOrigin(), nbp.getDestination());
        } 
    }  
    return (price_dist * vehicle.cust);
}

bool CVRP::crescentePackets(const NeighborPacket& p1, const NeighborPacket& p2){
    return (p1.distance < p2.distance);
}

std::vector<NeighborPacket> CVRP::sortPacketsAroundPacket(Packet pac){
    // ordenar dos pacotes mais proximos do pacote "pac"
    // armazenar em um vetor em ordem crescente as distancias
    int N = packets.size();
    std::vector<NeighborPacket> pkts_bor;
    //somente os pedidos devem aparecer aqui
    for(int i = 1; i < N; i++){
        NeighborPacket nbp = NeighborPacket(pac,packets[i]);
        pkts_bor.push_back(nbp);
    }
    std::sort(pkts_bor.begin(), pkts_bor.end(), crescentePackets);
    return pkts_bor;
}

std::vector<Vehicle> CVRP::optimizeVehicles(KMeans kmeans){
    // Construir uma lista de PacketCentroid
    WhichVehicleProblem pk = WhichVehicleProblem(
        kmeans, packets, vehicles
    );
    Solution optimize = pk.solve();

}