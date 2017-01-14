from graph_tool.all import *
from ggplot import *
import pandas as pd


def plot_distance_hist(g):
    d = distance_histogram(g)
    
    pdd = pd.DataFrame(d).T 

    max_x = max(pdd[1])
    max_y = max(pdd[0])

    p = ggplot(pdd, aes(x=1, y=0)) + geom_point() + xlab("Distance") + \
        ylab("# of paths") + ylim(0,max_y) + xlim(0, max_x) + geom_line()
    p.save("memoria/dist_hist.png")

def plot_clustering_coeff(g, type="total"):
    c = local_clustering(g, undirected=False).get_array()
    d = g.degree_property_map(deg=type).get_array()
    pdd = pd.DataFrame({'c':c, 'k':d})

    p = ggplot(pdd, aes(x='k', y='c')) + geom_point() + xlab("Degree") + \
        ylab("Clustering coef.") + ylim(min(pdd['c']), max(pdd['c'])) + \
        xlim(min(pdd['k']), max(pdd['k']))
    p.save("memoria/clust_hist.png")


# cargamos el grafo
g = load_graph("grafo.graphml")
# lo convertimos en directo
g.set_directed(True)
# sacamos el histograma de grado de entrada
plot_distance_hist(g)
plot_clustering_coeff(g)
