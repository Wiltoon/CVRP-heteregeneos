import os
import time
from precompilerKMeans import *

def generates(n_intances: int, city: str, typeVehicle: str):
    paths_to_precompile = "../resource/Loggibud/cvrp-instances-"+typeVehicle+"/dev/"+city
    for cont in range(n_intances):
        broke_city = city.split('-')
        name_instance = "cvrp-0-"+broke_city[0]+"-"+str(cont)
        instance = paths_to_precompile + name_instance + ".json"
        n_veiculos = extract_vehicles(instance)
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

# Gerar KMEANS 2.0 de acordo com os veiculos existentes
if __name__ == '__main__':
    cities = ["pa-0/", "df-0/", "rj-0/"]
    typeVehicle = "2.0"
    # cities = ["pa-0/"]
    n_intances = 20
    for city in cities:
        generates(paths_to_precompile, n_intances, city, typeVehicle)
        