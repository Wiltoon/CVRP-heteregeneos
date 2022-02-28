#include "WhichVehicleProblem.hpp"

WhichVehicleProblem::WhichVehicleProblem(
    KMeans kmeans, 
    std::vector<Packet> packets,
    std::vector<Vehicle> vehicles
){
    this->kmeans = kmeans;
    this->packet_total = packets;
    this->vehicles = vehicles;
    IloModel model(env);
    this->model = model;
}

KMeans WhichVehicleProblem::getKmeans(){
    return this->kmeans;
}

std::vector<Packet> WhichVehicleProblem::getPacketsTotal(){
    return this->packet_total;
}

std::vector<Vehicle> WhichVehicleProblem::getVehicles(){
    return this->vehicles;
}

void WhichVehicleProblem::createParams(){
    int regions_size = getKmeans().getClusters().size();
    int K = getVehicles().size();

    IloNumArray R(env, regions_size);
    for(int r = 0; r<regions_size; r++){
        R[r] = capacityTotalRegion(getKmeans().getCluster(r));
    }
    IloNumArray d(env, regions_size);
    for(int r = 0; r < regions_size; r++){
        d[r] = distanceRegion(getKmeans().getCluster(r));
    }
    IloNumArray c(env, K);
    for(int k = 0; k < K; k++){
        c[k] = vehicles[k].cust;
    }
    IloNumArray Qm(env, K);
    for(int k = 0; k < K; k++){
        Qm[k] = vehicles[k].charge_max;
    }
    this->R = R;
    this->d = d;
    this->c = c;
    this->Qm = Qm;
}

void WhichVehicleProblem::createVariables(){
    int regions_size = getKmeans().getClusters().size();
    int K = getVehicles().size();
    IloArray <IloBoolVarArray> x(env, regions_size);
    for(int r = 0; r < regions_size; r++){
        x[r] = IloBoolVarArray(env, K);
    }
    this->x = x;
    renameVars();
}

void WhichVehicleProblem::createFunctionObjetive(){
    int K = getVehicles().size();
    int N = getKmeans().getClusters().size();
    IloExpr fo(env);
    for (int r = 0; r < N; r++) {
        for (int v = 0; v < K; v++) {
            fo += c[v] * d[r] * x[r][v];
        }
    }
    model.add(IloMinimize(env, fo));
    fo.end();
}

void WhichVehicleProblem::createConstraints(){
    constraintRegionCapacity();
    constraintJustAVehicleIntoRegion();
}

Solution WhichVehicleProblem::solve(int timeLimite){
    IloCplex cplex(model);
    IloNum objFO = IloInfinity;
    createParams();
    createVariables();
    createFunctionObjetive();
    createConstraints();
    std::cout << "Solution WhichVehicleProblem::solved" << std::endl;
    cplex.setParam(IloCplex::TiLim, timeLimite);
    cplex.extract(model);
    char* output;
    std::string saida("out/saida_veiculos_qntd_per_region.lp");
    output = &saida[0];
    cplex.exportModel(output);
    IloBool result = cplex.solve();
    std::cout << "result: " << result << std::endl;
    if(result){
        // Criar o vetor de vehicles per region
        VehiclePerRegionSolution out = vehiclesPerRegion(cplex);
        Solution sol = Solution(out);
        return sol;
    } else {
        // Retornar ERRO INFACTIBILIDADE?
        std::cout << "ERRO ";
        VehiclePerRegionSolution out = VehiclePerRegionSolution("Not results feasible Vehicle Per Region");
        Solution sol = Solution(out);
        return sol;
    }
}

void WhichVehicleProblem::renameVars(){
    int N = getKmeans().getClusters().size();
    int K = getVehicles().size();
    
    char* char_x;
    for(int r = 0; r < N; r++) {
        for(int v = 0; v < K; v++){
            std::string namex("x_" + std::to_string(r) + "_" + std::to_string(v));
            char_x = &namex[0];
            x[r][v].setName(char_x);
        }
    }
}

void WhichVehicleProblem::constraintRegionCapacity(){
    int regions = getKmeans().getClusters().size();
    int K = getVehicles().size();
    
    IloConstraintArray cons_array_cap(env);
    for (int r = 0; r < regions; r++) {
        IloExpr restCap(env);
        char* namevar;
        std::string name("capacityRegion" + std::to_string(r));
        namevar = &name[0];
        for (int v = 0; v < K; v++) {
            restCap += x[r][v] * Qm[v];
        }
        IloConstraint consRestCap = (restCap >= R[r]);
        consRestCap.setName(namevar);
        model.add(consRestCap);
        cons_array_cap.add(consRestCap);
        restCap.end();
    }
}

void WhichVehicleProblem::constraintJustAVehicleIntoRegion(){
    int regions = getKmeans().getClusters().size();
    int K = getVehicles().size();

    IloConstraintArray cons_array_vei(env);
    for (int v = 0; v < K; v++) {
        IloExpr restVeiCap(env);
        char* namevar;
        std::string name("vehicleRegion" + std::to_string(v));
        namevar = &name[0];
        for (int r = 0; r < regions; r++) {
            restVeiCap += x[r][v];
        }
        IloConstraint consRestVeiCap = (restVeiCap <= 1);
        consRestVeiCap.setName(namevar);
        model.add(consRestVeiCap);
        cons_array_vei.add(consRestVeiCap);
        restVeiCap.end();
    }
}

int WhichVehicleProblem::capacityTotalRegion(Cluster region){
    int cap_total = 0;
    for(Packet pac : region.getPackets(getPacketsTotal())){
        cap_total += pac.charge;
    }
    return cap_total;
}

double WhichVehicleProblem::distanceRegion(Cluster region){
    double dx = (double)region.getCentralValue(0) - (double)packet_total[0].loc_x;
    double dy = (double)region.getCentralValue(1) - (double)packet_total[0].loc_y;
    return sqrt(dx * dx + dy * dy);
}

VehiclePerRegionSolution WhichVehicleProblem::vehiclesPerRegion(IloCplex& cplex){
    int regions = getKmeans().getClusters().size();
    int K = getVehicles().size();
    std::string message("Vehicle Per Region Sucess!");
    IloArray <IloNumArray> output(env, regions);
    for(int r = 0; r < regions; r++){
        output[r] = IloNumArray(env, K);
    }
    for(int r = 0; r < regions; r++){
        cplex.getValues(x[r], output[r]);
    }
    std::cout << message << std::endl;
    VehiclePerRegionSolution solution = VehiclePerRegionSolution(
        output, message, getPacketsTotal(), getVehicles(), regions
    );
    return solution;
}