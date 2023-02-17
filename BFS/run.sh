#!/bin/bash

if [ $1 = BFS ]
then
    ./build/BfsAlgorithm Graphs/15k.txt Graphs/res.txt
elif [$1 = PROFILING]
then 
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/Documents/Thesis/margot/paho.mqtt.c/build/src/
    ./build/BfsProfiling Graphs/15k.txt Graphs/res.txt
fi