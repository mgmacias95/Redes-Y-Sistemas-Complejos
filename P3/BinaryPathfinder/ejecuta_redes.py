#! /usr/bin/env python
# -*- coding: utf-8 -*-

from subprocess import Popen, PIPE
from tabulate import tabulate

l_max = lambda n: (n*(n-1))/2

# cienciogramas a analizar
cienciogramas = ["Argentina-2005.net", "Canada-2005.net", "Chile-2004.net", 
                "Cuba-2004.net", "Spain-2002.net", "Germany-2002.net", 
                "Japan-2002.net", "Mexico-2005.net", "United_States-2002.net",
                "World.net"]

# número de vértices de cada cienciograma
vertex_num = []

# parámetros q con los que realizar las distintas ejecuciones del pathfinder
# además de con todos estos, usaremos también n-1
qs = [1,2,3,4,5]

# Función para sacar el número de vértices de una red de citaciones científicas
def get_num_vertex(cienciograma):
    with open("../cienciogramas/" + cienciograma, 'r') as f:
        # leemos la primera línea del archivo, que es donde se especifican el
        # número de vértices de la red
        linea = f.readline()
        vertex = linea.split()[1]

    return int(vertex)


# obtenemos el número de vértices de cada cienciograma
for cg in cienciogramas:
    vertex_num.append(get_num_vertex(cg)-1)

# juntamos ambas listas para que sea más fácil iterar sobre ella
cienciogramas = list(zip(cienciogramas, vertex_num))

# realizamos las ejecuciones 
for cg in cienciogramas:
    lista_enlaces = []
    lista_densidad = []
    for q in qs + [cg[1]]:
        # podamos la red
        proc = Popen(["./binary-pathfinder", "../cienciogramas/" + cg[0], str(q)], stdout=PIPE)
        # contamos el número de enlaces que obtenemos
        enlaces = proc.stdout.read().split(b'*edges\n')[1].count(b'\n')
        lista_enlaces.append(enlaces)
        # calculamos la densidad de la red
        densidad = enlaces/l_max(cg[1]+1)
        lista_densidad.append(densidad)
    # generamos la tabla para el cienciograma actual
    table = [[(['Red original'] + qs[1:] + [cg[1]])[i], lista_enlaces[i], lista_densidad[i]]
            for i in range(len(lista_enlaces))]
    print(tabulate(table, headers=[cg[0] + '\n' + '(n='+str(cg[1]+1)+')',
        "Número de enlaces", "Densidad"], tablefmt="latex"))

