#!/bin/bash

dir=foo
n=15000
mkdir -p foo

for i in $(seq 1 $n);
do
    echo $i > foo/$i
done