#!/bin/bash

if [ $1 = BFS ]
then
    ./build/BfsAlgorithm \
    -I Graphs/v15k_e9_l.txt -O Graphs/res.txt \
    -D GPU \
    --gpu-block-dim 64 \
    --gpu-chunk-factor 2 \
    --gpu-offset-mem TEXTURE \
    --gpu-edge-mem TEXTURE 
elif [ $1 = BENCHMARK ]
then 
    INPUT="Graphs/v15k_e9_l.txt"
    if [ ! -z "$2" ]
    then
        INPUT="$2"
    else 
        echo "No input graph supplied; default input graph is used: $INPUT"
    fi

    ./build/BfsBenchmark -I $INPUT -O Graphs/res.txt
elif [ $1 = PROFILING ]
then 
    INPUT="Graphs/v15k_e9_l.txt"
    if [ ! -z "$2" ]
    then
        INPUT="$2"
    else 
        echo "No input graph supplied; default input graph is used: $INPUT"
    fi
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/Documents/Thesis/margot/paho.mqtt.c/build/src/
    ./build/BfsProfiling -I $INPUT -O Graphs/res.txt
fi
