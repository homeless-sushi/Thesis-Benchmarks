#!/bin/bash

if [ $1 = BFS ]
then
    ./build/BfsAlgorithm -I Graphs/v15k_e9_l.txt -O Graphs/res.txt
elif [$1 = PROFILING]
then 
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/Documents/Thesis/margot/paho.mqtt.c/build/src/
    ./build/BfsProfiling Graphs/v15k_e9_l.txt Graphs/res.txt
fi
