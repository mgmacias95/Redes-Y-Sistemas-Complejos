#! /usr/bin/env python
# -*- coding: utf-8 -*-

from subprocess import Popen, PIPE, run, TimeoutExpired
from tabulate import tabulate
import time

l_max = lambda n: (n*(n-1))/2

# tamaños de las redes a generar
ns = [500, 1000, 2000, 5000, 10000]
# algoritmos pathfinder a ejecutar
algs = ["../OriginalPathfinder/pathfinder", "../BinaryPathfinder/binary-pathfinder", 
        "../Fast-Pathfinder/fast-pathfinder", "../MST-Pathfinder/mst-pathfinder_the",
        "../MST-Pathfinder/mst-pathfinder_pract"]

# realizamos las ejecuciones. Todas serán con r=infty y q=n-1
table_row = []
for n in ns:
    tiempos_poda = []
    # generamos la red
    # Usage:
    with open("prueba"+str(n)+".txt","w") as out:
        red = Popen(["./create-mod",   # ./create-mod
            str(n),                    # <size>
            '1',                       # <symmetry>
            '0',                       # <diagonal-value>
            '0',                       # <weight-type>
            '50',                      # <min>
            '500',                     # <max>
            '0.5'],                    # <edge-prob>
            stdout=out)

    for a in algs:
        # la podamos midiendo el tiempo que tardamos con un limite de una hora
        try:
            antes = time.time()
            run([a, "prueba"+str(n)+".txt", str(n-1)], timeout=3600, stdout=PIPE)
            despues = time.time()
            tiempos_poda.append("$"+str(despues - antes)+"$")
        except TimeoutExpired as t:
            tiempos_poda.append("> 3600")

    table_row.append(tiempos_poda)

# generamos la tabla 
table = [r for r in table_row]
print(tabulate(table, headers=["Original", "Binary", "Fast", "MST (Teorico)", "MST (Práctico)"], tablefmt="latex"))
