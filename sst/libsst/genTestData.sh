#!/bin/sh

for i in `seq 1 $1`; do 
    for j in `seq 1 $1`; do 
	echo -n "$RANDOM "; 
    done; 
    echo ""; 
done