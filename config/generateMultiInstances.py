import os
from precompilerKMeans import *

def generates(paths_to_precompile: str):
    paths = [
        os.path.join(os.path.join(paths_to_precompile,nome)) 
        for nome in os.listdir(paths_to_precompile)
    ]
    n_veiculos = 20
    for instance in paths:
        it = instance.split("/")
        nameOutput = "../resource/Loggibud/KMeans/"+it[len(it)-1]
        bestK, data = readInstance(
            instance, 
            Kinit = int(n_veiculos/2),
            K = n_veiculos,
            type = True,
            mode = 'k-means++'
        )
        generateJson(
            nameOutput,
            bestK,
            data,
            n_veiculos
        )

if __name__ == '__main__':
    cities = "pa-0/"
    paths_to_precompile = "../resource/Loggibud/cvrp-instances-1.0/train/"+cities
    generates(paths_to_precompile)