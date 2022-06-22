import json

import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np
import pandas as pd

from sklearn.datasets import make_blobs
from sklearn.cluster import KMeans
from k_means_constrained import KMeansConstrained
from sklearn.metrics import silhouette_samples, silhouette_score

# Read Instance inicial
# Gerar o arquivo compilado que será utilizado pelo C++


def readInstance(
    instance: str, 
    Kinit: int, 
    K: int, 
    type: bool, 
    mode: str
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
        bestKmeans = findToBestK(dataframe, Kinit, K, mode, 300)
        return bestKmeans, data
    else:
        df = pd.read_csv(instance)
        df_variables2 = df.drop(['CUSTOMER CUST NO.', ' DEMAND', 'READY TIME',
                                'SERVICE TIME', 'AVAIL. TIME', 'DUE DATE'], axis=1)
        df_variables = df_variables2.drop([0], axis=0)
        bestKmeans = findToBestK(df_variables, Kinit, K, mode, 300)
        return bestKmeans, df_variables

def readInstanceConstrained(
    instance: str, 
    Kinit: int, 
    K: int, 
    type: bool, 
    mode: str
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
        sizeMax = int(len(data["deliveries"])/K)+1 # vehicle_MAX/5 + 1
        sizeMin = 24
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
        bestKmeans = findToBestK(df_variables, Kinit, K, mode, 300)
        return bestKmeans, df_variables


def findToBestK(
    dataframe: pd.DataFrame,
    Kinit: int,
    K: int,
    initializer: str,
    iterations: int
) -> KMeans:
    range_n_clusters = [cluster for cluster in range(Kinit, K)]
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
        K: int,
        timeExecution):
    content = dict()
    content['num_veiculos'] = K
    content['time_execution'] = timeExecution 
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
    # print(jstr)
    with open(nameInstance, "w") as outfile:
        outfile.write(jstr)