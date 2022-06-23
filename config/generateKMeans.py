import os
import time
from config.types import ParamsVehicles
from precompilerKMeans import *

def generates(n_intances: int, city: str, typeVehicle: str):
    paths_to_precompile = "../resource/Loggibud/cvrp-instances-"+typeVehicle+"/dev/"+city
    for cont in range(90,90+n_intances):
        broke_city = city.split('-')
        name_instance = "cvrp-0-"+broke_city[0]+"-"+str(cont)
        instance = paths_to_precompile + name_instance + ".json"
        n_veiculos, maxcap, mincap = extract_vehicles(name_instance + ".json")
        it = instance.split("/")
        ext = it[len(it)-1].split('.')
        print(instance)
        nameOutput = "../resource/Loggibud/KMeans-"+typeVehicle+"/"+city+name_instance+"-kmeans."+ext[1]
        timeStart = time.time()
        bestK, data = readInstanceConstrained(
            instance, 
            Kinit = 2,
            K = n_veiculos,
            type = True,
            mode = 'k-means++'
        )
        timeEnd = time.time()
        timeExecution = timeEnd - timeStart
        generateJson(
            nameOutput,
            bestK,
            data,
            n_veiculos,
            timeExecution
        )

def readInstanceConstrained(
    instance: str, 
    K: int, 
    type: bool, 
    mode: str,
    capmax: int,
    capmin: int
    ):
    if(type):
        df = open(instance)
        data = json.load(df)
        lngs = []
        lats = []
        for delivery in data["deliveries"]:
            lngs.append(delivery["point"]["lng"])
            lats.append(delivery["point"]["lat"])
        dataframe = pd.DataFrame({
            'lng': lngs,
            'lat': lats
        })
        sizeMax = capmax*4/5 # vehicle_MAX/5 + 1
        sizeMin = capmin*4/5
        print(sizeMin, sizeMax)
        bestKmeans = KMeansConstrained(
            n_clusters=K, #verificar como selecionar o k
            size_max=sizeMax,
            size_min=sizeMin,
            random_state=0
        )
        bestKmeans.fit_predict(dataframe)
        return bestKmeans, data
    else:
        df = pd.read_csv(instance)
        df_variables2 = df.drop(['CUSTOMER CUST NO.', ' DEMAND', 'READY TIME',
                                'SERVICE TIME', 'AVAIL. TIME', 'DUE DATE'], axis=1)
        df_variables = df_variables2.drop([0], axis=0)
        bestKmeans = findToBestK(df_variables, K, mode, 300)
        return bestKmeans, df_variables


def extract_vehicles(name_intance: str):
    path_params = "../resource/Loggibud/cvrp-instances-2.0/params/"
    # ler os parametros para retornar a qnt de vehicles existem e os tipos
    params = ParamsVehicles.from_file(path_params + name_intance)
    n_vehicles = sum(params.num_types)
    maxcap = min(params.capacities)
    mincap = max(params.capacities)
    return (int)(n_vehicles), maxcap, mincap

# Gerar KMEANS 2.0 de acordo com os veiculos existentes
if __name__ == '__main__':
    cities = ["pa-0/", "df-0/", "rj-0/"]
    typeVehicle = "2.0"
    # cities = ["pa-0/"]
    n_intances = 30
    for city in cities:
        generates(n_intances, city, typeVehicle)
        