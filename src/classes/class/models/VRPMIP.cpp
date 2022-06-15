#include <iostream>

#include "VRPMIP.hpp"
#include "../Packet.hpp"
#include "../Vehicle.hpp"

VRPMIP::VRPMIP(
    IloArray<IloNumArray> output,
    std::vector<Packet> packets,
    std::vector<Vehicle> vehicles,
    double **matriz,
    int region)
{
    this->region = region;
    this->packets = packets;
    this->vehicles = vehicles;
    this->N = packets.size();
    this->K = vehicles.size();
    this->matriz = matriz;
    IloModel model(env);
    this->model = model;
    // std::cout << "id :" << std::to_string(packets[0].id) << std::endl;
    IloArray<IloNumArray> managerPackets(env, this->K);
    for(int k = 0; k < this->K; k++){
        managerPackets[k] = IloNumArray(env, this->N);
        for (int p = 0; p < this->N; p++){
            // std::cout << output[region][p] << std::endl;
            if (output[region][p] > 0.9){
                managerPackets[k][p] = 1;
            } else {
                managerPackets[k][p] = 0;
            }
        }
    }
    this->managerPackets = managerPackets;
}
VRPMIP::VRPMIP(
    std::vector<Packet> packets,
    std::vector<Vehicle> vehicles,
    double **matriz,
    int region)
{
    this->region = region;
    this->packets = packets;
    this->vehicles = vehicles;
    this->matriz = matriz;
    this->N = packets.size();
    this->K = vehicles.size();
    IloModel model(env);
    this->model = model;
}

void VRPMIP::createParams()
{
    // Matriz de distancia
    IloArray<IloNumArray> d(env, N);
    for (int i = 0; i < N; i++)
    {
        d[i] = IloNumArray(env, N);
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == j)
            {
                d[i][j] = 999999.0;
            }
            else
            {
                d[i][j] = matriz[i][j];
            }
        }
    }
    // Carga dos Pacotes
    IloNumArray p(env, N);
    for (int i = 0; i < N; i++)
    {
        p[i] = packets[i].charge;
    }
    // Custo dos veículos
    IloNumArray e(env, K);
    // Ativação do veiculo k
    IloBoolArray v(env, K);
    // Ativação do veiculo k
    IloArray<IloBoolArray> w(env, K);
    // Carga maxima do veículo k
    IloNumArray Q(env, K);
    for (int k = 0; k < K; k++){
        e[k] = vehicles[k].cust;
        v[k] = 1;
        Q[k] = vehicles[k].charge_max;
        w[k] = IloBoolArray(env, N);
        for (int i = 0; i < N; i++){
            w[k][i] = (this->managerPackets[k][i] > 0.9);
        }
    }
    this->d = d;
    this->w = w;
    this->p = p;
    this->e = e;
    this->v = v;
    this->Q = Q;
}

void VRPMIP::createVariables(){
    // Veículo k percorre o arco i->j
    IloArray<IloArray<IloBoolVarArray>> x(env, K);
    for (int k = 0; k < K; k++)
    {
        x[k] = IloArray<IloBoolVarArray>(env, N); // Rotas atribuidas aos arcos (i,j)
        for (int i = 0; i < N; i++)
        {
            x[k][i] = IloBoolVarArray(env, N);
        }
    }
    // Pacote atendido pelo veiculo k w_kj
    // IloArray<IloBoolVarArray> w(env, K); // DESTINOS (index j)
    // Veículo K chega do ponto j
    IloArray<IloBoolVarArray> y(env, K);
    // Veículo K sai do ponto i
    IloArray<IloBoolVarArray> z(env, K); // (index i)
    // Veículo K carrega U cargas até o ponto J
    IloArray<IloNumVarArray> u(env, K);
    for (int k = 0; k < K; k++)
    {
        y[k] = IloBoolVarArray(env, N);
        z[k] = IloBoolVarArray(env, N);
        // w[k] = IloBoolVarArray(env, N);
        u[k] = IloNumVarArray(env, N, 0, vehicles[k].charge_max); // Variavel auxiliar para eliminicao de rota
    }
    // this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
    this->u = u;
    renameVars();
}

void VRPMIP::renameVars()
{
    char *char_u;
    for (int k = 0; k < K; k++)
    {
        for (int i = 0; i < N; i++)
        {
            std::string nameu("u_" + std::to_string(k) + "_" + std::to_string(i));
            char_u = &nameu[0];
            u[k][i].setName(char_u);
            u[k][i].setBounds(p[i + 1], Q[k]);
        }
    }

    char *char_x;
    for (int k = 0; k < K; k++)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                std::string namex("x_" + std::to_string(k) + "_" + std::to_string(i) + "_" + std::to_string(j));
                char_x = &namex[0];
                x[k][i][j].setName(char_x);
                if (i == j)
                {
                    x[k][i][j].setBounds(0, 0);
                }
            }
        }
    }

    // char* char_w;
    // for(int k = 0; k < K; k++){
    //     for(int i = 0; i < N; i++) {
    //         std::string namew("w_" + std::to_string(k) + "_" + std::to_string(i));
    //         char_w = &namex[0];
    //         w[k][i].setName(char_w);
    //     }
    // }
}

void VRPMIP::createFunctionObjetive()
{
    // criar a funcao objetivo no ambiente env e_k pode ser custo por km
    IloExpr cust(env);
    for (int k = 0; k < K; k++)
    {
        IloExpr fo(env);
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                fo += d[i][j] * x[k][i][j];
            }
        }
        cust += fo * e[k] * v[k];
        fo.end();
    }
    model.add(IloMinimize(env, cust));
    cust.end();
}

void VRPMIP::createConstraints()
{
    // RESTRIÇOES PARA CONSTRUIR AS ROTAS DO VRP COM RETORNO_DEPOSITO
    constraintLeaveCustomer();
    constraintArriveCustomer();
    constraintFlow();
    constraintDeposit();
    constraintMTZ();
    // constraintManagerPacket();
}

Solution VRPMIP::solve(int timeLimite)
{
    // aqui deve resolver o problema VRP
    IloCplex cplex(model);
    IloNum objFO = IloInfinity;
    createParams();
    createVariables();
    createFunctionObjetive();
    createConstraints();
    VRPSolution o = mip(timeLimite, cplex);
    Solution sol = Solution(o);
    return sol;
}

IloArray<IloNumArray> VRPMIP::buildUSol()
{
    IloArray<IloNumArray> uSol(env, K);
    for (int k = 0; k < K; k++)
    {
        uSol[k] = IloNumArray(env, N);
    }
    return uSol;
}

IloArray<IloNumArray> VRPMIP::buildWSol()
{
    IloArray<IloNumArray> wSol(env, K);
    for (int k = 0; k < K; k++)
    {
        wSol[k] = IloNumArray(env, N);
    }
    return wSol;
}

IloArray<IloArray<IloNumArray>> VRPMIP::buildXSol()
{
    IloArray<IloArray<IloNumArray>> xSol(env, K);
    for (int k = 0; k < K; k++)
    {
        xSol[k] = IloArray<IloNumArray>(env, N);
        for (int i = 0; i < N; i++)
        {
            xSol[k][i] = IloNumArray(env, N);
        }
    }
    return xSol;
}

VRPSolution VRPMIP::mip(int timeLimite, IloCplex &cplex)
{
    IloArray<IloArray<IloNumArray>> xSol = buildXSol();
    IloArray<IloNumArray> uSol = buildUSol();

    IloBool result = solveIterationMIP(timeLimite, cplex);
    if (result)
    {
        assignTheSolutionsMIP(xSol, uSol, cplex);
    }
    else
    {
        std::cout << "Sem solução!";
    }
    VRPSolution vrp = VRPSolution(
        xSol, uSol, vehicles, packets);
    return vrp;
}

void VRPMIP::assignTheSolutionsMIP(
    IloArray<IloArray<IloNumArray>> &xSol,
    IloArray<IloNumArray> &uSol,
    IloCplex &cplex)
{
    for (int k = 0; k < K; k++)
    {
        cplex.getValues(u[k], uSol[k]);
        for (int i = 0; i < N; i++)
        {
            cplex.getValues(x[k][i], xSol[k][i]);
        }
    }
}

IloBool VRPMIP::solveIterationMIP(
    int tempo,
    IloCplex &cplex)
{
    cplex.setParam(IloCplex::TiLim, tempo);
    cplex.extract(model);
    char *outputer;
    std::string saida("out/saida_R" + std::to_string(region) + ".lp");
    outputer = &saida[0];
    cplex.setOut(env.getNullStream());
    cplex.exportModel(outputer);
    return cplex.solve();
}

void VRPMIP::constraintLeaveCustomer()
{
    IloConstraintArray cons_leave(env);
    for (int k = 0; k < K; k++)
    {
        for (int j = 0; j < N; j++)
        {
            IloExpr restLeft(env);
            char *namevar;
            std::string name(
                "vehicle_" + std::to_string(k) + "_saiDoPonto_" + std::to_string(j));
            namevar = &name[0];
            for (int i = 0; i < N; i++)
            {
                restLeft += x[k][i][j];
            }
            IloConstraint consLeft = (restLeft == w[k][j]);
            consLeft.setName(namevar);
            model.add(consLeft);
            cons_leave.add(consLeft);
            restLeft.end();
        }
    }
}

void VRPMIP::constraintArriveCustomer()
{
    IloConstraintArray cons_arrive(env);
    for (int k = 0; k < K; k++)
    {
        for (int i = 0; i < N; i++)
        {
            IloExpr restArrive(env);
            char *namevar;
            std::string name(
                "vehicle_" + std::to_string(k) + "_chegaNoPonto_" + std::to_string(i));
            namevar = &name[0];
            for (int j = 0; j < N; j++)
            {
                restArrive += x[k][i][j];
            }
            IloConstraint consArr = (restArrive == w[k][i]);
            consArr.setName(namevar);
            model.add(consArr);
            cons_arrive.add(consArr);
            restArrive.end();
        }
    }
}

void VRPMIP::constraintFlow()
{
    IloConstraintArray cons_flow(env);
    for (int k = 0; k < K; k++)
    {
        for (int p = 0; p < N; p++)
        {
            IloExpr restFlow1(env);
            IloExpr restFlow2(env);
            char *namevar;
            std::string name(
                "vehicle_" + std::to_string(k) + "_caminhoDe_" + std::to_string(p));
            namevar = &name[0];
            for (int i = 0; i < N; i++)
            {
                restFlow1 += x[k][i][p];
            }
            for (int j = 0; j < N; j++)
            {
                restFlow2 += x[k][p][j];
            }
            IloConstraint consFlow = (restFlow1 == restFlow2);
            consFlow.setName(namevar);
            model.add(consFlow);
            cons_flow.add(consFlow);
            restFlow1.end();
            restFlow2.end();
        }
    }
}

void VRPMIP::constraintDeposit()
{
    IloConstraintArray cons_dep(env);
    for (int k = 0; k < K; k++)
    {
        IloExpr restDep(env);
        char *namevar;
        std::string name(
            "vehicle_" + std::to_string(k) + "_ativo");
        namevar = &name[0];
        for (int j = 0; j < N; j++)
        {
            if (j == 0)
            {
                x[k][0][j].setBounds(0, 0);
            }
            else
            {
                restDep += x[k][0][j];
            }
        }
        IloConstraint consDep = (restDep == v[k]);
        consDep.setName(namevar);
        model.add(consDep);
        cons_dep.add(consDep);
        restDep.end();
    }
}

void VRPMIP::constraintMTZ()
{
    IloConstraintArray cons_MTZ(env);
    for (int k = 0; k < K; k++)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 1; j < N; j++)
            {
                if (i != j)
                {
                    IloConstraint mtz = (u[k][i] >= u[k][j] + p[i] - Q[k] * (1 - x[k][i][j]));
                    char *char_arr;
                    std::string name(
                        "MTZ_" +
                        std::to_string(k) + "_" +
                        std::to_string(i) + "_" +
                        std::to_string(j));
                    char_arr = &name[0];
                    mtz.setName(char_arr);
                    cons_MTZ.add(mtz);
                    model.add(mtz);
                }
            }
        }
    }
}

void VRPMIP::constraintManagerPacket()
{
    IloConstraintArray cons_mag(env);
    for (int i = 1; i < N; i++)
    {
        IloExpr manag(env);
        for (int k = 0; k < K; k++)
        {
            manag += w[k][i];
        }
        IloConstraint unicVehicle = (manag == 1);
        manag.end();
        char *char_arr;
        std::string name(
            "manager_" + std::to_string(i));
        char_arr = &name[0];
        unicVehicle.setName(char_arr);
        cons_mag.add(unicVehicle);
        model.add(unicVehicle);
    }
}