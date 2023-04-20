#!/bin/bash

if [ $1 = BFS ]
then
    ./build/BfsAlgorithm \
    -I data/in/v15k_e9_l.txt -O data/out/res.txt \
    -D GPU \
    --gpu-block-dim 64 \
    --gpu-chunk-factor 2 \
    --gpu-offset-mem TEXTURE \
    --gpu-edge-mem TEXTURE 
elif [ $1 = BENCHMARK ]
then 
    INPUT="data/in/v15k_e9_l.txt"
    if [ ! -z "$2" ]
    then
        INPUT="$2"
    else 
        echo "No input graph supplied; default input graph is used: $INPUT"
    fi

    INSTANCE_NAME=""
    if [ ! -z "$3" ]
    then
        INSTANCE_NAME="--instance-name $3 "
    fi

    TARGET_THROUGHPUT=""
    if [ ! -z "$4" ]
    then
        TARGET_THROUGHPUT="--target-throughput $4 "
    fi

    ./build/BfsBenchmark -I $INPUT -O data/out/res.txt $INSTANCE_NAME $TARGET_THROUGHPUT
elif [ $1 = PROFILING ]
then 
    INPUT="data/in/v15k_e9_l.txt"
    if [ ! -z "$2" ]
    then
        INPUT="$2"
    else 
        echo "No input graph supplied; default input graph is used: $INPUT"
    fi
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/Documents/Thesis/margot/paho.mqtt.c/build/src/
    ./build/BfsProfiling -I $INPUT -O data/out/res.txt
fi
