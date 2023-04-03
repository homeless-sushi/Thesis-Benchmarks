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
    ./build/BfsBenchmark build/BfsAlgorithm Graphs/v15k_e9_l.txt Graphs/res.txt
elif [ $1 = PROFILING ]
then 
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/Documents/Thesis/margot/paho.mqtt.c/build/src/
    ./build/BfsProfiling -I Graphs/v15k_e9_l.txt -O Graphs/res.txt
fi
