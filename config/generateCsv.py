import os
import csv
import json
from loggibud.v1.eval.task1 import evaluate_solution
# Gerar 1 arquivo Geral sobre dados comparativos
# Algorithm | input | K | distance | n_veiculos | VAR(dif de max e min de pacotes/veiculos) | time 
# Ler arquivo por arquivo
# Captar os dados desejados
def generateGeneralCsv(path_outcsv: str, city: str, output: str, path_input: str):
    head = ["Algorithm", "input", "K", "distance", "n_veiculos", "time"]
    f = open(path_outcsv, 'w', newline='', encoding='utf-8')
    w = csv.writer(f)
    # Construir Cabeçalho
    w.writerow(head) 
    # Read city per city
    outputs = [
        os.path.join(output, solution) 
        for solution in os.listdir(output)
    ]
    for solution in outputs:
        path_broke = solution.split('/')
        name_instance = path_broke[len(path_broke)-1]
        instance = path_input + name_instance
        jfile = open(solution)
        root = json.load(jfile) 
        line = ["K-PPR-RF"]
        line.append(root['name'])
        line.append(root['k-regions'])
        line.append(evaluate_solution(instance, solution))
        line.append(root['total_vehicles'])
        line.append(root['time'])
        w.writerow(line)
        instance_broke = instance.split('.')
        name_broke = instance_broke.split('-')
        day = name_broke[len(name_broke)-1]
        generateEspecificDay(city, day, root)

    w.close()

# Gerar 1 arquivo especifico sobre os veículos name-day
# id_vehicle | capacity_used | n_deliveries
def generateEspecificDay(city: str, day: int, root):
    path_out = "output/csvs/"+city+"/especificDay-"+day+".csv"
    head = ["id_vehicle", "capacity_used", "n_deliveries"]
    f = open(path_out, 'w', newline='', encoding = 'utf-8')
    w = csv.writer(f)
    w.writerow(head)
    for id in range(len(root['vehicles'])):
        line = [id]
        line.append(computeCapacityRoute(root['vehicles'][id]))
        line.append(len(root['vehicles'][id]['deliveries']))
        w.writerow(line)
    w.close()

def computeCapacityRoute(vehicle):
    s = 0
    for delivery in vehicle['deliveries']:
        s += delivery['size']
    return s

def main():
    