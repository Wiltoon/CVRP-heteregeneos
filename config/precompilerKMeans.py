import json

import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np
import pandas as pd

from sklearn.datasets import make_blobs
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_samples, silhouette_score

# Read Instance inicial
# Gerar o arquivo compilado que será utilizado pelo C++


def readInstance(instance: str, K: int, type: bool, mode: str):
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
        bestKmeans = findToBestK(dataframe, K, mode, 300)
        return bestKmeans, data
    else:
        df = pd.read_csv(instance)
        df_variables2 = df.drop(['CUSTOMER CUST NO.', ' DEMAND', 'READY TIME',
                                'SERVICE TIME', 'AVAIL. TIME', 'DUE DATE'], axis=1)
        df_variables = df_variables2.drop([0], axis=0)
        bestKmeans = findToBestK(df_variables, K, mode, 300)
        return bestKmeans, df_variables


def findToBestK(
    dataframe: pd.DataFrame,
    K: int,
    initializer: str,
    iterations: int
) -> KMeans:
    range_n_clusters = [cluster for cluster in range(2, K)]
    score_max = -999
    for n_clusters in range_n_clusters:
        clusteres = KMeans(
            n_clusters=n_clusters,
            init=initializer,
            max_iter=iterations,
            random_state=10
        )
        cluster_labels = clusteres.fit_predict(dataframe)
        silhouette_avg = silhouette_score(dataframe, cluster_labels)
        if score_max < silhouette_avg:
            best_clusteres = clusteres
            score_max = silhouette_avg
    return best_clusteres


def generateJson(
        nameInstance: str,
        bestKmeans: KMeans,
        data,
        K: int):
    content = dict()
    content['num_veiculos'] = K
    content['n_clusters'] = bestKmeans.n_clusters
    content['total_points'] = len(data['deliveries'])
    content['clusteres'] = []
    dotes = {}
    for k in range(bestKmeans.n_clusters):
        dotes[k] = []
    for i in range(len(bestKmeans.labels_)):
        dotes[bestKmeans.labels_[i]].append(i)
    for key in dotes.keys():
        cluster = dict()
        cluster['id'] = key
        cluster['packets'] = []
        sumcapacity = 0
        for index in dotes[key]:
            packet = dict()
            packet['id'] = index
            packet['id_s'] = data['deliveries'][index]['id']
            point = dict()
            point['lng'] = data['deliveries'][index]['point']['lng']
            point['lat'] = data['deliveries'][index]['point']['lat']
            packet['point'] = point
            packet['capacity'] = data['deliveries'][index]['size']
            sumcapacity = sumcapacity + data['deliveries'][index]['size']
            cluster['packets'].append(packet)
        cluster['capacity_s'] = sumcapacity
        coordanates = dict()
        coordanates['lng'] = bestKmeans.cluster_centers_[key][0]
        coordanates['lat'] = bestKmeans.cluster_centers_[key][1]
        cluster['centroid'] = coordanates
        content['clusteres'].append(cluster)
    jstr = json.dumps(content, ensure_ascii=False, indent=4)
    print(jstr)
    with open(nameInstance, "w") as outfile:
        outfile.write(jstr)

def main():
    nameOutput = "cvrp-0-pa-0-kmeans.json"
    n_veiculos = 50
    bestK, data = readInstance(
        "..\\resource\\Loggibud\\cvrp-0-pa-0.json",
        K = n_veiculos,
        type = True,
        mode = "k-means++"
    )
    generateJson(
        nameOutput, 
        bestK,
        data, 
        n_veiculos
    )


if __name__ == '__main__':
   main()
# Criar um arquivo contendo a informação
# {
#       "num_veiculos": K,
#       "n_clusters": 10,
#       "total_points": N,
#       "clusteres": [
#           {
#               "id": 0,
#               "packets" : [
#                   {
#                       "id": 1,
#                       "id_s": "31sd3-142dasw-41241dad",
#                       "point": {
#                           "lat": 24.2423423424,
#                           "lng": 32.6365643624
#                       },
#                       "capacity": 7
#                   },
#                   {... packets}
#               ],
#               "capacity": sum(caps_packets),
#               "centroid": {
#                   "lat": 13.31231231,
#                   "lng": 15.65646323
#               }
#           },
#           {... clusteres}
#       ]
# }
