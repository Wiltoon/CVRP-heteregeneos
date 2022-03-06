import csv
# Gerar 1 arquivo Geral sobre dados comparativos
# Algorithm | input | K | distance | n_veiculos | VAR(dif de max e min de pacotes/veiculos) | time 
# Ler arquivo por arquivo
# Captar os dados desejados
def generateGeneralCsv(path_outcsv: str, city: str):
    head = ["Algorithm", "input", "K", "distance", "n_veiculos", "time"]
    f = open(path_outcsv, 'w', newline='', encoding='utf-8')
    w = csv.writer(f)
    # Construir Cabeçalho
    w.writerow(head) 
    # Read city per city
    for instance in output_data:
        root = instance.root
        line = ["K-PPRRF"]
        line.append(root['name'])
        line.append(root['k-regions'])
        line.append(extractDistance(root))
        line.append(root['total_vehicles'])
        line.append(root['time'])
        w.writerow(line)
        generateEspecificDay(city, day, root)

    w.close()

# Gerar 1 arquivo especifico sobre os veículos name-day
# id_vehicle | capacity_used | n_deliveries
def generateEspecificDay(city: str, day: int, root):
    path_out = "caminho/a/definir/"+city+"/especificDay-"+day+".csv"
    head = ["id_vehicle", "capacity_used", "n_deliveries"]
    f = open(path_out, 'w', newline='', encoding = 'utf-8')
    w = csv.writerow(head)
    for id in range(len(root['vehicles'])):
        line = [id]
        line.append(computeCapacityRoute(root['vehicles'][id]))
        line.append(len(root['vehicles']['deliveries']))
        w.writerow(line)
    w.close()

def computeCapacityRoute(deliveries):

def extractDistance(root):
