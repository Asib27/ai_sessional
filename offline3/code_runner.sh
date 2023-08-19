#!/bin/bash

g++ main.cpp  -o main

echo "Name,n,m,random,greedy,semi-greedy,local search iteration,local search avg value,GRASP iteration,GRASP value" > results.csv
for file in benchmark/* ; do
    echo $file
    cp $file input.txt
    ./main $file < input.txt
done
