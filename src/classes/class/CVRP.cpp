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

static size_t WriteCallback(char* contents, size_t size, size_t nmemb, char* buffer_in)
{
    ((std::string*)buffer_in)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void CVRP::calculate_matrix_distance(int N, bool loggibud){
    if(loggibud){
        CURL *curl = curl_easy_init();
        Json::Value json;
        Json::CharReaderBuilder jsonBuilder;
        Json::Reader reader;
        Json::CharReader *JsonReader(jsonBuilder.newCharReader());
        double distance;
        std::string tmp = "http://ec2-34-222-175-250.us-west-2.compute.amazonaws.com/table/v1/driving/";
        // std::string tmp = "http://localhost:5000/table/v1/driving/";
        std::string errors;
        std::string readBuffer;
        for(int i = 0; i < N; i++){
            tmp += std::to_string(packets[i].loc_x) + "," + std::to_string(packets[i].loc_y) + ";";
        }
        tmp.pop_back();
        tmp += "?annotations=distance";
        clock_t tempoAPI = clock();
        if(curl){
            curl_easy_setopt(curl, CURLOPT_URL, tmp.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_perform(curl);
            const auto is_parsed = JsonReader->parse(
                readBuffer.c_str(),
                readBuffer.c_str() + readBuffer.length(),
                &json,
                &errors
            );

            if (!is_parsed)
            {
                std::cerr << "ERROR: Could not parse! " << errors << '\n';
            }
        }
        tempoAPI = clock() - tempoAPI;
        // Converter matrix distance??
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i == j) {
                    matrix_distance[i][j] = 999999.0;
                }
                else {
                    // std::cout << "distance[" << std::to_string(i) << "][" << std::to_string(j) << "] = " << std::to_string(json["distances"][i][j].asDouble()) << std::endl;
                    matrix_distance[i][j] = json["distances"][i][j].asDouble();
                }
            }
        }
        std::cout << "Tempo das chamadas API = " + std::to_string(((float)tempoAPI) / CLOCKS_PER_SEC) << std::endl;

        curl_easy_cleanup(curl);
    }
    else {
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
}
double CVRP::distance_euclidian(Packet origin, Packet destiny){
    double dx = (double)destiny.loc_x - (double)origin.loc_x;
    double dy = (double)destiny.loc_y - (double)origin.loc_y;
    return sqrt(dx * dx + dy * dy);
}

KMeans CVRP::parseKmeans(std::string fileKmeans){
    std::ifstream stream(fileKmeans);
    std::string line;
    std::string textJson;
    while(!stream.eof()){
        std::getline(stream,line);
        textJson += line;
    }
    Json::Value root;
    Json::Reader readerJson;
    readerJson.parse(textJson, root);
    int numVeiculos = root["num_veiculos"].asInt();
    int numClusters = root["n_clusters"].asInt();
    int totalPacotes = root["total_points"].asInt();
    KMeans kmeansSelect = KMeans(
        numClusters,
        totalPacotes,
        2,
        numVeiculos
    );
    std::vector<Cluster> clusteres;
    for(int i = 0; i < numClusters; i++){
        int idCluster = root["clusteres"][i]["id"].asInt();
        std::vector<double> coordinates;
        coordinates.push_back(root["clusteres"][i]["centroid"]["lng"].asDouble());
        coordinates.push_back(root["clusteres"][i]["centroid"]["lat"].asDouble());
        std::string id_point = "Centroid_" + std::to_string(i);
        std::string name = "Centroid";
        Point centroid = Point(id_point, coordinates, name);
        centroid.id_point = 0;
        centroid.setCluster(idCluster);
        Cluster cluster = Cluster(idCluster, centroid);
        const Json::Value packets = root["clusteres"][i]["packets"];
        int T = packets.size();
        for(int j = 0; j < T; j++){
            Packet packet = Packet(
                packets[j]["id"].asInt(),
                packets[j]["id_s"].asString(),
                packets[j]["point"]["lng"].asDouble(),
                packets[j]["point"]["lat"].asDouble(),
                packets[j]["point"]["capacity"].asInt()
            );
            packet.pt.setCluster(idCluster);
            packet.pt.id_s = packet.id_s;
            packet.pt.id_point = packet.id;
            cluster.addPacket(packet);
            cluster.addPoint(packet.pt);
        }
        kmeansSelect.addCluster(cluster);
        // for (int uu = 1; uu <= T; uu++) {
        //     std::cout << kmeansSelect.getCluster(idCluster).getPoint(uu).getID() << std::endl;
        // }
    }
    return kmeansSelect;
}

void CVRP::solveRegion(Arg args){
    std::vector<Vehicle> vehicles_used = args.mapRegion.vehiclePerRegion.at(args.region);
    std::vector<Packet> p_visited_region = 
        this->bestK.getCluster(args.region).getPackets(packets);
    Solution sol = solve(
        args.timeOrder,
        args.timeVRP,
        args.region,
        vehicles_used,
        p_visited_region,
        args.alpha
    );
    this->clusters_solved.push_back(sol);
}

void CVRP::solveRegionMIP(Arg args, double **matrix_distance){
    std::vector<Vehicle> vehicles_used = args.mapRegion.vehiclePerRegion.at(args.region);
    std::vector<Packet> p_visited_region = 
        this->bestK.getCluster(args.region).getPackets(packets);
    Solution sol = solveMIP(
        args.timeOrder,
        args.timeVRP,
        args.region,
        vehicles_used,
        matrix_distance,
        p_visited_region,
        args.alpha
    );
    this->clusters_solved.push_back(sol);
}

double CVRP::solveKmeansParallel(
    std::string fileKmeans,
    int timeOrder,
    int timeVRP,
    double alpha, 
    std::string nameInstance
    ){
    // Ler o arquivo pre compilado
    // Extrair as informações de quais pacotes estão em determinado clusters
    this->bestK = parseKmeans(fileKmeans);
    // Resolver de forma paralela cada cluster
    VehiclePerRegionSolution mapRegion = optimizeVehicles(bestK);
    std::vector<std::thread> threads(this->bestK.getK());
    Arguments arguments;
    arguments.alpha = alpha;
    arguments.mapRegion = mapRegion;
    arguments.timeOrder = timeOrder;
    arguments.timeVRP = timeVRP;
    std::clock_t start, end;
    start = clock();
    for (int region = 0; region < this->bestK.getK(); region++) {
        arguments.region = region;
        threads.push_back(
            std::thread([&]() {solveRegion(arguments); })
        );
    }
    for (std::thread& t : threads) {
        if(t.joinable()){
            t.join();
        }
    }
    threads.clear();
    end = clock();
    double timeExecution = (double)(end - start);
    return timeExecution;
}

double CVRP::solveKmeansSeriable(
    std::string fileKmeans,
    int timeOrder,
    int timeVRP,
    double alpha, 
    std::string nameInstance
    ){
    // Ler o arquivo pre compilado
    // Extrair as informações de quais pacotes estão em determinado clusters
    this->bestK = parseKmeans(fileKmeans);
    // Resolver de forma paralela cada cluster
    VehiclePerRegionSolution mapRegion = optimizeVehicles(bestK);
    Arguments arguments;
    arguments.alpha = alpha;
    arguments.mapRegion = mapRegion;
    arguments.timeOrder = timeOrder;
    arguments.timeVRP = timeVRP;
    std::clock_t start, end;
    start = clock();
    for (int region = 0; region < this->bestK.getK(); region++) {
        arguments.region = region;
        solveRegion(arguments);
    }
    end = clock();
    double timeExecution = (double)(end - start)/CLOCKS_PER_SEC;
    return timeExecution;
}
double CVRP::solveKmeansSeriableMIP(
    std::string fileKmeans,
    int timeOrder,
    int timeVRP,
    double alpha, 
    std::string nameInstance,
    double **matrix_distance
    ){
    // Ler o arquivo pre compilado
    // Extrair as informações de quais pacotes estão em determinado clusters
    this->bestK = parseKmeans(fileKmeans);
    // Resolver de forma paralela cada cluster
    VehiclePerRegionSolution mapRegion = optimizeVehicles(bestK);
    Arguments arguments;
    arguments.alpha = alpha;
    arguments.mapRegion = mapRegion;
    arguments.timeOrder = timeOrder;
    arguments.timeVRP = timeVRP;
    std::clock_t start, end;
    start = clock();
    for (int region = 0; region < this->bestK.getK(); region++) {
        arguments.region = region;
        solveRegionMIP(arguments, matrix_distance);
    }
    end = clock();
    double timeExecution = (double)(end - start)/CLOCKS_PER_SEC;
    return timeExecution;
}

void CVRP::solveWithKmeans(
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
    for(int region = 0; region < this->bestK.getClusters().size(); region++){
        std::vector<Vehicle> vehicles_used = mapRegion.vehiclePerRegion.at(region);
        std::vector<Packet> p_visited_region = 
            this->bestK.getCluster(region).getPackets(packets);
        Solution sol = solve(
            timeOrder,
            timeVRP,
            region,
            vehicles_used,
            p_visited_region,
            alpha
        );
        clusters_solved.push_back(sol);
        // sol.result.printerSolution();
    }
    std::cout << "PARTE PRINT 3" << std::endl;
    for(Solution s : clusters_solved){
        s.result.printerSolution();
    }
}

Solution CVRP::solveWithKmeans(
    int timeOrder, int timeVRP, 
    int N, int K, double alpha, 
    std::string nameInstance
){
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
    for(int region = 0; region < this->bestK.getClusters().size(); region++){
        std::vector<Vehicle> vehicles_used = mapRegion.vehiclePerRegion.at(region);
        std::vector<Packet> p_visited_region = 
            this->bestK.getCluster(region).getPackets(packets);
        Solution sol = solve(
            timeOrder,
            timeVRP,
            region,
            vehicles_used,
            p_visited_region,
            alpha, 
            nameInstance
        );
        clusters_solved.push_back(sol);
    }
    /*std::cout << "PARTE PRINT 3" << std::endl;
    for(Solution s : clusters_solved){
        s.result.printerSolution();
    outputJson(clusters_solved,nameInstance);
    }*/
}

void CVRP::outputJson(
    std::string city,
    std::vector<Solution> solutions,
    std::string nameInstance,
    double time_execution
){
    Json::Value root;
	root["name"] = nameInstance;
    root["k_regions"] = solutions.size();
    root["time_execution"] = time_execution;
	Json::Value vehicles_json(Json::arrayValue);
    for(Solution s : solutions){
        for(Vehicle v : s.result.vehicles){
            Json::Value route;
            Json::Value deliveries_json(Json::arrayValue);
            for(Packet p : v.deliveries){
                if(p.id_s != ""){
                    Json::Value delivery;
                    delivery["id"] = p.id_s;
                    delivery["point"]["lng"] = p.loc_x;
                    delivery["point"]["lat"] = p.loc_y;
                    delivery["size"] = p.charge;
                    deliveries_json.append(delivery);
                }
            }
            route["origin"]["lng"] = v.origin.loc_x;
            route["origin"]["lat"] = v.origin.loc_y;
            route["deliveries"] = deliveries_json;
            if (!deliveries_json.empty()) {
                vehicles_json.append(route);
            }
        }
    }
    root["total_vehicles"] = vehicles_json.size();
	root["vehicles"] = vehicles_json;
    std::string dir_path_here("out/"+city+"0/");
	Json::FastWriter writer;
	const std::string json_file = writer.write(root);
	std::string fileoutput(dir_path_here + nameInstance + ".json");
	std::fstream output_fstream;
	output_fstream.open(fileoutput, std::ios_base::out);
	if(!output_fstream.is_open()){
		std::cerr << "Failed to open" << json_file << '\n';
	} else {
		output_fstream << json_file << std::endl;
	}
}

void CVRP::printerPackets(std::vector<Packet> packs){
    std::cout << "packs = [ ";
    for(Packet p : packs){
        std::cout << p.id << " "; 
    }
    std::cout << "]!" << std::endl;
}

void CVRP::printerVehicles(std::vector<Vehicle> vehicles){
    std::cout << "vehicles = [ ";
    for(Vehicle v : vehicles){
        std::cout << v.id << " "; 
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
    int timeOrder, int timeVRP, int regiao,
    std::vector<Vehicle> vehicles_used, 
    std::vector<Packet> packs, double alpha
){
    int Nsub = packs.size();
    int Ksub = vehicles_used.size();    
    //A resolução desse problema retorna os packets organizados por veículos
    // printerPackets(packs);
    // printerVehicles(vehicles_used);
    Order organizePackets = Order(
        packs,
        vehicles_used,
        alpha, regiao
    );
    // resolver o problema da "mochila multipla"
    Solution solOrder = organizePackets.solve(timeOrder);
    VRP vrpRelaxFix = VRP(solOrder.partial.output, packs, vehicles_used, regiao);
    Solution solVRP = vrpRelaxFix.solve(timeVRP);
    Solution sol = Solution(solOrder.partial, solVRP.result);
    return sol;
}
Solution CVRP::solveMIP(
    int timeOrder, int timeVRP, int regiao,
    std::vector<Vehicle> vehicles_used, double **matrix_distance, 
    std::vector<Packet> packs, double alpha
){
    int Nsub = packs.size();
    int Ksub = vehicles_used.size();    
    //A resolução desse problema retorna os packets organizados por veículos
    Order organizePackets = Order(
        packs,
        vehicles_used,
        alpha, regiao
    );
    // resolver o problema da "mochila multipla"
    Solution solOrder = organizePackets.solve(timeOrder);
    VRPMIP vrp = VRPMIP(
        solOrder.partial.output, 
        packs, 
        vehicles_used,
        matrix_distance, 
        regiao);
    Solution solVRP = vrp.solve(timeVRP);
    Solution sol = Solution(solOrder.partial, solVRP.result);
    return sol;
}

Solution CVRP::solve(
    int timeOrder, int timeVRP, int regiao,
    std::vector<Vehicle> vehicles_used, 
    std::vector<Packet> packs, double alpha, std::string nameInstance
){
    int Nsub = packs.size();
    int Ksub = vehicles_used.size();    
    //A resolução desse problema retorna os packets organizados por veículos
    // printerPackets(packs);
    // printerVehicles(vehicles_used);
    Order organizePackets = Order(
        packs,
        vehicles_used,
        alpha, regiao
    );
    // resolver o problema da "mochila multipla"
    Solution solOrder = organizePackets.solve(timeOrder);
    VRP vrpRelaxFix = VRP(solOrder.partial.output, packs, vehicles_used, regiao);
    Solution solVRP = vrpRelaxFix.solve(timeVRP, nameInstance);
    Solution sol = Solution(solOrder.partial, solVRP.result);
    return sol;
}

VehiclePerRegionSolution CVRP::optimizeVehicles(KMeans kmeans){
    // Construir uma lista de PacketCentroid
    // printerVehicles(vehicles);
    // printerPackets(packets);
    WhichVehicleProblem pk = WhichVehicleProblem(
        kmeans, packets, vehicles
    );
    Solution optimize = pk.solve(25);
    return optimize.whichvehicles;
}