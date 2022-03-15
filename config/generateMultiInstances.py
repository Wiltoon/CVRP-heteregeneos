import os
import time
from precompilerKMeans import *

def generates(paths_to_precompile: str, n_intances: int, city: str):
    for cont in range(n_intances):
        broke_city = city.split('-')
        name_instance = "cvrp-0-"+broke_city[0]+"-"+str(cont)
        instance = paths_to_precompile + name_instance + ".json"
        n_veiculos = extract_vehicles(instance)
        it = instance.split("/")
        print(it)
        ext = it[len(it)-1].split('.')
        nameOutput = "../resource/Loggibud/KMeans/"+city+name_instance+"-kmeans."+ext[1]
        timeStart = time.time()
        bestK, data = readInstance(
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

def extract_vehicles(instance: str):
    df = open(instance)
    data = json.load(df)
    capacity_total = []
    for delivery in data["deliveries"]:
        capacity_total.append(delivery["size"])
    capacity = sum(capacity_total)
    n_veiculos = capacity/data["vehicle_capacity"]
    return (int)(n_veiculos+5)

if __name__ == '__main__':
    cities = ["pa-0/", "df-0/", "rj-0/"]
    n_intances = 20
    for city in cities:
        paths_to_precompile = "../resource/Loggibud/cvrp-instances-1.0/train/"+city
        generates(paths_to_precompile, n_intances, city)
        