from graph_tool.all import *
from math import log
import numpy as np


def compute_exponent_network(g, type="in"):
    # obtenemos el número de nodos en la red con cada uno de los grados
    num, degs = vertex_hist(g, deg=type)
    not_zeros = np.where(num != 0)
    num = num[not_zeros]
    degs = degs[not_zeros]
    num /= g.num_vertices()
    gamma = 0

    for pk, deg in zip(num[2:], degs[2:]):
        # print(deg)
        # print(pk)
        gamma += -(log(pk) / log(deg))

    return gamma / (not_zeros[0].size-2)

g = load_graph("grafo.graphml")
g.set_directed(True)
print("gamma in: ", compute_exponent_network(g=g))
print("gamma out: ", compute_exponent_network(g=g, type="out"))
