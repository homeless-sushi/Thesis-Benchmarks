import argparse

import numpy as np
from scipy.stats import truncnorm

def main() :
    parser = argparse.ArgumentParser(
        prog = 'graph-gen',
        description = 'Generate a graph with the specified parameters')
    
    parser.add_argument('outfile')      
    parser.add_argument('-v', '--vertex',
        help="Number of vertices",
        type=int,
        default=2**20)
    parser.add_argument('-e', '--edge',
        help="Average degree (number of egdes) of vertices",
        type=int,
        required=False)
    parser.add_argument('-l', '--local',
        help="Whether edges are local",
        action='store_true')

    args = parser.parse_args()
    
    outfileURL = args.outfile
    outfile = open(outfileURL, 'w')

    n_vertices = args.vertex

    avg_degree = args.edge or np.floor(np.sqrt(n_vertices))
    n_edges = 0
    degrees = []
    for vertex in range(n_vertices) :
        vertex_degree = get_random_degree(avg_degree)
        degrees.append(vertex_degree)
        n_edges+=vertex_degree

    write_graph_info(outfile, n_vertices, n_edges)
       
    local = args.local
    for vertex in range(n_vertices) :

        vertex_degree = degrees[vertex]
        n_edges+=vertex_degree

        rng_edges = np.random.default_rng()
        if local :
            vertex_edges = get_random_local_edges(rng_edges, vertex, vertex_degree, n_vertices)
        else :
            vertex_edges = get_random_edges(rng_edges, vertex, vertex_degree, n_vertices)

        write_vertex_edges(outfile, vertex_edges)

    outfile.close()
    
    return

def get_random_degree(mean: int) :
    delta = int(np.trunc(np.sqrt(mean)))
    low = mean - delta
    high = mean + delta
    return int(np.trunc(np.random.uniform(low, high)))

def get_random_local_edges(rng : np.random, vertex: int, degree: int, n_vertices: int) :
    low = vertex - degree*2
    high = vertex + degree*2

    if(low < 0) :
        remaining = -low
        low = 0
        high += remaining

    if(high > n_vertices) :
        remaining = high - n_vertices
        high = n_vertices
        low -= remaining
    
    available = [x for x in range(low, high)]
    available.remove(vertex)

    edges = []
    while(len(edges) < degree) :
        index = int(np.trunc(rng.uniform(0, len(available))))
        edge = available[index]
        available.pop(index)
        edges.append(edge)

    return sorted(edges)


def get_random_edges(rng : np.random, vertex: int, degree: int, n_vertices: int) :
    available = [x for x in range(n_vertices)]
    available.remove(vertex)

    edges = []
    while(len(edges) < degree) :
        index = int(np.trunc(rng.uniform(0, len(available))))
        edge = available[index]
        available.pop(index)
        edges.append(edge)

    return sorted(edges)

def write_graph_info(file, n_vertices, n_edges) :
    file.write(f"{n_vertices} {n_edges}\n")

def write_vertex_edges(file, edges) :
    for e in edges :
        file.write(f" {e}")
    file.write(f"\n")
    
if __name__ == "__main__" :
    main()