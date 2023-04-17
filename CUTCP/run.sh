#!/bin/bash

if [ $1 = ALGORITHM ]
then
    ./build/CutcpAlgorithm \
    -I data/in/watbox.sl40.pqr -O data/out/exclusion.txt
fi