import os
import time
from precompilerKMeans import *

def generates(paths_to_precompile: str):
    paths = [
        os.path.join(os.path.join(paths_to_precompile,nome)) 
        for nome in os.listdir(paths_to_precompile)
    ]
    n_veiculos = 20
    for instance in paths:
        it = instance.split("/")
        ext = it[len(it)-1].split('.')
        nameOutput = "../resource/Loggibud/KMeans/"+ext[0]+"-kmeans."+ext[1]
        timeStart = time.time()
        bestK, data = readInstance(
            instance, 
            Kinit = int(n_veiculos/4),
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

if __name__ == '__main__':
    cities = "pa-0/"
    paths_to_precompile = "../resource/Loggibud/cvrp-instances-1.0/train/"+cities
    generates(paths_to_precompile)