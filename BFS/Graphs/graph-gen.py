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
    
    outfile = args.outfile
    n_vertices = args.vertex
    avg_degree = args.edge or np.floor(np.sqrt(n_vertices))
    local = args.local
    
    n_edges = 0
    vertices = []
    # the degree for the vertices is a truncated normal around the mean
    rng_vertices_degrees = truncnorm(0, n_vertices, avg_degree, avg_degree/4)
    for vertex in range(n_vertices) :
        vertex_degree = get_random_degree(rng_vertices_degrees, avg_degree)
        n_edges+=vertex_degree
        rng_edges = np.random.default_rng()
        if local :
            vertex_edges = get_random_local_edges(rng_edges, vertex, vertex_degree, n_vertices)
        else :
            vertex_edges = get_random_edges(rng_edges, vertex, vertex_degree, n_vertices)
        vertices.append(vertex_edges)
    
    write_graph_file(outfile, n_vertices, n_edges, vertices)
    
    return

def get_random_degree(rng : np.random, mean: int) :
    return int(np.trunc(truncnorm.rvs(-1,1,mean,mean/5)))

def get_random_local_edges(rng : np.random, vertex: int, degree: int, n_vertices: int) :
    edges = set()
    edges.add(vertex)

    low = vertex - degree*2
    high = vertex + degree*2
    if(low < 0) :
        remaining = -low
        low = 0
        high += remaining
    elif(high > n_vertices) :
        remaining = high - n_vertices
        high = n_vertices
        low-=remaining
    
    while(len(edges) <= degree) :
        edge = int(np.trunc(rng.uniform(low, high)))
        edges.add(edge)

    edges.remove(vertex)
    return sorted(edges)


def get_random_edges(rng : np.random, vertex: int, degree: int, n_vertices: int) :
    edges = set()
    edges.add(vertex)
   
    while(len(edges) <= degree) :
        edge = int(np.trunc(rng.uniform(0, n_vertices)))
        edges.add(edge)

    edges.remove(vertex)
    return sorted(edges)

def write_graph_file(filename, n_vertices, n_edges, vertices) : 
    file = open(filename, 'w')
    file.write(f"{n_vertices} {n_edges}\n")
    for vs in vertices :
        for v in vs :
            file.write(f" {v}")
        file.write("\n")

    file.close()
    
if __name__ == "__main__" :
    main()