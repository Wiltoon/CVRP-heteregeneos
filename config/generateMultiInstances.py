import os
import time
from precompilerKMeans import *

def generates(paths_to_precompile: str, n_intances: int, city: str):
    for cont in range(90,90+n_intances):
        broke_city = city.split('-')
        name_instance = "cvrp-0-"+broke_city[0]+"-"+str(cont)
        instance = paths_to_precompile + name_instance + ".json"
        n_veiculos = extract_vehicles(instance)
        it = instance.split("/")
        ext = it[len(it)-1].split('.')
        print(instance)
        nameOutput = "../resource/Loggibud/KMeans/"+city+name_instance+"-kmeans."+ext[1]
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

def extract_vehicles(instance: str):
    df = open(instance)
    data = json.load(df)
    capacity_total = []
    for delivery in data["deliveries"]:
        capacity_total.append(delivery["size"])
    capacity = sum(capacity_total)
    n_veiculos = capacity/data["vehicle_capacity"]
    return (int)(n_veiculos)

if __name__ == '__main__':
    cities = ["pa-0/", "df-0/", "rj-0/"]
    # cities = ["pa-0/"]
    n_intances = 30
    for city in cities:
        paths_to_precompile = "../resource/Loggibud/cvrp-instances-1.0/dev/"+city
        generates(paths_to_precompile, n_intances, city)
        