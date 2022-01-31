#include "CVRP.hpp"

CVRP::CVRP() {
}

CVRP::CVRP(int N){
    matrix_distance = (double**) malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        matrix_distance[i] = (double*)malloc(N * sizeof(double));
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

Solution CVRP::solveWithKmeans(
    int timeOrder, int timeVRP, int N, int K, double alpha){
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
        int Nfit = N-1;
        KMeans kmeans = KMeans(i, Nfit, 2, 15);
        kmeans.run(coordinates_packets);
        possiblesKs.push_back(kmeans);
    }
    this->bestK = avaliateBestKmeans(possiblesKs);
    // Quais veículos utilizar para o cluster?
    VehiclePerRegionSolution mapRegion = optimizeVehicles(bestK);
    for(int k = 0; k < this->bestK.getClusters().size(); k++){
        std::vector<Vehicle> vehicles_used = mapRegion.vehiclePerRegion.at(k);
        std::vector<Packet> p_visited_region = 
            this->bestK.getCluster(k).getPackets(packets);
        Solution sol = solve(
            timeOrder,
            timeVRP,
            vehicles_used,
            p_visited_region,
            alpha
        );
        clusters_solved.push_back(sol);
    }

    std::cout << "PARTE PRINT 3" << std::endl;
    for(int k = 0; k < this->bestK.getClusters().size(); k++){
        std::vector<Vehicle> vehicles_used = mapRegion.vehiclePerRegion.at(k);
        std::vector<Packet> p_visited_region = 
            this->bestK.getCluster(k).getPackets(packets);
        clusters_solved[k].partial.printerOrderSolutionGlobal(
            vehicles_used,
            this->bestK.getCluster(k).getPackets(packets)
        );

    }
    // Devera unir todas as solutions obtidas para printar corretamente
    // cada veiculo possui sua rota determinada em ordem {JSON?}
}

void CVRP::printerPackets(std::vector<Packet> packs){
    std::cout << "vet = [ ";
    for(Packet p : packs){
        std::cout << p.id << " "; 
    }
    std::cout << "]!" << std::endl;
}

KMeans CVRP::avaliateBestKmeans(std::vector<KMeans> possiblesKs){
    double s;
    double f;
    double metric = 0.0;
    double result;
    KMeans bestSolutionK = KMeans();
    for(KMeans km : possiblesKs){
        s = km.avarageSilhouetteValue();
        f = km.fractionPointClustering();
        result = s * f;
        if(result > metric){
            bestSolutionK = km;
        }
    }
    bestSolutionK.printerClusters();
    return bestSolutionK;
}

Solution CVRP::solve(
    int timeOrder, int timeVRP, 
    std::vector<Vehicle> vehicles_used, std::vector<Packet> packs, double alpha
){
    int Nsub = packs.size();
    int Ksub = vehicles_used.size();    
    //A resolução desse problema retorna os packets organizados por veículos
    Order organizePackets = Order(
        packs,
        vehicles_used,
        alpha
    );
    // resolver o problema da "mochila multipla"
    Solution sol = organizePackets.solve(timeOrder);
    
    VRP vrpRelaxFix = VRP(sol.partial.output, vehicles_used, packs);
    return sol;
}

VehiclePerRegionSolution CVRP::optimizeVehicles(KMeans kmeans){
    // Construir uma lista de PacketCentroid
    WhichVehicleProblem pk = WhichVehicleProblem(
        kmeans, packets, vehicles
    );
    Solution optimize = pk.solve(25);
    return optimize.whichvehicles;
}