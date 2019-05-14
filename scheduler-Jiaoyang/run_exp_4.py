import os

algs = ["BASE", "FCFS", "FLFS"]
input_dir = "./input-files/"
instances = [input_dir + f"random_100_{i}.yaml" for i in range(5)]
configs = [input_dir + f"configs_exp4/config_p0.yaml" ]+ \
          [input_dir + f"configs_exp4/config_p0.{i}.yaml" for i in [2, 4, 6,8]]

for alg in algs:
    for conf in configs:
        for ins in instances:
            command = f"./solver --depart=./input-files/links.txt --output=./input-files/output --node=./input-files/nodes.txt --runway=./input-files/runway_nodes.txt --link=./input-files/links.txt --spot=./input-files/spot_nodes.txt -i {ins} -m ./input-files/aircraft-model.yaml -s {alg} -k 100 --interval_min=0 --interval_max=10 -c {conf}"
            os.system(command)